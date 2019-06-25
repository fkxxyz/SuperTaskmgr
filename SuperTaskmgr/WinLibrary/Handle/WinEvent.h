
// WinEvent.h
#pragma once

class CEvent : public CKernelObject {
protected:
	HANDLE &m_hEvent;

#ifdef _DEBUG
	BOOL bIsSeted;
#endif

public:
	CEvent():m_hEvent(m_Handle){}
	~CEvent(){ if (m_hEvent) Close(); }

	//CreateEvent
	virtual BOOL Create(
		BOOL bManualReset=TRUE,
		BOOL bInitialState=FALSE,
		LPCTSTR lpName=NULL,
		LPSECURITY_ATTRIBUTES lpEventAttributes=NULL
		)
	{
		ASSERT(m_hEvent==NULL);
		m_hEvent = CreateEvent(lpEventAttributes,bManualReset,bInitialState,lpName);
#ifdef _DEBUG
		bIsSeted = bInitialState;
#endif
		return m_hEvent != NULL;
	}

	//OpenEvent
	virtual BOOL Open(LPCTSTR lpName,DWORD dwDesiredAccess=EVENT_ALL_ACCESS,BOOL bInheritHandle=FALSE){
		ASSERT(m_hEvent==NULL);
		m_hEvent = OpenEvent(dwDesiredAccess,bInheritHandle,lpName);
		return m_hEvent != NULL;
	}

	virtual BOOL IsSeted() const {
		ASSERT(m_hEvent!=NULL);
		switch (WaitForSingleObject(m_hEvent,0)){
		case WAIT_OBJECT_0:
#ifdef _DEBUG
			ASSERT(bIsSeted == TRUE);
#endif
			return TRUE;
		case WAIT_TIMEOUT:
#ifdef _DEBUG
			ASSERT(bIsSeted == FALSE);
#endif
			return FALSE;
		default:
			ASSERT(0);
			return FALSE;
		}
	}

	//ResetEvent
	virtual BOOL Reset(){
		ASSERT(m_hEvent!=NULL);
#ifdef _DEBUG
		bIsSeted = FALSE;
#endif
		return ResetEvent(m_hEvent);
	}

	//SetEvent
	virtual BOOL Set(){
		ASSERT(m_hEvent!=NULL);
#ifdef _DEBUG
		bIsSeted = TRUE;
#endif
		return SetEvent(m_hEvent);
	}

	//PulseEvent
	virtual BOOL Pulse(){
		ASSERT(m_hEvent!=NULL);
		return PulseEvent(m_hEvent);
	}
};


