
// WinProcessApi.cpp

#include "stdafx.h"

#pragma warning(disable: 4073)
#pragma init_seg(lib)

typedef NTSTATUS (NTAPI *FUNC_NtSuspendProcess)(HANDLE);
DEFINE_WINAPI(FUNC_NtSuspendProcess,NtSuspendProcess,
	L"ntdll","NtSuspendProcess",TRUE);

typedef NTSTATUS (NTAPI *FUNC_NtResumeProcess)(HANDLE);
DEFINE_WINAPI(FUNC_NtResumeProcess,NtResumeProcess,
	L"ntdll","NtResumeProcess",TRUE);

typedef NTSTATUS (NTAPI *FUNC_NtQueryInformationProcess)(HANDLE, DWORD, PVOID, ULONG, PULONG);
DEFINE_WINAPI(FUNC_NtQueryInformationProcess,NtQueryInformationProcess,
	L"ntdll","NtQueryInformationProcess",TRUE);


