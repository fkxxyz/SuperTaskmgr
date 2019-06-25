
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrCommand.h"
#include <Psapi.h>

C360TdiFilter CSuperTaskmgrDlg::TdiFilter;
extern DWORD (WINAPI *SHRunDialog)(HWND,DWORD,LPVOID,LPVOID,LPVOID,DWORD);


//“刷新”命令
void CSuperTaskmgrDlg::OnRefresh(){
	Refresh(TRUE);
}

//新建任务，“运行”对话框
void CSuperTaskmgrDlg::OnNewTask()
{
	if (stOsVerInfo.dwPlatformId==VER_PLATFORM_WIN32_NT)
		SHRunDialog(
		m_hWnd,
		0xF3C061F,
		L"C:\\WINDOWS",
		L"创建新任务",
		L"Windows 将根据你所输入的名称，为你打开相应的程序、文件夹、文档或 Internet 资源。",
		0x54
		);
	else
		SHRunDialog(
		m_hWnd,
		0xF3C061F,
		L"C:\\WINDOWS",
		"创建新任务","Windows 将根据你所输入的名称，为你打开相应的程序、文件夹、文档或 Internet 资源。",
		0x54
		);
}

//“结束进程”菜单
void CSuperTaskmgrDlg::OnKill()
{
	DWORD dwLastError;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (!KillProcess(pListPro[i]->PID)){
			dwLastError=GetLastError();
			if (m_LstProc.GetSelectedCount()==1)
				ShowError(m_hWnd,_T("无法终止进程"),_T("结束进程失败！\n"),_T(""),dwLastError);
		}
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateKill(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//“强制结束进程”菜单
void CSuperTaskmgrDlg::OnUpdateForceKill(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

//“一键结束进程”按钮
void CSuperTaskmgrDlg::OnBnClickedOnekeyKill()
{
	int i,l;
	l=m_LstProc.GetItemCount();
	for (i=0;i<l;i++){
		if (pListPro[i]->Checked){
			KillProcess(pListPro[i]->PID);
		}
	}
	Sleep(32);
	Refresh();
}

//弹出式子菜单空函数
void CSuperTaskmgrDlg::OnMenuItemNull(){}
void CSuperTaskmgrDlg::OnUpdateMenuItemNull(CCmdUI* pCmdUI){}

//“优先级”菜单
void CSuperTaskmgrDlg::OnUpdateMenuItemPriority(CCmdUI* pCmdUI) 
{
	if(pCmdUI->m_pSubMenu)
		pCmdUI->m_pMenu->EnableMenuItem(2, MF_BYPOSITION |
			(m_LstProc.GetSelectedCount() ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
}

BOOL CSuperTaskmgrDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CMenu *menu;
	BOOL ckms[7]={0};
	int i;
	int pn;

	if (!pHandlerInfo && nID>=40000){
		if (nID>=40500 && nID<40509){ //优先级菜单
			if (nCode==0){ //On Command
				pn=nID-40500; //优先级序号
				BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
					CProcess Process;
					if (Process.Open(pListPro[i]->PID,PROCESS_SET_INFORMATION))
						Process.SetPriorityClass(PriorityClass[pn]);//设置优先级
				END_ENUM_SELECTED_ITEM

				Sleep(32);
				Refresh();
			} else if (nCode==-1 && nID==40501){ //On Update
				//获取所选中的进程优先级
				BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
					ckms[PriorityIndex(pListPro[i]->Priority)]=TRUE;
				END_ENUM_SELECTED_ITEM

				//复选对应的菜单
				menu=m_Menu.GetSubMenu(1)->GetSubMenu(2);
				for (i=1;i<=6;i++)
					menu->CheckMenuItem(6-i,MF_BYPOSITION | (ckms[i]?MF_CHECKED:0));
			}
		} else if (nID>=41300 && nID<41309){ //刷新频率菜单
			if (nCode==0){ //On Command
				setting.RefreshSpeed=nID-41300;
				SetTimer(1,TimerInterval[setting.RefreshSpeed],NULL);
			} else if (nCode==-1 && nID==41300){ //On Update
				ckms[setting.RefreshSpeed]=TRUE;

				//复选对应的菜单
				menu=m_Menu.GetSubMenu(3)->GetSubMenu(5);
				for (i=0;i<=5;i++)
					menu->CheckMenuItem(5-i,MF_BYPOSITION | (ckms[i]?MF_CHECKED:0));
			}
		}
	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//“清空专用工作集”菜单
void CSuperTaskmgrDlg::OnEmptyWorkingSet()
{
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		CProcess Process;
		if (Process.Open(pListPro[i]->PID,PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA)){
			Process.EmptyWorkingSet();
		}
		m_LstProc.SetItemState(i,0,LVIS_SELECTED);
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateEmptyWorkingSet(CCmdUI *pCmdUI)
{
	BOOL bEnabled=FALSE;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (!pListPro[i]->isSuspended){
			bEnabled=TRUE;
			break;
		}
	END_ENUM_SELECTED_ITEM
	pCmdUI->Enable(bEnabled);
}


//“暂停进程运行”菜单
void CSuperTaskmgrDlg::OnSuspend()
{
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		CProcess Process;
		DWORD dwLastError;
		if (!Process.Open(pListPro[i]->PID,PROCESS_SUSPEND_RESUME)){
			dwLastError=GetLastError();
			goto er;
		}
		NTSTATUS ntstatus=Process.NtSuspend();
		if (!NT_SUCCESS(ntstatus)){
			dwLastError=RtlNtStatusToDosError(ntstatus);
			goto er;
		}
		goto lContinue;
er:
		ShowError(m_hWnd,_T("无法挂起进程"),_T("挂起进程失败！\n"),_T(""),dwLastError);
lContinue:
		m_LstProc.SetItemState(i,0,LVIS_SELECTED);
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateSuspend(CCmdUI *pCmdUI)
{
	BOOL bEnabled=FALSE;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (!pListPro[i]->isSuspended){
			bEnabled=TRUE;
			break;
		}
	END_ENUM_SELECTED_ITEM
	pCmdUI->Enable(bEnabled);
}

//“恢复进程运行”菜单
void CSuperTaskmgrDlg::OnResume()
{
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		CProcess Process;
		DWORD dwLastError;
		if (!Process.Open(pListPro[i]->PID,PROCESS_SUSPEND_RESUME)){
			dwLastError=GetLastError();
			goto er;
		}
		NTSTATUS ntstatus=Process.NtResume();
		if (!NT_SUCCESS(ntstatus)){
			dwLastError=RtlNtStatusToDosError(ntstatus);
			goto er;
		}
		goto lContinue;
er:
		ShowError(m_hWnd,_T("无法恢复进程"),_T("恢复进程失败！\n"),_T(""),dwLastError);
lContinue:
		m_LstProc.SetItemState(i,0,LVIS_SELECTED);
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateResume(CCmdUI *pCmdUI)
{
	BOOL bEnabled=FALSE;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (pListPro[i]->isSuspended){
			bEnabled=TRUE;
			break;
		}
	END_ENUM_SELECTED_ITEM
	pCmdUI->Enable(bEnabled);
}


//“网速限制”菜单
void CSuperTaskmgrDlg::OnLimitSpeed()
{
	VERIFY(TdiFilter.GetProcessLimited(&stProcessLimited));

	int i,j;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		break;
	END_ENUM_SELECTED_ITEM

	for (j=0;j<stProcessLimited.nProcess;j++){
		if (_tcsicmp(stProcessLimited.limit[j].szPath,pListPro[i]->path)==0){
			break;
		}
	}
	if (j==stProcessLimited.nProcess){
		memset(&stProcessLimited.limit[j],0,sizeof(stProcessLimited.limit[j]));
	}
	LimitSpeedDlg.SetProcess(&stProcessLimited.limit[j],pListPro[i]);

	RECT rWinRect;
	GetWindowRect(&rWinRect);
	LimitSpeedDlg.SetWindowPos(this,
		(rWinRect.right+rWinRect.left)/2-80,
		(rWinRect.bottom+rWinRect.top)/2-80,
		0,0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
}
void CSuperTaskmgrDlg::OnUpdateLimitSpeed(CCmdUI *pCmdUI)
{
	if (m_LstProc.GetSelectedCount()!=1){
		pCmdUI->Enable(FALSE);
		return;
	}
	BOOL bEnabled=FALSE;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (pListPro[i]->NetSendSpeed!=-2l && pListPro[i]->NetRecvSpeed!=-2l){
			bEnabled=TRUE;
			break;
		}
	END_ENUM_SELECTED_ITEM
	pCmdUI->Enable(bEnabled);
}


//“禁止访问网络”菜单
void CSuperTaskmgrDlg::OnDisableNetwork()
{
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (!pListPro[i]->bDisabledNetwork){
			if (*pListPro[i]->path){
				TdiFilter.DisableProcessNetwork(pListPro[i]->path);
			}
		}
		m_LstProc.SetItemState(i,0,LVIS_SELECTED);
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateDisableNetwork(CCmdUI *pCmdUI)
{
	BOOL bEnabled=FALSE;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (!pListPro[i]->bDisabledNetwork && pListPro[i]->NetSendSpeed!=-2l && pListPro[i]->NetRecvSpeed!=-2l){
			bEnabled=TRUE;
			break;
		}
	END_ENUM_SELECTED_ITEM
	pCmdUI->Enable(bEnabled);
}

//“允许访问网络”菜单
void CSuperTaskmgrDlg::OnAllowNetwork()
{
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (pListPro[i]->bDisabledNetwork){
			if (*pListPro[i]->path){
				TdiFilter.EnableProcessNetwork(pListPro[i]->path);
			}
		}
		m_LstProc.SetItemState(i,0,LVIS_SELECTED);
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}

void CSuperTaskmgrDlg::OnUpdateAllowNetwork(CCmdUI *pCmdUI)
{
	BOOL bEnabled=FALSE;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (pListPro[i]->bDisabledNetwork){
			bEnabled=TRUE;
			break;
		}
	END_ENUM_SELECTED_ITEM
	pCmdUI->Enable(bEnabled);
}

//设置名单的操作
void CSuperTaskmgrDlg::ListMenuCommand(BOOL &bSelAllVar,BOOL bChangeSelected,BOOL bInListSelect,LPTSTR *List,int &ListN,INT_PTR iProStOffset){
	int i,j;
	if (bSelAllVar){
		//从名单中移除
		BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
			if (InList(List,ListN,pListPro[i]->name)){
				for (j=0;j<ListN;j++){
					if (_tcscmp(List[j],pListPro[i]->name)==0){
						List[j]=List[--ListN];
						break;
					}
				}
			}
			if (bChangeSelected)
				m_LstProc.SetItemState((int)i,bInListSelect?0:LVIS_SELECTED,LVIS_SELECTED);
		END_ENUM_SELECTED_ITEM
	} else {
		//加入名单
		BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
			if (!InList(List,ListN,pListPro[i]->name)){;
				List[ListN++]=SPNameContainer.NewData(pListPro[i]->name,_tcslen(pListPro[i]->name)+1);
			}
			if (bChangeSelected)
				m_LstProc.SetItemState((int)i,bInListSelect?LVIS_SELECTED:0,LVIS_SELECTED);
		END_ENUM_SELECTED_ITEM
	}
	for (i=0;i<proN;i++)
		*(BOOL *)((INT_PTR)pro[i]+iProStOffset)=InList(List,ListN,pro[i]->name);
}

//“设置黑名单”菜单
void CSuperTaskmgrDlg::OnSetBlack()
{
	bSelAllSystem=TRUE;
	ListMenuCommand(bSelAllSystem,FALSE,FALSE,setting.SystemProcess,setting.nSystemProcess,(INT_PTR)&pro[0]->inSystem-*(INT_PTR*)&pro[0]);
	bSelAllWhite=TRUE;
	ListMenuCommand(bSelAllWhite,FALSE,FALSE,setting.WhiteList,setting.nWhiteList,(INT_PTR)&pro[0]->inWhite-*(INT_PTR*)&pro[0]);
	ListMenuCommand(bSelAllBlack,TRUE,TRUE,setting.BlackList,setting.nBlackList,(INT_PTR)&pro[0]->inBlack-*(INT_PTR*)&pro[0]);
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateSetBlack(CCmdUI *pCmdUI)
{
	ListUpdateMenu(bSelAllBlack,(INT_PTR)&pro[0]->inBlack-*(INT_PTR*)&pro[0]);
	pCmdUI->SetText(bSelAllBlack?_T("从黑名单中移除(&B)"):_T("加入黑名单(&B)"));
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//“设置白名单”菜单
void CSuperTaskmgrDlg::OnSetWhite()
{
	bSelAllBlack=TRUE;
	ListMenuCommand(bSelAllBlack,FALSE,TRUE,setting.BlackList,setting.nBlackList,(INT_PTR)&pro[0]->inBlack-*(INT_PTR*)&pro[0]);
	ListMenuCommand(bSelAllWhite,TRUE,FALSE,setting.WhiteList,setting.nWhiteList,(INT_PTR)&pro[0]->inWhite-*(INT_PTR*)&pro[0]);
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateSetWhite(CCmdUI *pCmdUI)
{
	ListUpdateMenu(bSelAllWhite,(INT_PTR)&pro[0]->inWhite-*(INT_PTR*)&pro[0]);
	pCmdUI->SetText(bSelAllWhite?_T("从白名单中移除(&W)"):_T("加入白名单(&W)"));
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//“设置系统进程”菜单
void CSuperTaskmgrDlg::OnSetSystem()
{
	bSelAllBlack=TRUE;
	ListMenuCommand(bSelAllBlack,FALSE,TRUE,setting.BlackList,setting.nBlackList,(INT_PTR)&pro[0]->inBlack-*(INT_PTR*)&pro[0]);
	bSelAllWhite=TRUE;
	ListMenuCommand(bSelAllWhite,FALSE,FALSE,setting.WhiteList,setting.nWhiteList,(INT_PTR)&pro[0]->inWhite-*(INT_PTR*)&pro[0]);
	ListMenuCommand(bSelAllSystem,TRUE,FALSE,setting.SystemProcess,setting.nSystemProcess,(INT_PTR)&pro[0]->inSystem-*(INT_PTR*)&pro[0]);
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateSetSystem(CCmdUI *pCmdUI)
{
	ListUpdateMenu(bSelAllSystem,(INT_PTR)&pro[0]->inSystem-*(INT_PTR*)&pro[0]);
	pCmdUI->SetText(bSelAllSystem?_T("取消系统进程(&S)"):_T("设为系统进程(&S)"));
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//“查看文件属性”菜单
void CSuperTaskmgrDlg::OnAttrib()
{
	SHELLEXECUTEINFO sei;
	sei.cbSize=sizeof(SHELLEXECUTEINFO);
	sei.fMask=SEE_MASK_NOCLOSEPROCESS | SEE_MASK_INVOKEIDLIST | SEE_MASK_FLAG_NO_UI;
	sei.hwnd=m_hWnd;
	sei.lpVerb=_T("properties");
	sei.lpParameters=NULL;
	sei.lpDirectory=NULL;
	sei.nShow=NULL;
	sei.hInstApp=NULL;
	sei.lpIDList=NULL;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		sei.lpFile=pListPro[i]->path;
		ShellExecuteEx(&sei);
	END_ENUM_SELECTED_ITEM
}
void CSuperTaskmgrDlg::OnUpdateAttrib(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//“打开所在目录”菜单
void CSuperTaskmgrDlg::OnDirectory()
{
	TCHAR lpszFilePath[MAX_PATH];
	TCHAR lpszParameter[MAX_PATH];
	int i;
	_stprintf_s(lpszFilePath,MAX_PATH,_T("%s\\explorer.exe"),lpszSystemRoot);
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		_stprintf_s(lpszParameter,MAX_PATH,_T("/select,\"%s\""),pListPro[i]->path);
		ShellExecute(NULL,_T("open"),lpszFilePath,lpszParameter,NULL,SW_SHOWNORMAL);
	END_ENUM_SELECTED_ITEM
}

void CSuperTaskmgrDlg::OnUpdateDirectory(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//“全选”菜单
void CSuperTaskmgrDlg::OnSelAll()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		pListPro[i]->Selected=TRUE;
	}
	m_OneKeyKill.EnableWindow(n);
}

//“全选”按钮
void CSuperTaskmgrDlg::OnBnClickedButtomSelAll()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetCheck(i,TRUE);
	}
	m_OneKeyKill.EnableWindow(n);
	m_LstProc.SetFocus();
}

//“反选”菜单
void CSuperTaskmgrDlg::OnSelRev()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetItemState(i,LVIS_SELECTED-m_LstProc.GetItemState(i,LVIS_SELECTED),LVIS_SELECTED);
		pListPro[i]->Selected=!pListPro[i]->Selected;
	}
	for (i=0;i<n;i++){
		if (m_LstProc.GetCheck(i))
			break;
	}
	m_OneKeyKill.EnableWindow(i!=n);
}

//“反选”按钮
void CSuperTaskmgrDlg::OnBnClickedSelRev()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetCheck(
			i,
			!m_LstProc.GetCheck(i)
		);
	}
	for (i=0;i<n;i++){
		if (m_LstProc.GetCheck(i))
			break;
	}
	m_OneKeyKill.EnableWindow(i!=n);
	m_LstProc.SetFocus();
}

//“非白名单”菜单
void CSuperTaskmgrDlg::OnSelNotWhite()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetItemState(i,(!pListPro[i]->inWhite&&!pListPro[i]->inSystem)?LVIS_SELECTED:0,LVIS_SELECTED);
		pListPro[i]->Selected=!pListPro[i]->inWhite&&!pListPro[i]->inSystem;
	}
	for (i=0;i<n;i++){
		if (m_LstProc.GetCheck(i))
			break;
	}
	m_OneKeyKill.EnableWindow(i!=n);
}

//“非白名单”按钮
void CSuperTaskmgrDlg::OnBnClickedSelNotWhite()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetCheck(
			i,
			!pListPro[i]->inWhite && !pListPro[i]->inSystem
		);
	}
	for (i=0;i<n;i++){
		if (m_LstProc.GetCheck(i))
			break;
	}
	m_OneKeyKill.EnableWindow(i!=n);
	m_LstProc.SetFocus();
}

//“黑名单”菜单
void CSuperTaskmgrDlg::OnSelBlack()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetItemState(i,pListPro[i]->inBlack?LVIS_SELECTED:0,LVIS_SELECTED);
		pListPro[i]->Selected=pListPro[i]->inBlack;
	}
	for (i=0;i<n;i++){
		if (m_LstProc.GetCheck(i))
			break;
	}
	m_OneKeyKill.EnableWindow(i!=n);
}

//“黑名单”按钮
void CSuperTaskmgrDlg::OnBnClickedSelBlack()
{
	int i,n;
	n=m_LstProc.GetItemCount();
	for (i=0;i<n;i++){
		m_LstProc.SetCheck(
			i,
			pListPro[i]->inBlack
		);
	}
	for (i=0;i<n;i++){
		if (m_LstProc.GetCheck(i))
			break;
	}
	m_OneKeyKill.EnableWindow(i!=n);
	m_LstProc.SetFocus();
}

//“前端显示”菜单
void CSuperTaskmgrDlg::OnShowInFront()
{
	setting.ShowInFront=!setting.ShowInFront;
	SetWindowPos(
		setting.ShowInFront ? &CWnd::wndTopMost : &CWnd::wndNoTopMost
		,0,0,0,0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER
	);
}
void CSuperTaskmgrDlg::OnUpdateShowInFront(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(setting.ShowInFront);
}

//“最小化时隐藏”菜单
void CSuperTaskmgrDlg::OnMinHide()
{
	setting.HideWhenMinimize=!setting.HideWhenMinimize;
}
void CSuperTaskmgrDlg::OnUpdateMinHide(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(setting.HideWhenMinimize);
}


//“显示悬浮窗”菜单
void CSuperTaskmgrDlg::OnShowFloatingwindow()
{
	setting.ShowFloatingWindow=!setting.ShowFloatingWindow;
	m_FloatingWindow.Setup(setting);
	//m_FloatingWindow.ShowWindow(setting.ShowFloatingWindow ? SW_NORMAL : SW_HIDE);
}
void CSuperTaskmgrDlg::OnUpdateShowFloatingwindow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(setting.ShowFloatingWindow);
}


//“显示系统关键进程”菜单
void CSuperTaskmgrDlg::OnShowSystem()
{
	setting.ShowSystemProcess=!setting.ShowSystemProcess;
	RefreshList();
}
void CSuperTaskmgrDlg::OnUpdateShowSystem(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(setting.ShowSystemProcess);
}

//“恢复默认设置”菜单
void CSuperTaskmgrDlg::OnLoadDefault()
{
	bReadCFGFile=TRUE;
	bReadWhiteList=TRUE;
	bReadBlackList=TRUE;
	bReadSystemList=TRUE;
	LoadCFG(TRUE);
	ApplyCFG();
	Refresh();
}

//“显示”菜单
void CSuperTaskmgrDlg::OnShow(){
	ShowWindow(SW_SHOW);
	VERIFY(OpenIcon());
	VERIFY(SetForegroundWindow());
	m_LstProc.SetFocus();
}

//等待记事本编辑的线程函数
DWORD WINAPI CSuperTaskmgrDlg::WaitEditList(LPVOID lpParam){
	DWORD result=0;
	CProcess &Process=*(CProcess*)lpParam;

	if (WaitForSingleObject(Process,-1)!=WAIT_OBJECT_0){
		result=-1;
		goto end;
	}

	pcThis->bReadWhiteList=TRUE;
	pcThis->bReadBlackList=TRUE;

	if (!pcThis->LoadCFG(FALSE))
		result=-2;

end:
	Process.Close();
	return result;
}

void CSuperTaskmgrDlg::NotepadEditListFile(LPTSTR szListFile,BOOL *lpbListSign){
	if (NotepadProcess){
		ASSERT(0);
		return ;
	}

	TCHAR path[MAX_PATH];
	_stprintf_s(
		path,
		MAX_PATH,
		_T("%s\\%s \"%s\""),
		lpszSystemRoot,
		_T("notepad.exe"),
		szListFile
	);

	if (!NotepadProcess.Create(NULL,path)){
		ShowError(m_hWnd,_T("无法创建进程"),_T("打开记事本时出错！\n"),_T(""),GetLastError());
	}

	if (WaitNotepadThread) WaitNotepadThread.Terminate();
	WaitNotepadThread.Create(&NotepadProcess,WaitEditList);
}
void CSuperTaskmgrDlg::OnUpdateNotepadEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NotepadProcess==NULL);
}


//“白名单进程设置”菜单
void CSuperTaskmgrDlg::OnEditWhite()
{
	NotepadEditListFile(sWhiteListFile,&bReadWhiteList);
}

//“黑名单进程设置”菜单
void CSuperTaskmgrDlg::OnEditBlack()
{
	NotepadEditListFile(sBlackListFile,&bReadBlackList);
}

//“关于”菜单
void CSuperTaskmgrDlg::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



