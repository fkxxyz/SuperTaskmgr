
// WinFile.cpp

#include "stdafx.h"
#include "WinFile.h"

BOOL CAnsiTextFile::ReadLine(LPTSTR lpText,size_t nMaxTextLength=0x1000){
	CLastErrorFunction result;

	//检查参数的合理性
	if (nMaxTextLength<2){
		assert(0);
		result.SetErrorCode(ERROR_INSUFFICIENT_BUFFER);
		goto end;
	}

	size_t nSpaceMem=nMaxTextLength;
#ifdef UNICODE
	CHAR szStaticBuffer[0x1000];
	//申请读取所需的空间
	LPSTR lpAnsiText;
	if (nMaxTextLength>0x1000){
		if (!(lpAnsiText=(LPSTR)new BYTE [nSpaceMem])){
			result.SetErrorCode();
			goto end;
		}
	} else
		lpAnsiText=szStaticBuffer;
	CHAR *lpReadStart=lpAnsiText;
#else
	CHAR *lpReadStart=lpText;
#endif
	nSpaceMem--; //预留 '\0' 结尾符所需内存

	while (1){
		//开始读取
		DWORD nReadSize;
		if (
			!Read(
				lpReadStart,
				(DWORD)(nBaseSize<nSpaceMem?nBaseSize:nSpaceMem),
				&nReadSize
			)
		){
			*lpText=_T('\0');
			result.SetErrorCode();
			goto end1;
		}
		if (!nReadSize){
			*lpReadStart=_T('\0');
			result.SetErrorCode(ERROR_BAD_LENGTH);
			goto end1;
		}

		//寻找换行符
		int i;
		for (i=0;i<(int)nReadSize;i++){
			if (lpReadStart[i]=='\n')
				break;
		}
		if (i<(int)nReadSize){ //若找到换行符 '\n'
			CHAR &c=lpReadStart[i];
			m_FilePointer += i + 1;
			if (!SetPointer(m_FilePointer))
				goto end1;
			c='\0';
			for (CHAR *p=&c-1;p>lpReadStart && *p=='\r';p--)
				*p='\0';
			break; //找到换行符，循环读取结束
		}

		//未找到换行符则准备继续读取
		m_FilePointer += nReadSize;
		lpReadStart+=nReadSize;
		nSpaceMem-=nReadSize;
		if (!nSpaceMem){
			*lpText=_T('\0');
			result.SetErrorCode(ERROR_INSUFFICIENT_BUFFER);
			goto end1;
		}
	}

#ifdef UNICODE
	//将 ANSI 字符转换成 UNICODE
	if (!MultiByteToWideChar(CP_ACP,0,lpAnsiText,-1,lpText,(int)nMaxTextLength)){
		result.SetErrorCode();
		goto end1;
	}

end1:
	if (lpAnsiText != szStaticBuffer)
		delete lpAnsiText;
#else
end1:
#endif

end:
	return result.GetResult();
}

BOOL CAnsiTextFile::WriteText(LPCTSTR lpText){
#ifdef UNICODE
#define MAX_READ_UNICODE 1024
#define MAX_WRITE_ANSI (MAX_READ_UNICODE*4)

	//获取文本长度
	size_t nTextLen=wcslen(lpText);

	//定义循环写入的变量指针、大小
	const WCHAR *pWideChar=lpText;
	int nWideChar;

	//Ansi 字符串缓冲区
	CHAR szAnsiBuffer[MAX_WRITE_ANSI];

	while (nTextLen){
		//得出准备写入多少个 UNICODE 字符
		nWideChar=nTextLen<MAX_READ_UNICODE ? (int)nTextLen : MAX_READ_UNICODE;

		//UNICODE 到 ANSI 转换
		int nAnsi=WideCharToMultiByte(CP_ACP,NULL,pWideChar,nWideChar,szAnsiBuffer,MAX_WRITE_ANSI,NULL,NULL);
		ASSERT(nAnsi);

		//ANSI 写入文件
		if (!CBinFile::Write(szAnsiBuffer,nAnsi,NULL))
			return FALSE;

		//准备下一次写入
		nTextLen-=nWideChar;pWideChar+=nWideChar;
	}
	return TRUE;
#else
	return CBinFile::Write(lpText,(DWORD)strlen(lpText),NULL);
#endif
}


