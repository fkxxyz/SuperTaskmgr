
// WinSystemInfoApi.cpp

#include "stdafx.h"

#pragma warning(disable: 4073)
#pragma init_seg(lib)

typedef ULONG (NTAPI *FUNC_RtlNtStatusToDosError)(NTSTATUS Status);
DEFINE_WINAPI(FUNC_RtlNtStatusToDosError,RtlNtStatusToDosError,
	L"ntdll","RtlNtStatusToDosError",TRUE);

typedef void (NTAPI *FUNC_RtlGetNtVersionNumbers)(LPDWORD,LPDWORD,LPDWORD);
DEFINE_WINAPI(FUNC_RtlGetNtVersionNumbers,RtlGetNtVersionNumbers,
	L"ntdll","RtlGetNtVersionNumbers",TRUE);

typedef NTSTATUS (NTAPI *FUNC_NtQuerySystemInformation)(UINT, PVOID, ULONG, PULONG);
DEFINE_WINAPI(FUNC_NtQuerySystemInformation,NtQuerySystemInformation,
	L"ntdll","NtQuerySystemInformation",TRUE);



