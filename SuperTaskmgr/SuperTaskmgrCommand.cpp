
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrCommand.h"
#include <Psapi.h>

C360TdiFilter CSuperTaskmgrDlg::TdiFilter;
extern DWORD (WINAPI *SHRunDialog)(HWND,DWORD,LPVOID,LPVOID,LPVOID,DWORD);


//��ˢ�¡�����
void CSuperTaskmgrDlg::OnRefresh(){
	Refresh(TRUE);
}

//�½����񣬡����С��Ի���
void CSuperTaskmgrDlg::OnNewTask()
{
	if (stOsVerInfo.dwPlatformId==VER_PLATFORM_WIN32_NT)
		SHRunDialog(
		m_hWnd,
		0xF3C061F,
		L"C:\\WINDOWS",
		L"����������",
		L"Windows ������������������ƣ�Ϊ�����Ӧ�ĳ����ļ��С��ĵ��� Internet ��Դ��",
		0x54
		);
	else
		SHRunDialog(
		m_hWnd,
		0xF3C061F,
		L"C:\\WINDOWS",
		"����������","Windows ������������������ƣ�Ϊ�����Ӧ�ĳ����ļ��С��ĵ��� Internet ��Դ��",
		0x54
		);
}

//���������̡��˵�
void CSuperTaskmgrDlg::OnKill()
{
	DWORD dwLastError;
	int i;
	BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
		if (!KillProcess(pListPro[i]->PID)){
			dwLastError=GetLastError();
			if (m_LstProc.GetSelectedCount()==1)
				ShowError(m_hWnd,_T("�޷���ֹ����"),_T("��������ʧ�ܣ�\n"),_T(""),dwLastError);
		}
	END_ENUM_SELECTED_ITEM
	Sleep(32);
	Refresh();
}
void CSuperTaskmgrDlg::OnUpdateKill(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//��ǿ�ƽ������̡��˵�
void CSuperTaskmgrDlg::OnUpdateForceKill(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

//��һ���������̡���ť
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

//����ʽ�Ӳ˵��պ���
void CSuperTaskmgrDlg::OnMenuItemNull(){}
void CSuperTaskmgrDlg::OnUpdateMenuItemNull(CCmdUI* pCmdUI){}

//�����ȼ����˵�
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
		if (nID>=40500 && nID<40509){ //���ȼ��˵�
			if (nCode==0){ //On Command
				pn=nID-40500; //���ȼ����
				BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
					CProcess Process;
					if (Process.Open(pListPro[i]->PID,PROCESS_SET_INFORMATION))
						Process.SetPriorityClass(PriorityClass[pn]);//�������ȼ�
				END_ENUM_SELECTED_ITEM

				Sleep(32);
				Refresh();
			} else if (nCode==-1 && nID==40501){ //On Update
				//��ȡ��ѡ�еĽ������ȼ�
				BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
					ckms[PriorityIndex(pListPro[i]->Priority)]=TRUE;
				END_ENUM_SELECTED_ITEM

				//��ѡ��Ӧ�Ĳ˵�
				menu=m_Menu.GetSubMenu(1)->GetSubMenu(2);
				for (i=1;i<=6;i++)
					menu->CheckMenuItem(6-i,MF_BYPOSITION | (ckms[i]?MF_CHECKED:0));
			}
		} else if (nID>=41300 && nID<41309){ //ˢ��Ƶ�ʲ˵�
			if (nCode==0){ //On Command
				setting.RefreshSpeed=nID-41300;
				SetTimer(1,TimerInterval[setting.RefreshSpeed],NULL);
			} else if (nCode==-1 && nID==41300){ //On Update
				ckms[setting.RefreshSpeed]=TRUE;

				//��ѡ��Ӧ�Ĳ˵�
				menu=m_Menu.GetSubMenu(3)->GetSubMenu(5);
				for (i=0;i<=5;i++)
					menu->CheckMenuItem(5-i,MF_BYPOSITION | (ckms[i]?MF_CHECKED:0));
			}
		}
	}
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//�����ר�ù��������˵�
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


//����ͣ�������С��˵�
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
		ShowError(m_hWnd,_T("�޷��������"),_T("�������ʧ�ܣ�\n"),_T(""),dwLastError);
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

//���ָ��������С��˵�
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
		ShowError(m_hWnd,_T("�޷��ָ�����"),_T("�ָ�����ʧ�ܣ�\n"),_T(""),dwLastError);
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


//���������ơ��˵�
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


//����ֹ�������硱�˵�
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

//������������硱�˵�
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

//���������Ĳ���
void CSuperTaskmgrDlg::ListMenuCommand(BOOL &bSelAllVar,BOOL bChangeSelected,BOOL bInListSelect,LPTSTR *List,int &ListN,INT_PTR iProStOffset){
	int i,j;
	if (bSelAllVar){
		//���������Ƴ�
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
		//��������
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

//�����ú��������˵�
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
	pCmdUI->SetText(bSelAllBlack?_T("�Ӻ��������Ƴ�(&B)"):_T("���������(&B)"));
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//�����ð��������˵�
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
	pCmdUI->SetText(bSelAllWhite?_T("�Ӱ��������Ƴ�(&W)"):_T("���������(&W)"));
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//������ϵͳ���̡��˵�
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
	pCmdUI->SetText(bSelAllSystem?_T("ȡ��ϵͳ����(&S)"):_T("��Ϊϵͳ����(&S)"));
	pCmdUI->Enable(m_LstProc.GetSelectedCount()!=0);
}

//���鿴�ļ����ԡ��˵�
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

//��������Ŀ¼���˵�
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

//��ȫѡ���˵�
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

//��ȫѡ����ť
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

//����ѡ���˵�
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

//����ѡ����ť
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

//���ǰ��������˵�
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

//���ǰ���������ť
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

//�����������˵�
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

//������������ť
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

//��ǰ����ʾ���˵�
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

//����С��ʱ���ء��˵�
void CSuperTaskmgrDlg::OnMinHide()
{
	setting.HideWhenMinimize=!setting.HideWhenMinimize;
}
void CSuperTaskmgrDlg::OnUpdateMinHide(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(setting.HideWhenMinimize);
}


//����ʾ���������˵�
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


//����ʾϵͳ�ؼ����̡��˵�
void CSuperTaskmgrDlg::OnShowSystem()
{
	setting.ShowSystemProcess=!setting.ShowSystemProcess;
	RefreshList();
}
void CSuperTaskmgrDlg::OnUpdateShowSystem(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(setting.ShowSystemProcess);
}

//���ָ�Ĭ�����á��˵�
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

//����ʾ���˵�
void CSuperTaskmgrDlg::OnShow(){
	ShowWindow(SW_SHOW);
	VERIFY(OpenIcon());
	VERIFY(SetForegroundWindow());
	m_LstProc.SetFocus();
}

//�ȴ����±��༭���̺߳���
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
		ShowError(m_hWnd,_T("�޷���������"),_T("�򿪼��±�ʱ����\n"),_T(""),GetLastError());
	}

	if (WaitNotepadThread) WaitNotepadThread.Terminate();
	WaitNotepadThread.Create(&NotepadProcess,WaitEditList);
}
void CSuperTaskmgrDlg::OnUpdateNotepadEdit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(NotepadProcess==NULL);
}


//���������������á��˵�
void CSuperTaskmgrDlg::OnEditWhite()
{
	NotepadEditListFile(sWhiteListFile,&bReadWhiteList);
}

//���������������á��˵�
void CSuperTaskmgrDlg::OnEditBlack()
{
	NotepadEditListFile(sBlackListFile,&bReadBlackList);
}

//�����ڡ��˵�
void CSuperTaskmgrDlg::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



