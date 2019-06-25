
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
	cProcessName,_T("��������"),136,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->szDispName,(INT_PTR)&((PROCESS*)0)->name,
		0,&CmpStr,FALSE,
	cDescription,_T("����"),146,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->FileDescription,(INT_PTR)&((PROCESS*)0)->FileDescription,
		0,&CmpStr,FALSE,
	cImagePath,_T("�ļ�·��"),192,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->path,(INT_PTR)&((PROCESS*)0)->path,
		0,&CmpStr,FALSE,
	cCommandLine,_T("������"),192,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->cmdline,(INT_PTR)&((PROCESS*)0)->cmdline,
		0,&CmpStr,FALSE,
	cCPU,_T("CPU"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->CPU,
		&ConvCPUStr,&CmpFloat,TRUE,
	cIOSpeed,_T("IO��д"),90,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->IOSpeed,
		&ConvSpeedStr,&CmpQword,TRUE,
	cPrivateWorkingSet,_T("ר�ù�����"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PrivateWorkingSet,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cNetRecvSpeed,_T("�����ٶ�"),84,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetRecvSpeed,
		&ConvNetSpeedStr,&CmpQword,TRUE,
	cNetSendSpeed,_T("�ϴ��ٶ�"),84,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetSendSpeed,
		&ConvNetSpeedStr,&CmpQword,TRUE,
	cNetTotalSpeed,_T("����ռ��"),84,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetTotalSpeed,
		&ConvNetSpeedStr,&CmpQword,TRUE,
	cNetRecvFlow,_T("��������"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetRecvFlow,
		&ConvMEMMBStr,&CmpQword,TRUE,
	cNetSendFlow,_T("�ϴ�����"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetSendFlow,
		&ConvMEMMBStr,&CmpQword,TRUE,
	cNetTotalFlow,_T("��������"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NetTotalFlow,
		&ConvMEMMBStr,&CmpQword,TRUE,
	cUserName,_T("�û���"),110,LVCFMT_LEFT,
		(INT_PTR)&((PROCESS*)0)->szUserName,(INT_PTR)&((PROCESS*)0)->szUserName,
		0,&CmpStr,FALSE,
	cIsWow64Process,_T("ƽ̨"),36,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szPlatform,(INT_PTR)&((PROCESS*)0)->isWow64Process,
		0,&CmpDwordPos,FALSE,
	cSuspended,_T("״̬"),36,LVCFMT_LEFT,
		0,(INT_PTR)&((PROCESS*)0)->isSuspended,
		&ConvSuspendStr,&CmpDwordPos,TRUE,
	cNumberOfThreads,_T("�߳�����"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->NumberOfThreads,
		&ConvULONGStr,&CmpDwordInv,TRUE,
	cCreateTime,_T("����ʱ��"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->CreateTime,
		&ConvTIMEStr,&CmpQword,TRUE,
	cCPUTime,_T("CPU ʱ��"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->CPUTime,
		&ConvTIMEStr,&CmpQword,TRUE,
	cPriority,_T("����Ȩ��"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->Priority,
		&ConvPriorityStr,&CmpPriority,TRUE,
	cPID,_T("PID"),54,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szPID,(INT_PTR)&((PROCESS*)0)->PID,
		0,&CmpDwordPos,FALSE,
	cInheritedFromProcessId,_T("��PID"),54,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szParentID,(INT_PTR)&((PROCESS*)0)->pPID,
		0,&CmpDwordPos,FALSE,
	cHandleCount,_T("�������"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->HandleCount,
		&ConvULONGStr,&CmpDwordInv,TRUE,
	cSessionId,_T("�ỰID"),54,LVCFMT_RIGHT,
		(INT_PTR)&((PROCESS*)0)->szSessionId,(INT_PTR)&((PROCESS*)0)->SessionId,
		0,&CmpDwordPos,FALSE,
	cPeakVirtualSize,_T("�����ڴ��ֵ"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PeakVirtualSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cVirtualSize,_T("�����ڴ�"),108,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->VirtualSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cPageFaultCount,_T("ҳ�����"),54,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PageFaultCount,
		&ConvULONGStr,&CmpDwordInv,TRUE,
	cPeakWorkingSetSize,_T("��������ֵ"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PeakWorkingSetSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cWorkingSetSize,_T("������"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->WorkingSetSize,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cQuotaPeakPagedPoolUsage,_T("ҳ�滺��ط�ֵ"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaPeakPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cQuotaPagedPoolUsage,_T("ҳ�滺���"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cQuotaPeakNonPagedPoolUsage,_T("��ҳ�滺��ط�ֵ"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaPeakNonPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cQuotaNonPagedPoolUsage,_T("��ҳ�滺���"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->QuotaNonPagedPoolUsage,
		&ConvMEMKBStr,&CmpSizeT,TRUE,
	cPageFileUsage,_T("ҳ���ļ�"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PageFileUsage,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cPeakPageFileUsage,_T("ҳ���ļ���ֵ"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PeakPageFileUsage,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cPrivateUsage,_T("�ύ��С"),70,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->PrivateUsage,
		&ConvMEMMBStr,&CmpSizeT,TRUE,
	cReadOperationCount,_T("I/O��ȡ"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->ReadOperationCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cWriteOperationCount,_T("I/Oд��"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->WriteOperationCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cOtherOperationCount,_T("I/O����"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->OtherOperationCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cReadTransferCount,_T("I/O��ȡ�ֽ�"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->ReadTransferCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cWriteTransferCount,_T("I/Oд���ֽ�"),64,LVCFMT_RIGHT,
		0,(INT_PTR)&((PROCESS*)0)->WriteTransferCount,
		&ConvULONGStr,&CmpQword,TRUE,
	cOtherTransferCount,_T("I/O�����ֽ�"),64,LVCFMT_RIGHT,
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
	_T("#���������̣������ִ�Сд����\r\n")
	_T("#�޸���ɱ���������������������������Ч��\r\n")
;
LPTSTR CSuperTaskmgrDlg::HeadString_Black =
	_T("#���������̣������ִ�Сд����\r\n")
	_T("#�޸���ɱ���������������������������Ч��\r\n")
;
LPTSTR CSuperTaskmgrDlg::HeadString_System =
	_T("#ϵͳ�ؼ����̣������ִ�Сд����\r\n")
	_T("#�޸���ɱ���������������������������Ч��\r\n")
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
	_T("ϵͳ����\r\n")
;

//ˢ��ʱ��������
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


//���б��ļ�sFileName�ж�ȡ���ݣ����浽Dest
BOOL CSuperTaskmgrDlg::ReadListFile(LPCTSTR sFileName,LPTSTR *Dest,int &DestN,LPCTSTR szNoFileDefaultHeadString=NULL,LPCTSTR szNoFileDefaultString=NULL){
	LPTSTR pc1,pc2;
	TCHAR szBuffer[MAX_PATH];
	if (!ExistFile(sFileName)){
		//��������ڸ��б��ļ���д��һ���ļ�
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
	while(file.ReadLine(szBuffer,MAX_PATH)){//����ÿһ��
		for (pc1=szBuffer;*pc1!=_T('\0') && *pc1==_T(' ');pc1++);//����ͷ�����еĿո�
		if (*pc1==_T('\0') || *pc1==_T('#')) continue;//�ж��Ƿ�����Ч��
		for (pc2=pc1;*pc2!=_T('\0');pc2++);//Ѱ��β��
		for (pc2--;*pc2==_T(' ')||*pc2==_T('\r')||*pc2==_T('\n');pc2--);//����β�����еĿո��\r\n
		*(++pc2)=_T('\0');
		_tcslwr_s(pc1,_tcslen(pc1)+1);//ת����Сд

		//��Ӵ�����
		Dest[DestN]=SPNameContainer.NewData(pc1,pc2-pc1+1);
		(DestN)++;
	}
	if (!file.Close()) return FALSE;
	return TRUE;
}

//���б�Srcд���ļ�sFileName��
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

//�������ļ�sFileName�ж�ȡ���ݣ����浽Dest
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
		for (pc1=szBuffer;*pc1!=_T('\0') && *pc1==_T(' ');pc1++);//����ͷ�����еĿո�
		if (*pc1==_T('\0') || *pc1==_T('#')) continue;//�ж��Ƿ�����Ч��
		pc3=pc2=_tcschr(pc1,_T('='));//Ѱ�ҵȺ�
		if (!pc2 || pc1==pc2) continue;//����޵ȺŻ�Ⱥ��ڿ�ͷ����Ը���

		//��ȡ�Ⱥ�ǰ������Ϊpc1
		for (pc2--;*pc2==_T(' ')&&pc2>pc1;pc2--);//���ԵȺ�ǰ���еĿո�
		*(++pc2)=_T('\0');
		if (pc1==pc2) continue;
		_tcslwr_s(pc1,_tcslen(pc1)+1);//ת����Сд

		//��ȡ�Ⱥ�ǰ������Ϊpc3
		for (pc3++;*pc3!=_T('\0') && *pc3==_T(' ');pc3++);//���ԵȺź����еĿո�
		if (*pc3==_T('\r')||*pc3==_T('\n')||*pc3==_T('\0')) continue;
		for (pc4=pc3;*pc4!=_T('\0');pc4++);//Ѱ��β��
		for (pc4--;*pc4==_T(' ')||*pc4==_T('\r')||*pc4==_T('\n')&&pc4>pc3;pc4--);//����β�����еĿո��\n
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

			//��ȡ����ǰ������Ϊpc3
			for (pc4=pc2-1;*pc4==_T(' ');pc4--)
				if (pc3>pc4) goto lcontinue;
			*(pc4+1)=_T('\0');

			//��ȡ���ź������Ϊpc2
			for (pc2++;*pc2==_T(' ');pc2++)
				if (*pc2==_T('\0')) goto lcontinue;

			//��������
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

//������д���ļ�sFileName��
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

//��ȡ����
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

	//��ȡ��������������
	if (bReadWhiteList){
		if (!ReadListFile(sWhiteListFile,setting.WhiteList,setting.nWhiteList,HeadString_White,DefaultSetting_WhiteListFile))
			ret=FALSE;
		bReadWhiteList=FALSE;
		for (i=0;i<proN;i++)
			pro[i]->inWhite=InList(setting.WhiteList,setting.nWhiteList,pro[i]->name);
	}

	//��ȡ��������������
	if (bReadBlackList){
		if (!ReadListFile(sBlackListFile,setting.BlackList,setting.nBlackList,HeadString_Black,DefaultSetting_BlackListFile))
			ret=FALSE;
		bReadBlackList=FALSE;
		for (i=0;i<proN;i++)
			pro[i]->inBlack=InList(setting.BlackList,setting.nBlackList,pro[i]->name);
	}

	//��ȡϵͳ��������
	if (bReadSystemList){
		if (!ReadListFile(sSystemProcessFile,setting.SystemProcess,setting.nSystemProcess,HeadString_System,DefaultSetting_SystemProcessFile))
			ret=FALSE;
		bReadSystemList=FALSE;
		for (i=0;i<proN;i++)
			pro[i]->inSystem=InList(setting.SystemProcess,setting.nSystemProcess,pro[i]->name);
	}

	return ret;
}

//��������
BOOL CSuperTaskmgrDlg::SaveCFG(){
	if (!WriteCFGFile(sCFGFile,setting,ColumnOrd)) return FALSE;
	if (!WriteListFile(sWhiteListFile,setting.WhiteList,setting.nWhiteList,HeadString_White)) return FALSE;
	if (!WriteListFile(sBlackListFile,setting.BlackList,setting.nBlackList,HeadString_Black)) return FALSE;
	if (!WriteListFile(sSystemProcessFile,setting.SystemProcess,setting.nSystemProcess,HeadString_System)) return FALSE;
	return TRUE;
}

//ˢ���б�ͷ������
void CSuperTaskmgrDlg::RefreshColumnCFG(){
	int i,n;
	CHeaderCtrl *header=m_LstProc.GetHeaderCtrl();
	n=header->GetItemCount();
	for (i=0;i<n;i++){
		ColumnOrd[i]=header->OrderToIndex(i);
		setting.column[i].width=m_LstProc.GetColumnWidth(i);
	}
}

//��ʼ���б�ؼ�
void CSuperTaskmgrDlg::InitListCtrl(){
	int i;
	LVCOLUMN stColumn;
	//							����ѡȡ			������				��ѡ��			��ͷ���϶�
	m_LstProc.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES|LVS_EX_HEADERDRAGDROP);
	m_LstProc.SetRedraw(FALSE);

	//�����б�ͷ�е�����һ��
	VERIFY(m_LstProc.SetColumnCount(setting.ColumnNumber));

	//�����б�ͷ�е�ÿһ�������
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

//Ӧ������
BOOL CSuperTaskmgrDlg::ApplyCFG(){

	//��鴰��λ�õĺ�����
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

	//��ʼ������λ��
	MoveWindow(setting.left,setting.top,setting.width,setting.height);

	//��ʼ���б�ؼ�
	InitListCtrl();

	//��ʼ��ͼ���б�
	if (!m_ImageList.m_hImageList){
		VERIFY(m_ImageList.Create(16, 16, ILC_COLOR16, 8, 8));
		m_ImageList.SetBkColor(RGB(255,255,255));
		m_ImageList.Add(hIconNull);
	}
	m_LstProc.SetImageList(&m_ImageList,LVSIL_SMALL);

	//����ˢ��Ƶ��
	SetTimer(1,TimerInterval[setting.RefreshSpeed],NULL);

	//���ô����ö�
	VERIFY(SetWindowPos(setting.ShowInFront ? &CWnd::wndTopMost : &CWnd::wndNoTopMost,0,0,0,0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER));

	//��ʾ������
	if (!m_FloatingWindow.m_hWnd)
		VERIFY(m_FloatingWindow.Initialize());

	m_FloatingWindow.Setup(setting);
	return TRUE;
}



