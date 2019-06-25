
// WinPEFileInfo.cpp

#include "stdafx.h"
#include "WinPEFileInfo.h"
#pragma comment(lib, "version.lib")


#define MAX_VERSION_INFO_SIZE (100*1024)
BOOL GetPeFileVersionInfoItem(LPCTSTR szFilePath,LPCWSTR szKey,LPTSTR szInfoBuffer,int *cchInfoChar){

	//获取版本信息长度
	DWORD dwSize;
	dwSize=GetFileVersionInfoSize(szFilePath,NULL);
	if (!dwSize)
		return FALSE;

	//检查版本信息长度
	if (dwSize>MAX_VERSION_INFO_SIZE){
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		assert(0);
		return FALSE;
	}

	//获取版本信息
	BYTE pVersionInfoBuf[100*1024]; // 100 kb
	if (!GetFileVersionInfo(szFilePath,NULL,(DWORD)dwSize,pVersionInfoBuf))
		return FALSE;

	return GetVersionItem(pVersionInfoBuf,dwSize,szKey,szInfoBuffer,cchInfoChar);
}


BOOL GetAppVersion(PVERSION_NUMBER pVersionNumber){
	TCHAR szVersion[64];
	int cchChar=64;
	if (!GetAppVersionString(szVersion,&cchChar)){
		assert(0);
		return FALSE;
	}

	_stscanf_s(szVersion,_T("%d.%d.%d.%d"),
		&pVersionNumber->dwMajor,
		&pVersionNumber->dwMinor,
		&pVersionNumber->dwBuild,
		&pVersionNumber->dwRevision
		);
	return TRUE;
}



