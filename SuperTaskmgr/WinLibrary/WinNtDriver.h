
// WinNtDriver.h

#pragma once

#include <WinIoCtl.h>

extern NTSTATUS (NTAPI *ZwLoadDriver)(PUNICODE_STRING);
extern NTSTATUS (NTAPI *ZwUnloadDriver)(PUNICODE_STRING);
extern void (NTAPI *RtlGetNtVersionNumbers)(LPDWORD,LPDWORD,LPDWORD);
extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);

class CNtDriverService {
	static void GetSystemVersion(DWORD &dwVersionValue,BOOL &bIs64BitSystem,BOOL &bIsWow64Process);
	static void GetDriverRegisterName(LPWSTR szwDest,size_t nDest,LPCTSTR szServiceName);

public:
	static DWORD dwVersionValue;
	static BOOL bIs64BitSystem;
	static BOOL bIsWow64Process;

public:
	CNtDriverService();
	~CNtDriverService();

	static BOOL Install(
		LPCTSTR szServiceName,
		LPCTSTR szDisplayName,
		LPCTSTR szImagePath,
		DWORD dwStartType = SERVICE_SYSTEM_START,
		DWORD dwErrorControl = SERVICE_ERROR_IGNORE,
		LPCTSTR szGroup = NULL,
		DWORD dwTag = 0
		);
	static BOOL Start(LPCTSTR szServiceName);
	static BOOL Stop(LPCTSTR szServiceName);
	static BOOL Uninstall(LPCTSTR szServiceName);
};

class CNtDriverInstaller {
protected:
	CNtDriverService DriverService;

	TCHAR szSysImagePath[MAX_PATH];

	virtual BOOL FindSupportDriver(const DWORD *&drv,size_t &size) = 0;
	virtual BOOL CopySysFileToSystem(
		const DWORD *drv,
		size_t size,
		LPCTSTR szSysFileName,
		TCHAR szSysImagePath[],
		DWORD dwBuildNumber
	);
	virtual BOOL RemoveSysFile(TCHAR szSysImagePath[]);
	virtual BOOL InstallDriverImage(
		LPCTSTR szSysFileName,
		LPCTSTR szServiceName,
		LPCTSTR szDisplayName,
		DWORD dwStartType = SERVICE_SYSTEM_START,
		DWORD dwErrorControl = SERVICE_ERROR_IGNORE,
		LPCTSTR szGroup = NULL,
		DWORD dwTag = 0,
		DWORD dwBuildNumber = 0
		);

public:
	virtual BOOL Install() = 0;
	virtual BOOL Uninstall() = 0;
};

class CDevice : public CKernelObject {
protected:
	HANDLE &m_hDevice;

	// CreateFile
	BOOL Open(
		LPCTSTR lpDeviceName,
		DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE,
		DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE
		)
	{
		assert(m_hDevice==NULL);
		m_hDevice=CreateFile(
			lpDeviceName,
			dwDesiredAccess,
			dwShareMode,
			NULL,
			OPEN_EXISTING,
			0,
			NULL
			);
		if (m_hDevice==INVALID_HANDLE_VALUE)
			m_hDevice=NULL;
		return m_hDevice!=NULL;
	}

public:
	CDevice():m_hDevice(m_Handle){}

	virtual BOOL Open() = 0;

	// DeviceIoControl
	virtual BOOL IoControl(
		DWORD dwIoControlCode,
		LPVOID lpInBuffer,
		DWORD nInBufferSize,
		LPVOID lpOutBuffer,
		DWORD nOutBufferSize,
		LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped=NULL
		)
	{
		assert(m_hDevice);
		return DeviceIoControl(
			m_hDevice,
			dwIoControlCode,
			lpInBuffer,
			nInBufferSize,
			lpOutBuffer,
			nOutBufferSize,
			lpBytesReturned,
			lpOverlapped
			);
	}

	BOOL IoCode(DWORD dwIoControlCode,LPOVERLAPPED lpOverlapped=NULL){
		DWORD dwBytesReturned;
		if (!IoControl(dwIoControlCode,NULL,0,NULL,0,&dwBytesReturned,lpOverlapped))
			return FALSE;
		assert(dwBytesReturned==0);
		return TRUE;
	}
	BOOL IoSend(DWORD dwIoControlCode,void *pData,DWORD nSize,LPOVERLAPPED lpOverlapped=NULL){
		DWORD dwBytesReturned;
		if (!IoControl(dwIoControlCode,pData,nSize,NULL,0,&dwBytesReturned,lpOverlapped))
			return FALSE;
		assert(dwBytesReturned==0);
		return TRUE;
	}
	BOOL IoRecv(DWORD dwIoControlCode,void *pData,DWORD nSize,LPOVERLAPPED lpOverlapped=NULL){
		DWORD dwBytesReturned;
		if (!IoControl(dwIoControlCode,NULL,0,pData,nSize,&dwBytesReturned,lpOverlapped))
			return FALSE;
		assert(nSize==dwBytesReturned);
		return TRUE;
	}
	BOOL IoRecv(DWORD dwIoControlCode,void *pData,DWORD nSize,LPDWORD lpBytesReturned,LPOVERLAPPED lpOverlapped=NULL){
		return IoControl(dwIoControlCode,NULL,0,pData,nSize,lpBytesReturned,lpOverlapped);
	}

	BOOL IoSendDWORD(DWORD dwIoControlCode,DWORD dwData,LPOVERLAPPED lpOverlapped=NULL){
		return IoSend(dwIoControlCode,&dwData,sizeof(DWORD),0);
	}
	BOOL IoRecvDWORD(DWORD dwIoControlCode,LPDWORD pData,LPOVERLAPPED lpOverlapped=NULL){
		return IoRecv(dwIoControlCode,pData,sizeof(DWORD),lpOverlapped);
	}
};





