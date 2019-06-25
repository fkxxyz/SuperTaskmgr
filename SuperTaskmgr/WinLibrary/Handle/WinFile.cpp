
// WinFile.cpp

#include "stdafx.h"
#include "WinFile.h"

BOOL CAnsiTextFile::ReadLine(LPTSTR lpText,size_t nMaxTextLength=0x1000){
	CLastErrorFunction result;

	//�������ĺ�����
	if (nMaxTextLength<2){
		assert(0);
		result.SetErrorCode(ERROR_INSUFFICIENT_BUFFER);
		goto end;
	}

	size_t nSpaceMem=nMaxTextLength;
#ifdef UNICODE
	CHAR szStaticBuffer[0x1000];
	//�����ȡ����Ŀռ�
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
	nSpaceMem--; //Ԥ�� '\0' ��β�������ڴ�

	while (1){
		//��ʼ��ȡ
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

		//Ѱ�һ��з�
		int i;
		for (i=0;i<(int)nReadSize;i++){
			if (lpReadStart[i]=='\n')
				break;
		}
		if (i<(int)nReadSize){ //���ҵ����з� '\n'
			CHAR &c=lpReadStart[i];
			m_FilePointer += i + 1;
			if (!SetPointer(m_FilePointer))
				goto end1;
			c='\0';
			for (CHAR *p=&c-1;p>lpReadStart && *p=='\r';p--)
				*p='\0';
			break; //�ҵ����з���ѭ����ȡ����
		}

		//δ�ҵ����з���׼��������ȡ
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
	//�� ANSI �ַ�ת���� UNICODE
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

	//��ȡ�ı�����
	size_t nTextLen=wcslen(lpText);

	//����ѭ��д��ı���ָ�롢��С
	const WCHAR *pWideChar=lpText;
	int nWideChar;

	//Ansi �ַ���������
	CHAR szAnsiBuffer[MAX_WRITE_ANSI];

	while (nTextLen){
		//�ó�׼��д����ٸ� UNICODE �ַ�
		nWideChar=nTextLen<MAX_READ_UNICODE ? (int)nTextLen : MAX_READ_UNICODE;

		//UNICODE �� ANSI ת��
		int nAnsi=WideCharToMultiByte(CP_ACP,NULL,pWideChar,nWideChar,szAnsiBuffer,MAX_WRITE_ANSI,NULL,NULL);
		ASSERT(nAnsi);

		//ANSI д���ļ�
		if (!CBinFile::Write(szAnsiBuffer,nAnsi,NULL))
			return FALSE;

		//׼����һ��д��
		nTextLen-=nWideChar;pWideChar+=nWideChar;
	}
	return TRUE;
#else
	return CBinFile::Write(lpText,(DWORD)strlen(lpText),NULL);
#endif
}


