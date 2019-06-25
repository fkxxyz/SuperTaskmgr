
// WinAPI.cpp

#include "stdafx.h"
#include "WinAPI.h"





LPVOID GetAPIAddress(LPCSTR lpModuleName,LPCSTR lpProcName,BOOL bAssertNull){
	HMODULE hModule;
	if (!(hModule=GetModuleHandleA(lpModuleName)) || !(hModule=LoadLibraryA(lpModuleName))){
#ifdef _DEBUG
		DebugPrintA("Error :Can't find the module \"%s\".\n",lpModuleName);
		assert(0);
	}
	FARPROC lpAddress;
	if (!(lpAddress=GetProcAddress(hModule,lpProcName)) && bAssertNull){
		DebugPrintA("Error :Can't find the function \"%s\" in the module \"%s\".\n",
			lpProcName,lpModuleName);
		assert(0);
	}
	return lpAddress;
#else
	}
	return GetProcAddress(hModule,lpProcName);
#endif
}


LPVOID GetAPIAddress(LPCWSTR lpModuleName,LPCSTR lpProcName,BOOL bAssertNull){
	HMODULE hModule;
	if (!(hModule=GetModuleHandleW(lpModuleName)) || !(hModule=LoadLibraryW(lpModuleName))){
#ifdef _DEBUG
		DebugPrintW(L"Error :Can't find the module \"%s\".\n",lpModuleName);
		assert(0);
	}
	FARPROC lpAddress;
	if (!(lpAddress=GetProcAddress(hModule,lpProcName)) && bAssertNull){
		DebugPrintW(L"Error :Can't find the function \"%s\" in the module \"%s\".\n",
			lpProcName,lpModuleName);
		assert(0);
	}
	return lpAddress;
#else
	}
	return GetProcAddress(hModule,lpProcName);
#endif
}


