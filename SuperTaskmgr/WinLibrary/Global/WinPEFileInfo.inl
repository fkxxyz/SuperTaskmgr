
// WinPEFileInfo.inl


inline HICON GetFileIcon(LPCTSTR szFilePath){
	SHFILEINFO sh_info;
	if (szFilePath[0]==_T('\0'))
		return NULL;
	if (!SHGetFileInfo(szFilePath, 0, &sh_info, sizeof(sh_info), SHGFI_ICON | SHGFI_SMALLICON))
		return NULL;
	return sh_info.hIcon;
}

//在长度为ulSrcLen的btSrc宽字符串中查找szStr
inline LPWSTR wbtstr(BYTE *btSrc,size_t xSrcLen,LPCWSTR szStr,size_t xLen){
	ULONG_PTR i,j,x;
	if (xSrcLen<xLen) return NULL;
	x=xSrcLen-xLen;

	for (i=0;i<=x;i++){
		for (j=0;j<xLen;j++)
			if (((LPWSTR)btSrc)[i+j]!=szStr[j])
				break;
		if (j==xLen)
			return ((LPWSTR)btSrc)+i;
	}
	return NULL;
}

inline BOOL GetVersionItem(LPVOID pVersionInfo,DWORD dwInfoSize,LPCWSTR szKey,LPTSTR szResultBuffer,int *cchResultChar){

	//从版本信息中查找 szKey
	size_t ulKeyLen=wcslen(szKey);
	WCHAR szFindKey[64];
	swprintf_s(szFindKey,64,L"\1%s",szKey);
	LPWSTR szwInfoBuffer=wbtstr((BYTE *)pVersionInfo,(DWORD)dwInfoSize/2,szFindKey,ulKeyLen+2);
	if (!szwInfoBuffer){//如果未找到
		SetLastError(ERROR_NOT_FOUND);
		return FALSE;
	}

	//得出 szwInfoBuffer 就是目标值的宽字符串
	szwInfoBuffer+=ulKeyLen+2;
	while (!*szwInfoBuffer)
		szwInfoBuffer++;

	DWORD dwSize=(DWORD)wcslen(szwInfoBuffer); //目标值的长度
	//复制目标值到指定缓冲区
#ifdef UNICODE
	//防止内存溢出
	if (dwSize>(DWORD)*cchResultChar){
		*cchResultChar=dwSize;
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}
	wcscpy_s(szResultBuffer,*cchResultChar,szwInfoBuffer);
#else
	if (!WideCharToMultiByte(CP_ACP,0,szwInfoBuffer,(int)dwSize+1,szResultBuffer,*cchResultChar,"?",NULL)){
		if (GetLastError()==ERROR_INSUFFICIENT_BUFFER){
			*cchResultChar=WideCharToMultiByte(CP_ACP,0,szwInfoBuffer,(int)dwSize*2,0,0,"?",NULL);
			SetLastError(ERROR_INSUFFICIENT_BUFFER);
		}
		return FALSE;
	}
#endif

	return TRUE;
}

inline BOOL GetAppVersionString(LPTSTR szResultBuffer,int *cchResultChar){

	//获取当前应用程序的路径
	TCHAR szAppPath[MAX_PATH];
	if (::GetModuleFileName(NULL,szAppPath,MAX_PATH) == 0) return FALSE;

	return GetPeFileVersionInfoItem(szAppPath,L"ProductVersion",szResultBuffer,cchResultChar);
}





