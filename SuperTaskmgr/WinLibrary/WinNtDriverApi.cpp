
// WinNtDriverApi.cpp

#include "stdafx.h"

#pragma warning(disable: 4073)
#pragma init_seg(lib)

typedef BOOL (WINAPI *FUNC_Wow64DisableWow64FsRedirection)(PVOID *);
DEFINE_WINAPI(FUNC_Wow64DisableWow64FsRedirection,dlWow64DisableWow64FsRedirection,
	L"kernel32","Wow64DisableWow64FsRedirection",FALSE);

typedef BOOL (WINAPI *FUNC_Wow64RevertWow64FsRedirection)(PVOID);
DEFINE_WINAPI(FUNC_Wow64RevertWow64FsRedirection,dlWow64RevertWow64FsRedirection,
	L"kernel32","Wow64RevertWow64FsRedirection",FALSE);


typedef NTSTATUS (NTAPI *FUNC_ZwLoadDriver)(PUNICODE_STRING);
DEFINE_WINAPI(FUNC_ZwLoadDriver,ZwLoadDriver,L"ntdll","ZwLoadDriver",TRUE);

typedef NTSTATUS (NTAPI *FUNC_ZwUnloadDriver)(PUNICODE_STRING);
DEFINE_WINAPI(FUNC_ZwUnloadDriver,ZwUnloadDriver,
	L"ntdll","ZwUnloadDriver",TRUE);




