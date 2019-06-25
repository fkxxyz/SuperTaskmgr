
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include <Shlwapi.h>

const UINT CSuperTaskmgrDlg::WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));




//创建并获取工作目录，获取到全局变量 sWorkDir
//返回1表示成功
BOOL CSuperTaskmgrDlg::initGetWorkDir(){
	TCHAR szBuffer[MAX_PATH];

	//获取 %appdata%
	VERIFY(GetEnvironmentVariable(_T("appdata"),szBuffer,MAX_PATH) != 0);

	//获取 sWorkDir
	_tcscat_s<MAX_PATH>(szBuffer,_T("\\"));
	_tcscat_s<MAX_PATH>(szBuffer,AppName);
	sWorkDir=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	//创建文件夹 sWorkDir
	CreateDirectory(sWorkDir,NULL);
	if (!PathIsDirectory(sWorkDir)) { ASSERT(0);return FALSE; }

	//初始化三个文件路径字符串
	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\WhiteList.ini"),sWorkDir);
	sWhiteListFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\BlackList.ini"),sWorkDir);
	sBlackListFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\SystemProcess.ini"),sWorkDir);
	sSystemProcessFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	//初始化设置文件字符串
	_stprintf_s<MAX_PATH>(szBuffer,_T("%s\\SuperTaskMgr.ini"),sWorkDir);
	sCFGFile=FilePathContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

	VERIFY(GetEnvironmentVariable(_T("systemroot"),lpszSystemRoot,MAX_PATH) != 0);

	return TRUE;
}

//初始化字符串
void CSuperTaskmgrDlg::initString(){
	DWORD VerValue=stOsVerInfo.dwMajorVersion*0x10000+stOsVerInfo.dwMinorVersion;
	if (VerValue<0x60000) // 2000、XP、2003 及一下
		DefaultSetting_SystemProcessFile=
			_tcsstr(
				DefaultSetting_AllSystemProcessFile,
				_T("system\r\n")
			);
	else if (VerValue<0x60002) // Win7 、Vista、Server 2008
		DefaultSetting_SystemProcessFile=
			_tcsstr(
				DefaultSetting_AllSystemProcessFile,
				_T("dwm.exe\r\n")
			);
	else if (VerValue<0x60005) // Win8 、Win8.1
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


//提升当前进程的权限，返回值表示是否成功
BOOL CSuperTaskmgrDlg::initCurrentProcessInfo(){
	HANDLE hCurrentProcess=GetCurrentProcess();//当前进程伪句柄
	if (!SetPriorityClass(hCurrentProcess,HIGH_PRIORITY_CLASS)) return FALSE;
	if (!CProcess::AquireToken(hCurrentProcess,SE_DEBUG_NAME)) return FALSE;
	return TRUE;
}

//初始化一些常数
void CSuperTaskmgrDlg::initConstant(){
	//检查常数
#ifdef _DEBUG
	for (int i=0;i<cENUM_COLUMN_END;i++){
		ASSERT(i==ColumnItemName[i].cindex);
	}
#endif

	fMaxColorMem=(float)(stMem.ullTotalPhys/4); //最大物理内存的1/4

	//获取当前进程ID
	dwCurrentProcessId=GetCurrentProcessId();

	//获取系统定义的页大小
	dwPageSize=SystemBasicInfo.PageSize;

	//获取操作系统位数
	SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
	dwSystemBits=
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ?
		64 : 32;

	//初始化字体
	hFontBold=CreateFont(12,6,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,FIXED_PITCH|FF_SWISS|FF_SCRIPT,NULL);
	hFontNomal=CreateFont(12,6,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,FIXED_PITCH|FF_SWISS|FF_SCRIPT,NULL);
}

//初始化驱动模块
BOOL CSuperTaskmgrDlg::initDriver(){
	//打开驱动设备
	if (!TdiFilter.Open()){
		//安装 360netmon 驱动
		C360NetmonInstaller NetmonInstaller;
		if (!NetmonInstaller.Install()) return FALSE;

		if (!TdiFilter.Open()) return FALSE;
	}


	//检查设备是否工作正常
	BOOL bRunning;
	if (!TdiFilter.GetStatus(&bRunning)) return FALSE;

	//开始监控网络
	if (!bRunning)
		if (!TdiFilter.Start()) return FALSE;
	return TRUE;
}

//初始化函数
//返回值表示初始化是否成功
BOOL CSuperTaskmgrDlg::MainInit(){

	//初始化工作目录
	if (!initGetWorkDir()){
		ShowError(m_hWnd,AppName,_T("获取工作目录失败！\n"),NULL,GetLastError());
		return FALSE;
	}

	//获取系统版本信息
	stOsVerInfo.dwOSVersionInfoSize=sizeof(stOsVerInfo);
	if (!RealGetVersionEx(&stOsVerInfo)){
		ShowError(m_hWnd,AppName,_T("无法获取系统版本！\n"),NULL,GetLastError());
		return FALSE;
	}

	//获取全局内存信息
	stMem.dwLength=sizeof(MEMORYSTATUSEX);
	if (!GlobalMemoryStatusEx(&stMem)){
		ShowError(m_hWnd,AppName,_T("无法获取全局内存状态！\n"),NULL,GetLastError());
		return FALSE;
	}

	//初始化常数
	initConstant();
	hIconNull=AfxGetApp()->LoadIcon(IDI_ICONNULL);

	//初始化字符串
 	initString();

	//提示超级任务管理器的权限
	VERIFY(initCurrentProcessInfo());

	//获取系统基本信息
	NTSTATUS status;
	if (!NT_SUCCESS(status=NtQuerySystemBasicInfo(&SystemBasicInfo))){
		ShowError(m_hWnd,AppName,_T("无法获取系统基本信息！\n"),NULL,RtlNtStatusToDosError(status));
		return FALSE;
	}
	dwCSRSS_PID=GetCsrssPID();

	//初始化驱动模块
 	VERIFY(initDriver());

	return TRUE;
}

//释放内存，退出程序
void CSuperTaskmgrDlg::FreeAll(){

	int i;
	for (i=0;i<proN;i++){
		DestroyIcon(pro[i]->hIcon);
	}
	proN=0;

	VERIFY(DestroyIcon(hIconNull));
}

//终止函数
//运行结束时调用
BOOL CSuperTaskmgrDlg::MainTerm(){
	FreeAll();
	return TRUE;
}
