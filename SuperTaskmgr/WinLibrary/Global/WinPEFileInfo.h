

// WinPEFileInfo.cpp
#pragma once

typedef struct _VERSION_NUMBER {
	DWORD dwMajor,dwMinor,dwBuild,dwRevision;
} VERSION_NUMBER, *PVERSION_NUMBER;



HICON GetFileIcon(LPCTSTR szFilePath);
BOOL GetPeFileVersionInfoItem(LPCTSTR szFilePath,LPCWSTR szKey,LPTSTR szInfoBuffer,int *cchInfoChar);
BOOL GetVersionItem(LPVOID pVersionInfo,DWORD dwInfoSize,LPCWSTR szKey,LPTSTR szResultBuffer,int *cchResultChar);

BOOL GetAppVersionString(LPTSTR szResultBuffer,int *cchResultChar);
BOOL GetAppVersion(PVERSION_NUMBER pVersionNumber);

#include "WinPEFileInfo.inl"


