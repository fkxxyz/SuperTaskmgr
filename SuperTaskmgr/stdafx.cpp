// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// WinLib.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������


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







