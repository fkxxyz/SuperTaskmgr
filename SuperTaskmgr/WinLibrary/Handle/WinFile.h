
// WinFile.h
#pragma once

typedef union _HL_LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} HL_LARGE_INTEGER;

class CBinFile : public CKernelObject {
protected:
	HANDLE &m_hFile;

public:
	CBinFile():m_hFile(m_Handle){}

	// CreateFile
	virtual BOOL OpenRead(LPCTSTR szFilePath){
		m_hFile = CreateFile(
			szFilePath,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (m_hFile==INVALID_HANDLE_VALUE)
			m_hFile=NULL;
		return m_hFile!=NULL;
	}

	// CreateFile
	virtual BOOL OpenWrite(LPCTSTR szFilePath,BOOL bAppend=FALSE){
		m_hFile = CreateFile(
			szFilePath,
			bAppend ? FILE_APPEND_DATA : GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (m_hFile==INVALID_HANDLE_VALUE)
			m_hFile=NULL;
		return m_hFile!=NULL;
	}

	virtual DWORD SetPointer(LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod){
		assert(m_hFile);
		return SetFilePointer(m_hFile, lDistanceToMove, lpDistanceToMoveHigh, dwMoveMethod);
	}
	virtual BOOL SetPointer(LONG value, DWORD dwMoveMethod){
		return SetPointer(value, NULL, dwMoveMethod) != HFILE_ERROR;
	}
	virtual BOOL SetPointer(LONG value){
		DWORD dwPointer;
		while (1){
			dwPointer = SetPointer(value, NULL, FILE_BEGIN);
			if (dwPointer == HFILE_ERROR) return FALSE;
			if (dwPointer == SetPointer(0, NULL, FILE_CURRENT))
				break;
			TRACE(_T("文件指针设置时出错，已重试。\n"));
		}
		return TRUE;
	}
	virtual DWORD GetPointer(){
		return SetPointer(0, NULL, FILE_CURRENT);
	}
	virtual BOOL SetOffsetPointer(LONG value){
		return SetPointer(value, FILE_CURRENT) != HFILE_ERROR;
	}

	// GetFileSize
	virtual DWORD GetSize(LPDWORD lpFileSizeHigh = NULL){
		assert(m_hFile);
		return GetFileSize(m_hFile,lpFileSizeHigh);
	}

	// ReadFile
	virtual BOOL Read(LPVOID lpBuffer,DWORD nNumberOfBytesToRead,LPDWORD lpNumberOfBytesRead=NULL){
		assert(m_hFile);
		return ReadFile(m_hFile,lpBuffer,nNumberOfBytesToRead,lpNumberOfBytesRead,NULL);
	}

	// WriteFile
	virtual BOOL Write( LPCVOID lpBuffer,DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten=NULL){
		assert(m_hFile);
		DWORD dwNumberOfBytesWritten;
		if (lpNumberOfBytesWritten==NULL)
			lpNumberOfBytesWritten=&dwNumberOfBytesWritten;
		return WriteFile(m_hFile,lpBuffer,nNumberOfBytesToWrite,lpNumberOfBytesWritten,NULL);
	}
};


class CAnsiTextFile : public CBinFile {
	LONG m_FilePointer;
	size_t nBaseSize;

public:
	CAnsiTextFile(size_t nBufferSize=0x1000):nBaseSize(nBufferSize){}

	void SetBaseSize(size_t value){nBaseSize=value;}

	BOOL OpenRead(LPCTSTR lpFileName){
		m_FilePointer = 0;
		return CBinFile::OpenRead(lpFileName);
	}
	BOOL ReadLine(LPTSTR lpText,size_t nMaxLength);
	BOOL WriteText(LPCTSTR lpText);
};



