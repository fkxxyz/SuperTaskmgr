
// WinDirectory.h
#pragma once

BOOL DeviceDosPathToNtPath(LPCTSTR szDeviceDosPath, LPTSTR szNtPath);

inline BOOL ExistFile(LPCTSTR szFilePath){
	register DWORD attrib=GetFileAttributes(szFilePath);
	if (attrib == INVALID_FILE_ATTRIBUTES){
		DWORD dwLastError=GetLastError();
		if (dwLastError==ERROR_FILE_NOT_FOUND || ERROR_PATH_NOT_FOUND)
			return FALSE;
		assert(0);
		return FALSE;
	} else
		return (attrib & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

inline BOOL ExistFolder(LPCTSTR szFolderPath){
	register DWORD attrib=GetFileAttributes(szFolderPath);
	return
		(attrib != INVALID_FILE_ATTRIBUTES) &&
		(attrib & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

inline BOOL ExistPath(LPCTSTR szPath){
	return GetFileAttributes(szPath) != INVALID_FILE_ATTRIBUTES;
}

inline BOOL DemandPath(LPCTSTR szFolderPath){
	TCHAR path[MAX_PATH];
	static const TCHAR SEP=_T('\\');

	const TCHAR *fp=szFolderPath;
	TCHAR *p=path;
	while(1) {
		if (*fp == SEP || *fp == _T('\0')){
			*p=_T('\0');
			if (!ExistFolder(path) && !CreateDirectory(path,NULL))
				return FALSE;
			if (*fp == _T('\0'))
				break;
		}

		*p=*fp;
		fp++;p++;
	}
	return TRUE;
}




