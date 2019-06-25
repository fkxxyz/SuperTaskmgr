
// WinNtDriver.cpp

#include "stdafx.h"
#include "WinNtDriver.h"

#include "WinSCManager.h"


DWORD CNtDriverService::dwVersionValue;
BOOL CNtDriverService::bIs64BitSystem;
BOOL CNtDriverService::bIsWow64Process;

extern BOOL (WINAPI *dlWow64DisableWow64FsRedirection)(PVOID *);
extern BOOL (WINAPI *dlWow64RevertWow64FsRedirection)(PVOID);


CNtDriverService::CNtDriverService(){
	//获取系统信息
	GetSystemVersion(dwVersionValue, bIs64BitSystem, bIsWow64Process);
	VERIFY(CProcess::AquireToken(GetCurrentProcess(),SE_LOAD_DRIVER_NAME));
}
CNtDriverService::~CNtDriverService(){
}

void CNtDriverService::GetSystemVersion(DWORD &dwVersionValue,BOOL &bIs64BitSystem,BOOL &bIsWow64Process){
	//获取系统版本信息
	DWORD dwMajorVersion,dwMinorVersion,dwBuildNumber;
	RtlGetNtVersionNumbers(&dwMajorVersion,&dwMinorVersion,&dwBuildNumber);
	dwVersionValue=(dwMajorVersion<<16)+dwMinorVersion;

	//获取系统信息
	SYSTEM_INFO si;
	if (GetNativeSystemInfo!=NULL)
		GetNativeSystemInfo(&si);
	else
		GetSystemInfo(&si);
	bIs64BitSystem=si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64;

	if (!IsWow64Process(GetCurrentProcess(),&bIsWow64Process))
		bIsWow64Process=FALSE;
}

BOOL CNtDriverService::Install(
	LPCTSTR szServiceName,
	LPCTSTR szDisplayName,
	LPCTSTR szImagePath,
	DWORD dwStartType,
	DWORD dwErrorControl,
	LPCTSTR szGroup,
	DWORD dwTag
	)
{
	TCHAR szKey[1024];
	TCHAR szImageNtPath[MAX_PATH];
	_stprintf_s<MAX_PATH>(szImageNtPath,_T("\\??\\%s"),szImagePath);

	// 设置键值 SYSTEM\\CurrentControlSet\\Services\\%Service%
	_stprintf_s<1024>(szKey,_T("SYSTEM\\CurrentControlSet\\Services\\%s"),szServiceName);
	CWinRegKey key;
	if (!key.Create(HKEY_LOCAL_MACHINE,szKey)) return FALSE;
	if (!key.SetStringValue(_T("DisplayName"),szDisplayName)) return FALSE;
	if (!key.SetStringValue(_T("ImagePath"),szImageNtPath,REG_EXPAND_SZ)) return FALSE;
	if (!key.SetDWORDValue(_T("Start"),dwStartType)) return FALSE;
	if (!key.SetDWORDValue(_T("Type"),SERVICE_KERNEL_DRIVER)) return FALSE;
	if (!key.SetDWORDValue(_T("ErrorControl"),dwErrorControl)) return FALSE;
	if (szGroup && !key.SetStringValue(_T("Group"),szGroup)) return FALSE;
	if (dwTag && !key.SetDWORDValue(_T("Tag"),dwTag)) return FALSE;

	DWORD dwDeleteFlag;
	if (key.QueryDWORDValue(_T("DeleteFlag"),&dwDeleteFlag)){
		if (dwDeleteFlag==1){
			if (!key.DeleteValue(_T("DeleteFlag")))
				return FALSE;
		}
	}

	//设置 ZwLoadDriver 的参数 UNICODE_STRING
	UNICODE_STRING us;
	us.Buffer=(LPWSTR)szKey;
	GetDriverRegisterName(us.Buffer,512,szServiceName);
	us.Length=(USHORT)wcslen(us.Buffer)*2;
	us.MaximumLength=us.Length+2;

	// 开始加载
	NTSTATUS status=ZwLoadDriver(&us);
	if (!NT_SUCCESS(status)){
		DebugPrint(_T("ZwLoadDriver() returns 0x%x\n"),status);
	}
	SetLastError(RtlNtStatusToDosError(status));
	return NT_SUCCESS(status) || status==STATUS_IMAGE_ALREADY_LOADED;
}

BOOL CNtDriverService::Start(LPCTSTR szServiceName){
	CSCManager SCM;
	if (!SCM.Open()) return FALSE;
	CService Service(SCM);
	if (!Service.Open(szServiceName)) return FALSE;
	if (!Service.Start()){
		return GetLastError() == ERROR_SERVICE_ALREADY_RUNNING;
	}
	return TRUE;
}

BOOL CNtDriverService::Stop(LPCTSTR szServiceName){
	CSCManager SCM;
	if (!SCM.Open()) return FALSE;
	CService Service(SCM);
	if (!Service.Open(szServiceName)) return FALSE;
	if (!Service.Stop()) return FALSE;
	return TRUE;
}

BOOL CNtDriverService::Uninstall(LPCTSTR szServiceName){
	// 尝试用 SCManager 删除服务
	CSCManager SCM;
	SCM.Open();
	CService Service(SCM);
	if (
		Service.Open(szServiceName)
			&&
		Service.Delete()
		)
		return TRUE;

	// 通过注册表把服务标记为删除
	TCHAR szKey[1024];
	_stprintf_s<1024>(szKey,_T("SYSTEM\\CurrentControlSet\\Services\\%s"),szServiceName);
	CWinRegKey key;
	if (!key.Open(HKEY_LOCAL_MACHINE,szKey))
		return GetLastError()==ERROR_FILE_NOT_FOUND;
	if (
		key.SetDWORDValue(_T("Start"),SERVICE_DISABLED)
			&&
		key.SetDWORDValue(_T("DeleteFlag"),TRUE)
		)
		return TRUE;

	// 通过 ZwUnloadDriver 删除服务
	UNICODE_STRING us;
	us.Buffer=(LPWSTR)szKey;
	GetDriverRegisterName(us.Buffer,512,szServiceName);
	us.Length=(USHORT)wcslen(us.Buffer)*2;
	us.MaximumLength=us.Length+2;
	NTSTATUS status=ZwUnloadDriver(&us);
	if (!NT_SUCCESS(status)){
		DebugPrint(_T("ZwUnloadDriver() returns 0x%x\n"),status);
	}
	SetLastError(RtlNtStatusToDosError(status));
	return NT_SUCCESS(status);
}



BOOL CNtDriverInstaller::CopySysFileToSystem(const DWORD *drv,size_t size,LPCTSTR szDrvFileName,TCHAR szSysImagePath[],DWORD dwNewBuildNumber){
	CLastErrorFunction result;
	BOOL &bIsWow64Process = CNtDriverService::bIsWow64Process;

	LPVOID OldFSValue;
	if (bIsWow64Process){
		//禁用 Wow64 重定向
		assert(dlWow64DisableWow64FsRedirection!=NULL);
		if (!dlWow64DisableWow64FsRedirection(&OldFSValue))
			return FALSE;
	}

	//获取系统 system32 路径
	if (GetSystemDirectory(szSysImagePath,MAX_PATH) == 0){
		result.SetErrorCode();
		goto end;
	}

	//获取到驱动的路径
	_tcscat_s(szSysImagePath,MAX_PATH,_T("\\drivers\\"));
	_tcscat_s(szSysImagePath,MAX_PATH,szDrvFileName);

	if (ExistFile(szSysImagePath)){
		TCHAR szFileVerson[64];
		int cchChar=64;
		//获取文件版本
		if (!GetPeFileVersionInfoItem(szSysImagePath,L"FileVersion",szFileVerson,&cchChar)){
			result.SetErrorCode();
			goto end;
		}
		DWORD dwBuildVersion,dwReserve;
		_stscanf_s(szFileVerson,_T("%d.%d.%d.%d"),&dwReserve,&dwReserve,&dwReserve,&dwBuildVersion);
		
		if (dwBuildVersion>=dwNewBuildNumber)
			goto end;
	}

	{
		//写入驱动文件
		CBinFile file;
		if (
			!file.OpenWrite(szSysImagePath) ||
			!file.Write(drv,(DWORD)size) ||
			!file.Close()
			)
		{
			result.SetErrorCode();
			goto end;
		}
	}

end:
	if (bIsWow64Process){
		//启用 Wow64 重定向
		assert(dlWow64RevertWow64FsRedirection!=NULL);
		if (!dlWow64RevertWow64FsRedirection(OldFSValue))
			return FALSE;
	}
	return result;
}

BOOL CNtDriverInstaller::RemoveSysFile(TCHAR szSysImagePath[]){
	LPVOID OldFSValue;
	BOOL &bIsWow64Process = CNtDriverService::bIsWow64Process;

	if (bIsWow64Process){
		//禁用 Wow64 重定向
		assert(dlWow64DisableWow64FsRedirection!=NULL);
		if (!dlWow64DisableWow64FsRedirection(&OldFSValue))
			return FALSE;
	}

	BOOL bResult=DeleteFile(szSysImagePath);

	if (bIsWow64Process){
		//启用 Wow64 重定向
		assert(dlWow64RevertWow64FsRedirection!=NULL);
		if (!dlWow64RevertWow64FsRedirection(OldFSValue))
			return FALSE;
	}
	return bResult;
}

void CNtDriverService::GetDriverRegisterName(LPWSTR szwDest,size_t nDest,LPCTSTR szServiceName){
	wcscpy_s(szwDest,nDest,L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
#ifdef UNICODE
	wcscat_s(szwDest,nDest,szServiceName);
#else
	VERIFY(MultiByteToWideChar(
		CP_ACP,0,
		szServiceName,
		(int)strlen(szServiceName)+1,
		szwDest+wcslen(szwDest),
		(int)nDest-52
		)
		);
#endif
}

BOOL CNtDriverInstaller::InstallDriverImage(
	LPCTSTR szSysFileName,
	LPCTSTR szServiceName,
	LPCTSTR szDisplayName,
	DWORD dwStartType,
	DWORD dwErrorControl,
	LPCTSTR szGroup,
	DWORD dwTag,
	DWORD dwBuildNumber
	)
{
	//通过系统版本得到应该安装的驱动版本
	const DWORD *drv; size_t size;
	if (!FindSupportDriver(drv,size)) return FALSE;

	//复制驱动文件到系统驱动目录
	if (!CopySysFileToSystem(drv,size,szSysFileName,szSysImagePath,dwBuildNumber)) return FALSE;

	//安装驱动服务
	if (!DriverService.Install(
		szServiceName,
		szDisplayName,
		szSysImagePath,
		dwStartType,
		dwErrorControl,
		szGroup,
		dwTag
		))
		return FALSE;
	return TRUE;
}






