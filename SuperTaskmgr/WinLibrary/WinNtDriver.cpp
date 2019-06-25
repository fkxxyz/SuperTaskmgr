
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
	//��ȡϵͳ��Ϣ
	GetSystemVersion(dwVersionValue, bIs64BitSystem, bIsWow64Process);
	VERIFY(CProcess::AquireToken(GetCurrentProcess(),SE_LOAD_DRIVER_NAME));
}
CNtDriverService::~CNtDriverService(){
}

void CNtDriverService::GetSystemVersion(DWORD &dwVersionValue,BOOL &bIs64BitSystem,BOOL &bIsWow64Process){
	//��ȡϵͳ�汾��Ϣ
	DWORD dwMajorVersion,dwMinorVersion,dwBuildNumber;
	RtlGetNtVersionNumbers(&dwMajorVersion,&dwMinorVersion,&dwBuildNumber);
	dwVersionValue=(dwMajorVersion<<16)+dwMinorVersion;

	//��ȡϵͳ��Ϣ
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

	// ���ü�ֵ SYSTEM\\CurrentControlSet\\Services\\%Service%
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

	//���� ZwLoadDriver �Ĳ��� UNICODE_STRING
	UNICODE_STRING us;
	us.Buffer=(LPWSTR)szKey;
	GetDriverRegisterName(us.Buffer,512,szServiceName);
	us.Length=(USHORT)wcslen(us.Buffer)*2;
	us.MaximumLength=us.Length+2;

	// ��ʼ����
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
	// ������ SCManager ɾ������
	CSCManager SCM;
	SCM.Open();
	CService Service(SCM);
	if (
		Service.Open(szServiceName)
			&&
		Service.Delete()
		)
		return TRUE;

	// ͨ��ע���ѷ�����Ϊɾ��
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

	// ͨ�� ZwUnloadDriver ɾ������
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
		//���� Wow64 �ض���
		assert(dlWow64DisableWow64FsRedirection!=NULL);
		if (!dlWow64DisableWow64FsRedirection(&OldFSValue))
			return FALSE;
	}

	//��ȡϵͳ system32 ·��
	if (GetSystemDirectory(szSysImagePath,MAX_PATH) == 0){
		result.SetErrorCode();
		goto end;
	}

	//��ȡ��������·��
	_tcscat_s(szSysImagePath,MAX_PATH,_T("\\drivers\\"));
	_tcscat_s(szSysImagePath,MAX_PATH,szDrvFileName);

	if (ExistFile(szSysImagePath)){
		TCHAR szFileVerson[64];
		int cchChar=64;
		//��ȡ�ļ��汾
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
		//д�������ļ�
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
		//���� Wow64 �ض���
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
		//���� Wow64 �ض���
		assert(dlWow64DisableWow64FsRedirection!=NULL);
		if (!dlWow64DisableWow64FsRedirection(&OldFSValue))
			return FALSE;
	}

	BOOL bResult=DeleteFile(szSysImagePath);

	if (bIsWow64Process){
		//���� Wow64 �ض���
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
	//ͨ��ϵͳ�汾�õ�Ӧ�ð�װ�������汾
	const DWORD *drv; size_t size;
	if (!FindSupportDriver(drv,size)) return FALSE;

	//���������ļ���ϵͳ����Ŀ¼
	if (!CopySysFileToSystem(drv,size,szSysFileName,szSysImagePath,dwBuildNumber)) return FALSE;

	//��װ��������
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






