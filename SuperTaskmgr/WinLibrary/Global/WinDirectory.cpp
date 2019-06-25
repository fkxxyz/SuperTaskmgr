
// WinDirectory.cpp

#include "stdafx.h"
#include "WinDirectory.h"

BOOL DeviceDosPathToDosPath(LPCTSTR szDeviceDosPath, LPTSTR szDosPath){
	TCHAR szLogicalDriveStrings[256];
	DWORD nLen = GetLogicalDriveStrings(256, szLogicalDriveStrings);
	return TRUE;
}

BOOL NtPathToDosPath(LPCTSTR szNtPath){
	return FALSE;
}

BOOL FormatToAbsolutePath(LPTSTR szPath){
	return FALSE;
}



