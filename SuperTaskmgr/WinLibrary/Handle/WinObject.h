
// WinObject.h
#pragma once

template <class HTYPE>
class CHandleObject {
protected:
	HTYPE m_Handle;

public:
	CHandleObject():m_Handle(NULL){}
	explicit CHandleObject(HTYPE handle):m_Handle(NULL){ Attach(handle);}
	explicit CHandleObject(const CHandleObject &object):m_Handle(NULL){ Attach((HTYPE)object); }

	// 必须在子类的析构函数里面调用 Close()，Close() 的功能是关闭句柄，并且设置句柄值为 NULL
	~CHandleObject(){ assert(m_Handle==NULL); }

	//virtual BOOL Open() = 0;
	//virtual BOOL Create() = 0;
	virtual BOOL Close() = 0;

	virtual void Attach(HTYPE handle){
		assert(m_Handle==NULL);
		m_Handle=handle;
	}
	virtual void Detach(){
		assert(m_Handle!=NULL);
		m_Handle=NULL;
	}

	operator HTYPE() const { return m_Handle; }
	BOOL operator ==(CHandleObject &object) const { return m_Handle==object; }
	CHandleObject &operator =(const CHandleObject &object){ Attach(object); return *this; }
};

class CKernelObject : public CHandleObject<HANDLE>{
public:
	~CKernelObject(){
		if (m_Handle)
			Close();
	}


	//virtual BOOL Open() = 0;
	//virtual BOOL Create() = 0;

	// CloseHandle
	virtual BOOL Close(){
		assert(m_Handle!=NULL);
		if (!CloseHandle(m_Handle)) return FALSE;
		m_Handle=NULL;
		return TRUE;
	}

	// WaitForSingleObject
	virtual BOOL Wait(DWORD dwMilliseconds=INFINITE) const {
		assert(m_Handle!=NULL);
		DWORD dwWaitResult=WaitForSingleObject(m_Handle,dwMilliseconds);
		if (dwWaitResult==WAIT_TIMEOUT) SetLastError(ERROR_TIMEOUT);
		assert(dwWaitResult != WAIT_FAILED);
		return dwWaitResult==WAIT_OBJECT_0;
	}
};


