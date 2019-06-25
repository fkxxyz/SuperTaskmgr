
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrConfig.h"

#ifdef _WIN64
#define CmpSizeT CmpQword
#else
#define CmpSizeT CmpDwordInv
#endif

const COLUMN_ITEM_STRUCT CSuperTaskmgrDlg::ColumnItemName[cENUM_COLUMN_END]={
	cProcessName,_T("进程名称"),136,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->szDispName,(INT_PTR)&((PROCESS*)0)->name,
		0,&CmpStr,FALSE,
	cDescription,_T("描述"),146,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->FileDescription,(INT_PTR)&((PROCESS*)0)->FileDescription,
		0,&CmpStr,FALSE,
	cImagePath,_T("文件路径"),192,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->path,(INT_PTR)&((PROCESS*)0)->path,
		0,&CmpStr,FALSE,
	cCommandLine,_T("命令行"),192,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->cmdline,(INT_PTR)&((PROCESS*)0)->cmdline,
		0,&CmpStr,FALSE,
	cCPU,_T("CPU"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->CPU,
		&ConvCPUStr,&CmpFloat,TRUE,
	cIOSpeed,_T("IO读写"),90,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->IOSpeed,
		&ConvSpeedStr,&CmpQword,TRUE,
	cPrivateWorkingSet,_T("专用工作集"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PrivateWorkingSet,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cNetRecvSpeed,_T("下载速度"),84,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetRecvSpeed,
		&ConvNetSpeedStr,&CmpQword,TRUE,
	cNetSendSpeed,_T("上传速度"),84,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetSendSpeed,
		&ConvNetSpeedStr,&CmpQword,TRUE,
	cNetTotalSpeed,_T("网速占用"),84,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetTotalSpeed,
		&ConvNetSpeedStr,&CmpQword,TRUE,
	cNetRecvFlow,_T("下载流量"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetRecvFlow,
		&ConvMEMMBStr,&CmpQword,TRUE,
	cNetSendFlow,_T("上传流量"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetSendFlow,
		&ConvMEMMBStr,&CmpQword,TRUE,
	cNetTotalFlow,_T("已用流量"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetTotalFlow,
		&ConvMEMMBStr,&CmpQword,TRUE,
	cUserName,_T("用户名"),110,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->szUserName,(INT_PTR)&((PROCESS*)0)->szUserName,
		0,&CmpStr,FALSE,
	cIsWow64Process,_T("平台"),36,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szPlatform,(INT_PTR)&((PROCESS*)0)->isWow64Process,
		0,&CmpDwordPos,FALSE,
	cSuspended,_T("状态"),36,LVCFMT_LEFT,
		0,(INT_PTR)&((PROCESS*)0)->isSuspended,
		&ConvSuspendStr,&CmpDwordPos,TRUE,
	cNumberOfThreads,_T("线程数量"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NumberOfThreads,
		&ConvULONGStr,&CmpDwordInv,TRUE,
	cCreateTime,_T("运行时间"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->CreateTime,
		&ConvTIMEStr,&CmpQword,TRUE,
	cCPUTime,_T("CPU 时间"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->CPUTime,
		&ConvTIMEStr,&CmpQword,TRUE,
	cPriority,_T("优先权限"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->Priority,
		&ConvPriorityStr,&CmpPriority,TRUE,
	cPID,_T("PID"),54,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szPID,(INT_PTR)&((PROCESS*)0)->PID,
		0,&CmpDwordPos,FALSE,
	cInheritedFromProcessId,_T("父PID"),54,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szParentID,(INT_PTR)&((PROCESS*)0)->pPID,
		0,&CmpDwordPos,FALSE,
	cHandleCount,_T("句柄数量"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->HandleCount,
		&ConvULONGStr,&CmpDwordInv,TRUE,
	cSessionId,_T("会话ID"),54,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szSessionId,(INT_PTR)&((PROCESS*)0)->SessionId,
		0,&CmpDwordPos,FALSE,
	cPeakVirtualSize,_T("虚拟内存峰值"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PeakVirtualSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cVirtualSize,_T("虚拟内存"),108,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->VirtualSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cPageFaultCount,_T("页面错误"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PageFaultCount,
		&ConvULONGStr,&CmpDwordInv,TRUE,
	cPeakWorkingSetSize,_T("工作集峰值"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PeakWorkingSetSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cWorkingSetSize,_T("工作集"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->WorkingSetSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cQuotaPeakPagedPoolUsage,_T("页面缓冲池峰值"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaPeakPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cQuotaPagedPoolUsage,_T("页面缓冲池"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cQuotaPeakNonPagedPoolUsage,_T("非页面缓冲池峰值"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaPeakNonPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cQuotaNonPagedPoolUsage,_T("非页面缓冲池"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaNonPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cPageFileUsage,_T("页面文件"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PageFileUsage,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cPeakPageFileUsage,_T("页面文件峰值"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PeakPageFileUsage,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cPrivateUsage,_T("提交大小"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PrivateUsage,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cReadOperationCount,_T("I/O读取"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->ReadOperationCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cWriteOperationCount,_T("I/O写入"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->WriteOperationCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cOtherOperationCount,_T("I/O其他"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->OtherOperationCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cReadTransferCount,_T("I/O读取字节"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->ReadTransferCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cWriteTransferCount,_T("I/O写入字节"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->WriteTransferCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cOtherTransferCount,_T("I/O其他字节"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->OtherTransferCount,
		&ConvULONGStr,&CmpQword,TRUE
};

SETTING CSuperTaskmgrDlg::setting;

const SETTING CSuperTaskmgrDlg::DefaultSetting = {
	480,24,720,640,3,FALSE,TRUE,FALSE,FALSE,
	1.0l,0.9l,3,192,
	7,
	cProcessName,136,
	cDescription,146,
	cCPU,54,
	cNetRecvSpeed,84,
	cNetSendSpeed,84,
	cIOSpeed,84,
	cPrivateWorkingSet,75
};


LPTSTR CSuperTaskmgrDlg::HeadString_White =
	_T("#白名单进程（不区分大小写）：\r\n")
	_T("#修改完成保存后重启“超级任务管理器”生效。\r\n")
;
LPTSTR CSuperTaskmgrDlg::HeadString_Black =
	_T("#黑名单进程（不区分大小写）：\r\n")
	_T("#修改完成保存后重启“超级任务管理器”生效。\r\n")
;
LPTSTR CSuperTaskmgrDlg::HeadString_System =
	_T("#系统关键进程（不区分大小写）：\r\n")
	_T("#修改完成保存后重启“超级任务管理器”生效。\r\n")
;

LPTSTR CSuperTaskmgrDlg::DefaultSetting_WhiteListFile =
	_T("explorer.exe\r\n")
	_T("QQ.exe\r\n")
	_T("QQProtect.exe\r\n")
;
LPTSTR CSuperTaskmgrDlg::DefaultSetting_BlackListFile =
	_T("\r\n")
;
LPTSTR CSuperTaskmgrDlg::DefaultSetting_AllSystemProcessFile =
// Win10
	_T("sihost.exe\r\n")
	_T("shellexperiencehost.exe\r\n")
	_T("searchui.exe\r\n")
	_T("fontdrvhost.exe\r\n")
	_T("chsime.exe\r\n")
	_T("taskhostw.exe\r\n")

// Win8
	_T("wudfhost.exe\r\n")
	_T("dasHost.exe\r\n")
	_T("settingsynchost.exe\r\n")
	_T("runtimebroker.exe\r\n")
	_T("LogonUI.exe\r\n")

// Win7
	_T("dwm.exe\r\n")
	_T("conhost.exe\r\n")
	_T("lsm.exe\r\n")
	_T("wininit.exe\r\n")
	_T("sppsvc.exe\r\n")

// XP
	_T("system\r\n")
	_T("smss.exe\r\n")
	_T("csrss.exe\r\n")
	_T("winlogon.exe\r\n")
	_T("services.exe\r\n")
	_T("lsass.exe\r\n")
	_T("svchost.exe\r\n")
	_T("ctfmon.exe\r\n")
	_T("alg.exe\r\n")
	_T("dllhost.exe\r\n")
	_T("spoolsv.exe\r\n")
	_T("wmiprvse.exe\r\n")
	_T("系统空闲\r\n")
;

//刷新时间间隔常量
const int CSuperTaskmgrDlg::TimerInterval[6]={INT_MAX,4000,2000,1000,500,250};


void CSuperTaskmgrDlg::CopySetting(SETTING &Dest,const SETTING &Src){
	int i;

	Dest.left=Src.left;
	Dest.top=Src.top;
	Dest.width=Src.width;
	Dest.height=Src.height;
	Dest.RefreshSpeed=Src.RefreshSpeed;
	Dest.ShowInFront=Src.ShowInFront;
	Dest.HideWhenMinimize=Src.HideWhenMinimize;
	Dest.ShowFloatingWindow=Src.ShowFloatingWindow;
	Dest.ShowSystemProcess=Src.ShowSystemProcess;
	Dest.dwFloatWindowX=Src.dwFloatWindowX;
	Dest.dwFloatWindowY=Src.dwFloatWindowY;
	Dest.iCloseToBorder=Src.iCloseToBorder;
	Dest.bAlpha=Src.bAlpha;

	Dest.ColumnNumber=Src.ColumnNumber;
	for (i=0;i<Dest.ColumnNumber;i++){
		Dest.column[i].cindex=Src.column[i].cindex;
		Dest.column[i].width=Src.column[i].width;
	}

	Dest.nWhiteList=0;
	Dest.nBlackList=0;
	Dest.nSystemProcess=0;
	SPNameContainer.Reset();
}


//从列表文件sFileName中读取数据，保存到Dest
BOOL CSuperTaskmgrDlg::ReadListFile(LPCTSTR sFileName,LPTSTR *Dest,int &DestN,LPCTSTR szNoFileDefaultHeadString=NULL,LPCTSTR szNoFileDefaultString=NULL){
	LPTSTR pc1,pc2;
	TCHAR szBuffer[MAX_PATH];
	if (!ExistFile(sFileName)){
		//如果不存在该列表文件则写入一个文件
		CAnsiTextFile file;
		if (!file.OpenWrite(sFileName)) return FALSE;
		if (szNoFileDefaultHeadString)
			if (!file.WriteText(szNoFileDefaultHeadString)) return FALSE;
		if (szNoFileDefaultString)
			if (!file.WriteText(szNoFileDefaultString)) return FALSE;
		if (!file.Close()) return FALSE;
	}
	if (!ExistFile(sFileName)){
		ASSERT(0);
		return FALSE;
	}

	DestN=0;
	CAnsiTextFile file;
	if (!file.OpenRead(sFileName)) return FALSE;
	while(file.ReadLine(szBuffer,MAX_PATH)){//遍历每一行
		for (pc1=szBuffer;*pc1!=_T('\0') && *pc1==_T(' ');pc1++);//忽略头部所有的空格
		if (*pc1==_T('\0') || *pc1==_T('#')) continue;//判断是否是有效行
		for (pc2=pc1;*pc2!=_T('\0');pc2++);//寻找尾部
		for (pc2--;*pc2==_T(' ')||*pc2==_T('\r')||*pc2==_T('\n');pc2--);//忽略尾部所有的空格和\r\n
		*(++pc2)=_T('\0');
		_tcslwr_s(pc1,_tcslen(pc1)+1);//转换成小写

		//添加此数据
		Dest[DestN]=SPNameContainer.NewData(pc1,pc2-pc1+1);
		(DestN)++;
	}
	if (!file.Close()) return FALSE;
	return TRUE;
}

//将列表Src写入文件sFileName中
BOOL CSuperTaskmgrDlg::WriteListFile(LPCTSTR sFileName,LPTSTR *Src,int &SrcN,LPCTSTR szHeadString=NULL){
	int i;
	CAnsiTextFile file;
	if (!file.OpenWrite(sFileName)) return FALSE;
		if (szHeadString){
			if (!file.WriteText(szHeadString)) return FALSE;
		}
		for (i=0;i<SrcN;i++){
			if (!file.WriteText(Src[i])) return FALSE;
			if (!file.WriteText(_T("\r\n"))) return FALSE;
		}
	if (!file.Close()) return FALSE;
	return TRUE;
}

//从设置文件sFileName中读取数据，保存到Dest
BOOL CSuperTaskmgrDlg::ReadCFGFile(LPCTSTR sFileName,SETTING &Dest,CONST SETTING &DefaultSetting){
	LPTSTR pc1,pc2,pc3,pc4;
	TCHAR szBuffer[MAX_PATH];
	CopySetting(Dest,DefaultSetting);
	if (!ExistFile(sFileName))
		return TRUE;
	else
		Dest.ColumnNumber=0;

	CAnsiTextFile file;
	if (!file.OpenRead(sFileName)) return FALSE;
	while(file.ReadLine(szBuffer,MAX_PATH)){
		for (pc1=szBuffer;*pc1!=_T('\0') && *pc1==_T(' ');pc1++);//忽略头部所有的空格
		if (*pc1==_T('\0') || *pc1==_T('#')) continue;//判断是否是有效行
		pc3=pc2=_tcschr(pc1,_T('='));//寻找等号
		if (!pc2 || pc1==pc2) continue;//如果无等号或等号在开头则忽略改行

		//获取等号前的数据为pc1
		for (pc2--;*pc2==_T(' ')&&pc2>pc1;pc2--);//忽略等号前所有的空格
		*(++pc2)=_T('\0');
		if (pc1==pc2) continue;
		_tcslwr_s(pc1,_tcslen(pc1)+1);//转换成小写

		//获取等号前的数据为pc3
		for (pc3++;*pc3!=_T('\0') && *pc3==_T(' ');pc3++);//忽略等号后所有的空格
		if (*pc3==_T('\r')||*pc3==_T('\n')||*pc3==_T('\0')) continue;
		for (pc4=pc3;*pc4!=_T('\0');pc4++);//寻找尾部
		for (pc4--;*pc4==_T(' ')||*pc4==_T('\r')||*pc4==_T('\n')&&pc4>pc3;pc4--);//忽略尾部所有的空格和\n
		*(++pc4)=_T('\0');

		if (_tcseqr(pc1,_T("formleft")))
			setting.left=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("formtop")))
			setting.top=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("formwidth")))
			setting.width=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("formheight")))
			setting.height=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("refreshspeed")))
			setting.RefreshSpeed=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("formshowinfront")))
			setting.ShowInFront=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("hidewhenminimize")))
			setting.HideWhenMinimize=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("showfloatingwindow")))
			setting.ShowFloatingWindow=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("showsystemprocess")))
			setting.ShowSystemProcess=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("dwfloatwindowx"))){
			register __int64 i64=_tcstoi64(pc3,&pc4,16);
			setting.dwFloatWindowX=*(double*)&i64;
		}else if (_tcseqr(pc1,_T("dwfloatwindowy"))){
			register __int64 i64=_tcstoi64(pc3,&pc4,16);
			setting.dwFloatWindowY=*(double*)&i64;
		}else if (_tcseqr(pc1,_T("iclosetoborder")))
			setting.iCloseToBorder=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("balpha")))
			setting.bAlpha=_tcstoi(pc3);
		else if (_tcseqr(pc1,_T("column"))){
			pc2=_tcschr(pc3,_T(','));

			//获取逗号前的数据为pc3
			for (pc4=pc2-1;*pc4==_T(' ');pc4--)
				if (pc3>pc4) goto lcontinue;
			*(pc4+1)=_T('\0');

			//获取逗号后的数据为pc2
			for (pc2++;*pc2==_T(' ');pc2++)
				if (*pc2==_T('\0')) goto lcontinue;

			//设置数据
			setting.column[setting.ColumnNumber].cindex=(ENUM_COLUMN)_tcstoi(pc3);
			setting.column[setting.ColumnNumber].width=_tcstoi(pc2);
			setting.ColumnNumber++;
		}
lcontinue:
		;
	}
	if (!file.Close()) return FALSE;
	return TRUE;
}

//将设置写入文件sFileName中
BOOL CSuperTaskmgrDlg::WriteCFGFile(LPCTSTR sFileName,const SETTING &Src,const int *ColumnOrd){
	int i;
	TCHAR szBuffer[32767];
	CAnsiTextFile file;
	if (!file.OpenWrite(sFileName)) return FALSE;
	_stprintf_s<32767>(
		szBuffer,
		_T("FormLeft = %d\r\n")
		_T("FormTop = %d\r\n")
		_T("FormWidth = %d\r\n")
		_T("FormHeight = %d\r\n")
		_T("RefreshSpeed = %d\r\n")
		_T("FormShowInFront = %d\r\n")
		_T("HideWhenMinimize = %d\r\n")
		_T("ShowFloatingWindow = %d\r\n")
		_T("ShowSystemProcess = %d\r\n")
		_T("dwFloatWindowX = %I64x\r\n")
		_T("dwFloatWindowY = %I64x\r\n")
		_T("iCloseToBorder = %d\r\n")
		_T("bAlpha = %d\r\n"),
		Src.left,
		Src.top,
		Src.width,
		Src.height,
		Src.RefreshSpeed,
		Src.ShowInFront,
		Src.HideWhenMinimize,
		Src.ShowFloatingWindow,
		Src.ShowSystemProcess,
		*(__int64*)&Src.dwFloatWindowX,
		*(__int64*)&Src.dwFloatWindowY,
		Src.iCloseToBorder,
		Src.bAlpha
	);
	if (!file.WriteText(szBuffer)) return FALSE;
	for (i=0;i<Src.ColumnNumber;i++){
		_stprintf_s(szBuffer,32,_T("Column = %d,%d\r\n"),Src.column[ColumnOrd[i]].cindex,Src.column[ColumnOrd[i]].width);
		if (!file.WriteText(szBuffer)) return FALSE;
	}
	if (!file.Close()) return FALSE;
	return TRUE;
}

//读取设置
BOOL CSuperTaskmgrDlg::LoadCFG(BOOL lDefault=FALSE){
	BOOL ret;
	int i;
	ret=TRUE;

	if (bReadCFGFile){
		if (lDefault){
			CopySetting(setting,DefaultSetting);
			DeleteFile(sWhiteListFile);
			DeleteFile(sBlackListFile);
			DeleteFile(sSystemProcessFile);
		} else
			if (!ReadCFGFile(sCFGFile,setting,DefaultSetting))
				ret=FALSE;
		bReadCFGFile=FALSE;
	}

	if (bReadWhiteList && bReadBlackList && bReadSystemList)
		SPNameContainer.Reset();

	//读取白名单进程设置
	if (bReadWhiteList){
		if (!ReadListFile(sWhiteListFile,setting.WhiteList,setting.nWhiteList,HeadString_White,DefaultSetting_WhiteListFile))
			ret=FALSE;
		bReadWhiteList=FALSE;
		for (i=0;i<proN;i++)
			pro[i]->inWhite=InList(setting.WhiteList,setting.nWhiteList,pro[i]->name);
	}

	//读取黑名单进程设置
	if (bReadBlackList){
		if (!ReadListFile(sBlackListFile,setting.BlackList,setting.nBlackList,HeadString_Black,DefaultSetting_BlackListFile))
			ret=FALSE;
		bReadBlackList=FALSE;
		for (i=0;i<proN;i++)
			pro[i]->inBlack=InList(setting.BlackList,setting.nBlackList,pro[i]->name);
	}

	//读取系统进程设置
	if (bReadSystemList){
		if (!ReadListFile(sSystemProcessFile,setting.SystemProcess,setting.nSystemProcess,HeadString_System,DefaultSetting_SystemProcessFile))
			ret=FALSE;
		bReadSystemList=FALSE;
		for (i=0;i<proN;i++)
			pro[i]->inSystem=InList(setting.SystemProcess,setting.nSystemProcess,pro[i]->name);
	}

	return ret;
}

//保存设置
BOOL CSuperTaskmgrDlg::SaveCFG(){
	if (!WriteCFGFile(sCFGFile,setting,ColumnOrd)) return FALSE;
	if (!WriteListFile(sWhiteListFile,setting.WhiteList,setting.nWhiteList,HeadString_White)) return FALSE;
	if (!WriteListFile(sBlackListFile,setting.BlackList,setting.nBlackList,HeadString_Black)) return FALSE;
	if (!WriteListFile(sSystemProcessFile,setting.SystemProcess,setting.nSystemProcess,HeadString_System)) return FALSE;
	return TRUE;
}

//刷新列表头的设置
void CSuperTaskmgrDlg::RefreshColumnCFG(){
	int i,n;
	CHeaderCtrl *header=m_LstProc.GetHeaderCtrl();
	n=header->GetItemCount();
	for (i=0;i<n;i++){
		ColumnOrd[i]=header->OrderToIndex(i);
		setting.column[i].width=m_LstProc.GetColumnWidth(i);
	}
}

//初始化列表控件
void CSuperTaskmgrDlg::InitListCtrl(){
	int i;
	LVCOLUMN stColumn;
	//							整行选取			网格线				复选框			表头可拖动
	m_LstProc.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP);
	m_LstProc.SetRedraw(FALSE);

	//保持列表头中的项数一致
	VERIFY(m_LstProc.SetColumnCount(setting.ColumnNumber));

	//设置列表头中的每一项的数据
	stColumn.mask=LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;
	for (i=0;i<setting.ColumnNumber;i++){
		VERIFY(m_LstProc.SetColumn(
			i,
			ColumnItemName[setting.column[i].cindex].name,
			ColumnItemName[setting.column[i].cindex].LvcFormat,
			setting.column[i].width
			));
	}
	m_LstProc.SetRedraw(TRUE);
}

//应用设置
BOOL CSuperTaskmgrDlg::ApplyCFG(){

	//检查窗体位置的合理性
	RECT stWorkArea;
	VERIFY(SystemParametersInfo(SPI_GETWORKAREA,0,&stWorkArea,0));
	SIZE stScreenSize={
		stWorkArea.right-stWorkArea.left,
		stWorkArea.bottom-stWorkArea.top
	};
	if (setting.width<MIN_SETTING_WIDTH)
		setting.width=MIN_SETTING_WIDTH;
	if (setting.width>stScreenSize.cx)
		setting.width=stScreenSize.cx;
	if (setting.height<MIN_SETTING_HEIGHT)
		setting.height=MIN_SETTING_HEIGHT;
	if (setting.height>stScreenSize.cy)
		setting.height=stScreenSize.cy;
	if (setting.left<stWorkArea.left)
		setting.left=stWorkArea.left;
	if (setting.left+setting.width>stWorkArea.right)
		setting.left=stWorkArea.right-setting.width;
	if (setting.top<stWorkArea.top)
		setting.top=stWorkArea.top;
	if (setting.top+setting.height>stWorkArea.bottom)
		setting.top=stWorkArea.bottom-setting.height;

	//初始化窗体位置
	MoveWindow(setting.left,setting.top,setting.width,setting.height);

	//初始化列表控件
	InitListCtrl();

	//初始化图标列表
	if (!m_ImageList.m_hImageList){
		VERIFY(m_ImageList.Create(16, 16, ILC_COLOR16, 8, 8));
		m_ImageList.SetBkColor(RGB(255,255,255));
		m_ImageList.Add(hIconNull);
	}
	m_LstProc.SetImageList(&m_ImageList,LVSIL_SMALL);

	//设置刷新频率
	SetTimer(1,TimerInterval[setting.RefreshSpeed],NULL);

	//设置窗体置顶
	VERIFY(SetWindowPos(setting.ShowInFront ? &CWnd::wndTopMost : &CWnd::wndNoTopMost,0,0,0,0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER));

	//显示悬浮窗
	if (!m_FloatingWindow.m_hWnd)
		VERIFY(m_FloatingWindow.Initialize());

	m_FloatingWindow.Setup(setting);
	return TRUE;
}



