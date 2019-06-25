// stdafx.cpp : 只包括标准包含文件的源文件
// WinLib.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用


#pragma warning(disable: 4073)
#pragma init_seg(lib)



typedef BOOL (WINAPI *FUNC_EndTask)(HWND,BOOL,BOOL);
DEFINE_WINAPI(FUNC_EndTask,EndTask,
	L"user32","EndTask",TRUE);

typedef NTSTATUS (NTAPI *FUNC_NtTerminateProcess)(HANDLE,UINT);
DEFINE_WINAPI(FUNC_NtTerminateProcess,NtTerminateProcess,
	L"ntdll","NtTerminateProcess",TRUE);

typedef DWORD (WINAPI *FUNC_SHRunDialog)(HWND,DWORD,LPVOID,LPVOID,LPVOID,DWORD);
DEFINE_WINAPI(FUNC_SHRunDialog,SHRunDialog,
	L"shell32",(LPCSTR)61,TRUE);







