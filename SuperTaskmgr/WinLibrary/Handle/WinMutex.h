
// WinMutex.h
#pragma once

class CMutex : public CKernelObject{
protected:
	HANDLE &m_hMutex;

public:
	inline CMutex():m_hMutex(m_Handle){};
	inline ~CMutex(){if (m_hMutex) Close();};

	//CreateMutex
	virtual inline BOOL Create(BOOL bInitialOwner,LPCTSTR lpName){
		ASSERT(m_hMutex==NULL);
		return (m_hMutex=CreateMutex(NULL,bInitialOwner,lpName)) != NULL;
	}

	//OpenMutex
	virtual inline BOOL Open(LPCTSTR lpName,BOOL bInheritHandle=FALSE){
		ASSERT(m_hMutex==NULL);
		return (m_hMutex=OpenMutex(MUTEX_ALL_ACCESS,bInheritHandle,lpName)) != NULL;
	}

	//WaitForSingleObject
	virtual inline BOOL Wait(DWORD dwMilliseconds=INFINITE){
		ASSERT(m_hMutex!=NULL);
		switch (WaitForSingleObject(m_hMutex,dwMilliseconds)){
		case WAIT_OBJECT_0:
			return TRUE;
		case WAIT_TIMEOUT:
			SetLastError(ERROR_TIMEOUT);
		default:
			return FALSE;
		}
	}

	//ReleaseMutex
	virtual inline BOOL Release(){
		ASSERT(m_hMutex!=NULL);
		return ReleaseMutex(m_hMutex);
	}

	//CloseHandle
	virtual BOOL Close(){
		ASSERT(m_hMutex!=NULL);
		if (!CloseHandle(m_hMutex)) return FALSE;
		m_hMutex=NULL;
		return TRUE;
	}
};


