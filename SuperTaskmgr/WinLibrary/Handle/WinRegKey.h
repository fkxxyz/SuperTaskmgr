
// WinRegKey.h
#pragma once

class CWinRegKey : public CHandleObject<HKEY>{
protected:
	HKEY &m_hKey;

public:
	CWinRegKey():m_hKey(m_Handle){}
	~CWinRegKey(){if (m_hKey) Close();}

	// RegOpenKeyEx
	BOOL Open(
		HKEY hKeyParent,
		LPCTSTR lpszKeyName,
		REGSAM samDesired = KEY_READ | KEY_WRITE
		)
	{
		assert(m_hKey==NULL);
		DWORD dwLastError=RegOpenKeyEx(hKeyParent,lpszKeyName,0,samDesired,&m_hKey);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegCloseKey
	BOOL Close(){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegCloseKey(m_hKey);
		m_hKey=NULL;
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}


	// RegCreateKeyEx
	BOOL Create(
		HKEY hKeyParent,
		LPCTSTR lpszKeyName,
		LPTSTR lpszClass = REG_NONE,
		DWORD dwOptions = REG_OPTION_NON_VOLATILE,
		REGSAM samDesired = KEY_READ | KEY_WRITE,
		LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
		LPDWORD lpdwDisposition = NULL
		)
	{
		assert(m_hKey==NULL);
		DWORD dwLastError=RegCreateKeyEx(
			hKeyParent,
			lpszKeyName,
			0,
			lpszClass,
			dwOptions,
			samDesired,
			lpSecAttr,
			&m_hKey,
			lpdwDisposition
			);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegDeleteValue
	BOOL DeleteValue(LPCTSTR lpszValue){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegDeleteValue(m_hKey,lpszValue);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegDeleteKey
	BOOL DeleteKey(LPCTSTR lpszSubKey){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegDeleteKey(m_hKey,lpszSubKey);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegDeleteTree
	BOOL DeleteTree(LPCTSTR lpszSubKey){
		assert(m_hKey!=NULL);
		assert(0);
		DWORD dwLastError=ERROR_NOT_SUPPORTED;//RegDeleteTree(m_hKey,lpszSubKey);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}


	// RegSaveKey
	BOOL Save(LPCTSTR lpDestFile,LPSECURITY_ATTRIBUTES lpSecAttr=NULL){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegSaveKey(m_hKey,lpDestFile,lpSecAttr);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegRestoreKey
	BOOL Save(LPCTSTR lpSrcFile,DWORD dwFlags=REG_FORCE_RESTORE){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegRestoreKey(m_hKey,lpSrcFile,dwFlags);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}


	// RegFlushKey
	BOOL Flush(){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegFlushKey(m_hKey);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}


	// RegEnumKeyEx
	BOOL EnumKey(
		DWORD iIndex,
		LPTSTR pszName,
		LPDWORD pnNameLength,
		FILETIME* pftLastWriteTime = NULL
		)
	{
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegEnumKeyEx(
			m_hKey,
			iIndex,
			pszName,
			pnNameLength,
			NULL,
			NULL,
			NULL,
			pftLastWriteTime
			);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegNotifyChangeKeyValue
	BOOL NotifyChangeKeyValue(
		BOOL bWatchSubtree,
		DWORD dwNotifyFilter,
		HANDLE hEvent,
		BOOL bAsync = TRUE
		)
	{
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegNotifyChangeKeyValue(
			m_hKey,
			bWatchSubtree,
			dwNotifyFilter,
			hEvent,
			bAsync
			);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}

	// RegQueryValueEx
	BOOL QueryValue(LPCTSTR pszValueName, DWORD* pdwType, void* pData, ULONG* pnBytes){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegQueryValueEx(
			m_hKey,
			pszValueName,
			0,
			pdwType,
			(LPBYTE)pData,
			pnBytes
			);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}
	BOOL QueryDWORDValue(LPCTSTR pszValueName,LPDWORD pdwData){
		ULONG nBytes=sizeof(DWORD);
		DWORD dwType;
		if (!QueryValue(
			pszValueName,
			&dwType,
			(LPBYTE)pdwData,
			&nBytes
			))
			return FALSE;
		if (dwType != REG_DWORD){
			SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}
		return TRUE;
	}
	BOOL QueryQWORDValue(LPCTSTR pszValueName,ULONGLONG *pqwData){
		ULONG nBytes=sizeof(ULONGLONG);
		DWORD dwType;
		if (!QueryValue(
			pszValueName,
			&dwType,
			(LPBYTE)pqwData,
			&nBytes
			))
			return FALSE;
		if (dwType != REG_QWORD){
			SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}
		return TRUE;
	}
	BOOL QueryStringValue(LPCTSTR pszValueName,LPTSTR pszValue,ULONG* pnChars){
		ULONG nBytes=*pnChars*sizeof(TCHAR);
		DWORD dwType;
		if (!QueryValue(
			pszValueName,
			&dwType,
			(LPBYTE)pszValue,
			&nBytes
			))
			return FALSE;
		if(dwType != REG_SZ && dwType != REG_EXPAND_SZ){
			SetLastError(ERROR_INVALID_DATA);
			return FALSE;
		}
		*pnChars=nBytes/sizeof(TCHAR);
		return TRUE;
	}

	// RegSetValueEx
	BOOL SetValue(LPCTSTR pszValueName, DWORD dwType, const void* pValue, ULONG nBytes){
		assert(m_hKey!=NULL);
		DWORD dwLastError=RegSetValueEx(m_hKey, pszValueName, NULL, dwType, (BYTE*)pValue, nBytes);
		SetLastError(dwLastError);
		return dwLastError==ERROR_SUCCESS;
	}
	BOOL SetDWORDValue(LPCTSTR pszValueName, DWORD dwValue){
		return SetValue(pszValueName,REG_DWORD,&dwValue,sizeof(DWORD));
	}
	BOOL SetQWORDValue(LPCTSTR pszValueName, LONGLONG qwValue){
		return SetValue(pszValueName,REG_DWORD,&qwValue,sizeof(LONGLONG));
	}
	BOOL SetStringValue(LPCTSTR pszValueName, LPCTSTR szValue, DWORD dwType=REG_SZ){
		assert(dwType==REG_SZ || dwType==REG_EXPAND_SZ);
		return SetValue(pszValueName,dwType,szValue,((int)_tcslen(szValue)+1)*sizeof(TCHAR));
	}
};



