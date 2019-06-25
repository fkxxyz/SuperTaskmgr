
#pragma once

template <class TYPE>
inline int cmp(TYPE &x1,TYPE &x2){
	if (x1==x2)
		return 0;
	if (x1>x2)
		return 1;
	if (x1<x2)
		return -1;
	return 0;
}

inline void SetWidth(LPTSTR szStr,TCHAR cFillChar,int xWidth){
	register TCHAR *i;
	for (i=szStr;i<szStr+xWidth && *i;i++);
	for (;i<szStr+xWidth;i++)
		*i=cFillChar;
	*i=0;
}

inline void CheckFormatPath(LPTSTR szPath,LPTSTR lpszSystemRoot){
	INT_PTR i,l=_tcslen(szPath);
	TCHAR szBuf[MAX_PATH];
	if (szPath[0]==_T('\\')){
		if (szPath[1]==_T('?') && szPath[2]==_T('?') && szPath[3]==_T('\\')){
			for (i=0;i<l-3;i++)
				szPath[i]=szPath[i+4];
		}
		if (!_tcsnicmp(szPath+1,_T("SystemRoot"),10)){
			_stprintf_s(szBuf,MAX_PATH,_T("%s\\%s"),lpszSystemRoot,szPath+11);
			_tcscpy_s(szPath,MAX_PATH,szBuf);
		}
	}
}

inline void ConvLLNetSpeedStr(LPTSTR szBuffer,LONGLONG qNetSpeed){
	FLOAT fItem=(FLOAT)qNetSpeed;
	if (fItem<0.0f){
		szBuffer[0]=_T('-');
		szBuffer[1]=_T('\0');
	} else if (fItem<1000.0f)
		_stprintf_s(szBuffer,32,_T("%.0f B/S"),fItem);
	else {
		fItem/=1024.0f;
		if (fItem<1000.0f)
			_stprintf_s(szBuffer,32,_T("%.1f K/S"),fItem);
		else {
			fItem/=1024.0f;
			if (fItem<1000.0f)
				_stprintf_s(szBuffer,32,_T("%.1f M/S"),fItem);
			else {
				fItem/=1024.0f;
				if (fItem<1000.0f)
					_stprintf_s(szBuffer,32,_T("%.1f G/S"),fItem);
				else {
					fItem/=1024.0f;
					if (fItem<1000.0f)
						_stprintf_s(szBuffer,32,_T("%.1f T/S"),fItem);
				}
			}
		}
	}
}

