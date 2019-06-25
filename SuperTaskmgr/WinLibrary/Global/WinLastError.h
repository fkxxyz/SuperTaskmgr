
// WinLastError.h

#pragma once

class CLastErrorFunction {
	BOOL bResult;
	DWORD dwLastError;
public:
	inline CLastErrorFunction():bResult(TRUE){}
	inline ~CLastErrorFunction(){if (!bResult) SetLastError(dwLastError);}

	inline void SetErrorCode(DWORD dwErrorCode=-1){
		dwLastError = (dwErrorCode==-1) ? GetLastError() : dwErrorCode;
		bResult = dwLastError == 0;
	}
	inline BOOL GetResult(){return bResult;}
	inline operator BOOL(){return bResult;}
};

inline LPTSTR GetLastErrorDescription(LPTSTR lpBuffer,DWORD nSize,DWORD dwLastErrorCode){
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwLastErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);
	_tcscpy_s(lpBuffer,nSize,(LPTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return lpBuffer;
}

inline LPTSTR GetLastErrorDescription(LPTSTR lpBuffer,DWORD nSize){
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);
	_tcscpy_s(lpBuffer,nSize,(LPTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	return lpBuffer;
}

inline void ShowError(HWND hWnd,LPCTSTR szTitle,LPCTSTR szFnString1,LPCTSTR szFnString2,DWORD dwErrorCode,UINT uType = MB_OK | MB_ICONWARNING){
	TCHAR szMsg[1024];
	TCHAR szErrorDescription[1024];
	_stprintf_s<1024>(
		szMsg,
		_T("´íÎó´úÂë %d\n\n%s%s%s"),
		dwErrorCode,
		szFnString1 ? szFnString1 : _T(""),
		GetLastErrorDescription(szErrorDescription,1024,dwErrorCode),
		szFnString2 ? szFnString2 : _T("")
		);
	MessageBox(hWnd,szMsg,szTitle,uType);
}



