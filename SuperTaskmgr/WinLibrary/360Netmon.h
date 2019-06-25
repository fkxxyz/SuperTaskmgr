
// 360Netmon.h

#pragma once

#include "WinNtDriver.h"

//  ==========================   Installer   ==========================
class C360NetmonInstaller : public CNtDriverInstaller {
private:
#ifndef _WIN64
	static const DWORD drv_50[87674];
	static const DWORD drv_60[18544];
	static const DWORD drv_wfp[20736];
#endif
	static const DWORD drv_x64[20336];
	static const DWORD drv_x64_wfp[22528];
	static const DWORD dwDrvBuildNumber;

	BOOL FindSupportDriver(const DWORD *&drv,size_t &size);

public:

	BOOL Install();
	BOOL Uninstall();

	static BOOL StartNetmon();
	static BOOL StopNetmon();
};
//  =========================================================================




//  ==========================   Io Control Code   ==========================
#define IOCTL_360TDIFILTER_GET_VERSION \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x100,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_GET_PROCESS_INIT_FLOW \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x101,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_START \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x102,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_STOP \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x103,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_GET_STATUS \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x104,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_GET_GLOBAL_FLOW \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x105,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_GET_PROCESS_FLOW \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x106,FILE_ANY_ACCESS,METHOD_BUFFERED)


#define IOCTL_360TDIFILTER_LIMIT_PROCESS_SPEED \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x201,FILE_ANY_ACCESS,METHOD_BUFFERED)

#define IOCTL_360TDIFILTER_GET_PROCESS_LIMITED \
	CTL_CODE(FILE_DEVICE_UNKNOWN,0x202,FILE_ANY_ACCESS,METHOD_BUFFERED)

//  =========================================================================





//  ==========================   Structures   ===============================
#pragma pack(push)
#pragma pack(4)

#define MAX_NP_NUMBER 100

typedef struct _DEVICE_VERSION {
	DWORD dwMajor,dwMinor,dwRevision,dwBuild;
} DEVICE_VERSION, *PDEVICE_VERSION;
typedef struct _INIT_FLOW {
	DWORD dwPid;
	LONGLONG SendFlow;
	LONGLONG RecvFlow;
	LONGLONG nDisabled; // 0x7FFFFFFFFFFFFFFF is disabled
	LONGLONG nReserve;  // input 0x7FFFFFFFFFFFFFFF
	LONGLONG SendSpeed;
	LONGLONG RecvSpeed;
	LONGLONG RecvFlow1;
	LONGLONG SendFlow1;
	LONGLONG SendSpeed1;
	LONGLONG RecvSpeed1;
} INIT_FLOW, *PINIT_FLOW;
typedef struct _PROCESS_INIT_FLOW {
	long nProcess;
	INIT_FLOW flow[MAX_NP_NUMBER];
} PROCESS_INIT_FLOW, *PPROCESS_INIT_FLOW;
typedef struct _GLOBAL_FLOW {
	LONGLONG SendFlow;
	LONGLONG RecvFlow;
	LONGLONG SendSpeed;
	LONGLONG RecvSpeed;
	LONGLONG SendFlow1;
	LONGLONG RecvFlow1;
	LONGLONG SendSpeed1;
	LONGLONG RecvSpeed1;
} GLOBAL_FLOW, *PGLOBAL_FLOW;
typedef struct _FLOW {
	DWORD dwPid;
	LONGLONG SendFlow;
	LONGLONG RecvFlow;
	LONGLONG nDisabled; // 0x7FFFFFFFFFFFFFFF is disabled
	LONGLONG nReserve;  // input 0x7FFFFFFFFFFFFFFF
	LONGLONG SendSpeed;
	LONGLONG RecvSpeed;
	LONGLONG RecvFlow1;
	LONGLONG SendFlow1;
	LONGLONG SendSpeed1;
	LONGLONG RecvSpeed1;
	LONGLONG nReserve1; // output 0x7FFFFFFFFFFFFFFF
} FLOW, *PFLOW;
typedef struct _PROCESS_FLOW {
	long nProcess;
	FLOW flow[MAX_NP_NUMBER];
} PROCESS_FLOW, *PPROCESS_FLOW;


typedef struct _LIMIT_SPEED {
	WCHAR szNtPath[MAX_PATH+40];
	WCHAR szPath[MAX_PATH];
	BOOL bCancelFlag;
	LONGLONG qwBlockCnnt;
	LONGLONG nLimitSend;
	LONGLONG nLimitRecv;
	DWORD dwZeroCheck;
} LIMIT_SPEED, *PLIMIT_SPEED;
typedef struct _PROCESS_LIMITED {
	long nProcess;
	LIMIT_SPEED limit[MAX_NP_NUMBER];
} PROCESS_LIMITED, *PPROCESS_LIMITED;

#pragma pack(pop)
//  =========================================================================





//  ==========================   Device class   =============================
class C360TdiFilter : public CDevice {

public:
	BOOL Open();
	BOOL GetVersion(PDEVICE_VERSION pVersion);
	BOOL GetProcessInitFlow(PPROCESS_INIT_FLOW pInitProcessFlow);
	BOOL Start();
	BOOL Stop();
	BOOL GetStatus(LPBOOL pRunning);
	BOOL GetGlobalFlow(PGLOBAL_FLOW pGlobalFlow);
	BOOL GetProcessFlow(PPROCESS_FLOW pProcessFlow);

	BOOL LimitProcessSpeed(LPCTSTR szImagePath,LONGLONG nLimitSend,LONGLONG nLimitRecv);
	BOOL DisableProcessNetwork(LPCTSTR szImagePath);
	BOOL EnableProcessNetwork(LPCTSTR szImagePath);
	BOOL GetProcessLimited(PPROCESS_LIMITED pProcessLimited);
};



#include "360Netmon.inl"




