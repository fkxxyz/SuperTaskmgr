
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include <Shlwapi.h>

const UINT CSuperTaskmgrDlg::WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));




//��������ȡ����Ŀ¼����ȡ��ȫ�ֱ��� sWorkDir
//����1��ʾ�ɹ�
BOOL CSuperTaskmgrDlg::initGetWorkDir(){
	TCHAR szBuffer[MAX_PATH];

	//��ȡ %appdata%
	VERIFY(GetEnvironmentVariable(_T("appdata"),szBuffer,MAX_PATH) != 0);

	//��ȡ sWorkDir
	_tcscat_s<MAX_PATH>(szBuffer,_T("\\"));
	_tcscat_s<MAX_PATH>(szBuffer,AppName);
	sWorkDir=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	//�����ļ��� sWorkDir
	CreateDirectory(sWorkDir,NULL);
	if (!PathIsDirectory(sWorkDir)) { ASSERT(0);return FALSE; }

	//��ʼ�������ļ�·���ַ���
	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\WhiteList.ini"),sWorkDir);
	sWhiteListFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\BlackList.ini"),sWorkDir);
	sBlackListFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\SystemProcess.ini"),sWorkDir);
	sSystemProcessFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	//��ʼ�������ļ��ַ���
	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\SuperTaskMgr.ini"),sWorkDir);
	sCFGFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	VERIFY(GetEnvironmentVariable(_T("systemroot"),lpszSystemRoot,MAX_PATH) != 0);

	return TRUE;
}

//��ʼ���ַ���
void CSuperTaskmgrDlg::initString(){
	DWORD VerValue=stOsVerInfo.dwMajorVersion*0x10000+stOsVerInfo.dwMinorVersion;
	if (VerValue<0x60000) // 2000��XP��2003 ��һ��
		DefaultSetting_SystemProcessFile=
			_tcsstr(
				DefaultSetting_AllSystemProcessFile,
				_T("system\r\n")
			);
	else if (VerValue<0x60002) // Win7 ��Vista��Server 2008
		DefaultSetting_SystemProcessFile=
			_tcsstr(
				DefaultSetting_AllSystemProcessFile,
				_T("dwm.exe\r\n")
			);
	else if (VerValue<0x60005) // Win8 ��Win8.1
		DefaultSetting_SystemProcessFile=
			_tcsstr(
				DefaultSetting_AllSystemProcessFile,
				_T("wudfhost.exe\r\n")
			);
	else  if (VerValue<0x100002) // Win10
		DefaultSetting_SystemProcessFile=
			_tcsstr(
				DefaultSetting_AllSystemProcessFile,
				_T("sihost.exe\r\n")
			);
}


//������ǰ���̵�Ȩ�ޣ�����ֵ��ʾ�Ƿ�ɹ�
BOOL CSuperTaskmgrDlg::initCurrentProcessInfo(){
	HANDLE hCurrentProcess=GetCurrentProcess();//��ǰ����α���
	if (!SetPriorityClass(hCurrentProcess,HIGH_PRIORITY_CLASS)) return FALSE;
	if (!CProcess::AquireToken(hCurrentProcess,SE_DEBUG_NAME)) return FALSE;
	return TRUE;
}

//��ʼ��һЩ����
void CSuperTaskmgrDlg::initConstant(){
	//��鳣��
#ifdef _DEBUG
	for (int i=0;i<cENUM_COLUMN_END;i++){
		ASSERT(i==ColumnItemName[i].cindex);
	}
#endif

	fMaxColorMem=(float)(stMem.ullTotalPhys/4); //��������ڴ��1/4

	//��ȡ��ǰ����ID
	dwCurrentProcessId=GetCurrentProcessId();

	//��ȡϵͳ�����ҳ��С
	dwPageSize=SystemBasicInfo.PageSize;

	//��ȡ����ϵͳλ��
	SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
	dwSystemBits=
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ?
		64 : 32;

	//��ʼ������
	hFontBold=CreateFont(12,6,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,FIXED_PITCH|FF_SWISS|FF_SCRIPT,NULL);
	hFontNomal=CreateFont(12,6,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,FIXED_PITCH|FF_SWISS|FF_SCRIPT,NULL);
}

//��ʼ������ģ��
BOOL CSuperTaskmgrDlg::initDriver(){
	//�������豸
	if (!TdiFilter.Open()){
		//��װ 360netmon ����
		C360NetmonInstaller NetmonInstaller;
		if (!NetmonInstaller.Install()) return FALSE;

		if (!TdiFilter.Open()) return FALSE;
	}


	//����豸�Ƿ�������
	BOOL bRunning;
	if (!TdiFilter.GetStatus(&bRunning)) return FALSE;

	//��ʼ�������
	if (!bRunning)
		if (!TdiFilter.Start()) return FALSE;
	return TRUE;
}

//��ʼ������
//����ֵ��ʾ��ʼ���Ƿ�ɹ�
BOOL CSuperTaskmgrDlg::MainInit(){

	//��ʼ������Ŀ¼
	if (!initGetWorkDir()){
		ShowError(m_hWnd,AppName,_T("��ȡ����Ŀ¼ʧ�ܣ�\n"),NULL,GetLastError());
		return FALSE;
	}

	//��ȡϵͳ�汾��Ϣ
	stOsVerInfo.dwOSVersionInfoSize=sizeof(stOsVerInfo);
	if (!RealGetVersionEx(&stOsVerInfo)){
		ShowError(m_hWnd,AppName,_T("�޷���ȡϵͳ�汾��\n"),NULL,GetLastError());
		return FALSE;
	}

	//��ȡȫ���ڴ���Ϣ
	stMem.dwLength=sizeof(MEMORYSTATUSEX);
	if (!GlobalMemoryStatusEx(&stMem)){
		ShowError(m_hWnd,AppName,_T("�޷���ȡȫ���ڴ�״̬��\n"),NULL,GetLastError());
		return FALSE;
	}

	//��ʼ������
	initConstant();
	hIconNull=AfxGetApp()->LoadIcon(IDI_ICONNULL);

	//��ʼ���ַ���
 	initString();

	//��ʾ���������������Ȩ��
	VERIFY(initCurrentProcessInfo());

	//��ȡϵͳ������Ϣ
	NTSTATUS status;
	if (!NT_SUCCESS(status=NtQuerySystemBasicInfo(&SystemBasicInfo))){
		ShowError(m_hWnd,AppName,_T("�޷���ȡϵͳ������Ϣ��\n"),NULL,RtlNtStatusToDosError(status));
		return FALSE;
	}
	dwCSRSS_PID=GetCsrssPID();

	//��ʼ������ģ��
 	VERIFY(initDriver());

	return TRUE;
}

//�ͷ��ڴ棬�˳�����
void CSuperTaskmgrDlg::FreeAll(){

	int i;
	for (i=0;i<proN;i++){
		DestroyIcon(pro[i]->hIcon);
	}
	proN=0;

	VERIFY(DestroyIcon(hIconNull));
}

//��ֹ����
//���н���ʱ����
BOOL CSuperTaskmgrDlg::MainTerm(){
	FreeAll();
	return TRUE;
}
