
// WinAPI.h

/*
	请在 stdafx.cpp 里面定义所有的未导出的 api 函数
	示例：
#pragma warning(disable: 4073)
#pragma init_seg(lib)

typedef BOOL (WINAPI *FUNC_IsWow64Process)(HANDLE,PBOOL);
DEFINE_WINAPI(FUNC_IsWow64Process,IsWow64Process,L"kernel32","IsWow64Process");

typedef ULONG (WINAPI *FUNC_RtlNtStatusToDosError)(NTSTATUS Status);
DEFINE_WINAPI(FUNC_RtlNtStatusToDosError,RtlNtStatusToDosError,L"ntdll","RtlNtStatusToDosError");

typedef void (WINAPI *FUNC_RtlGetNtVersionNumbers)(LPDWORD,LPDWORD,LPDWORD);
DEFINE_WINAPI(FUNC_RtlGetNtVersionNumbers,RtlGetNtVersionNumbers,L"ntdll","RtlGetNtVersionNumbers");

*/


#pragma once

LPVOID GetAPIAddress(LPCSTR lpModuleName,LPCSTR lpProcName,BOOL bAssertNull);
LPVOID GetAPIAddress(LPCWSTR lpModuleName,LPCSTR lpProcName,BOOL bAssertNull);

#define DEFINE_WINAPI(FUNC_TYPE, lpAddress, lpModuleName, lpProcName, bAssertNull) \
	FUNC_TYPE lpAddress=(FUNC_TYPE)GetAPIAddress(lpModuleName,lpProcName,bAssertNull)

extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);


