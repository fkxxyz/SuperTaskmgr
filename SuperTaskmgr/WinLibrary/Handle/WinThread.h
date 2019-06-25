
// WinThread.h
#pragma once

class CThread : public CKernelObject{
protected:
	HANDLE &m_hThread;
	DWORD m_dwThreadID;

public:
	CThread():m_hThread(m_Handle),m_dwThreadID(0){};
	~CThread(){if (m_hThread) VERIFY(Terminate());}

	DWORD GetID() const {
		ASSERT(m_hThread!=NULL);
		ASSERT(m_dwThreadID!=NULL); // 只有用 Create 创建的线程才支持 GetID()
		return m_dwThreadID;
	}

	virtual BOOL Open(DWORD dwThreadID,DWORD dwDesiredAccess=THREAD_ALL_ACCESS,BOOL bInheritHandle=FALSE){
		ASSERT(m_hThread==NULL);
		m_hThread = OpenThread(dwDesiredAccess,bInheritHandle,m_dwThreadID);
		m_dwThreadID=dwThreadID;
		return m_hThread != NULL;
	}

	virtual BOOL Create(LPVOID lpParameter,LPTHREAD_START_ROUTINE lpStartAddress,BOOL bSuspended=FALSE,SIZE_T dwStackSize=0){
		ASSERT(m_hThread==NULL);
#ifdef _DEBUG
		if (!(m_hThread = CreateThread(
			NULL,
			dwStackSize,
			lpStartAddress,
			lpParameter,
			CREATE_SUSPENDED,
			&m_dwThreadID
			)
			)
			)
			return FALSE;
		DebugPrint(_T("线程 'Win32 线程' (0x%x) 已启动\n"),m_dwThreadID);

		if (!bSuspended)
			VERIFY(Resume());
		return TRUE;
#else
		return (
			m_hThread = CreateThread(
				NULL,
				dwStackSize,
				lpStartAddress,
				lpParameter,
				bSuspended ? CREATE_SUSPENDED : 0,
				&m_dwThreadID
				)
				) != NULL;
#endif
	}
	virtual BOOL Terminate(DWORD dwExitCode=1){
		ASSERT(m_hThread!=NULL);
		return TerminateThread(m_hThread,dwExitCode);
	}
	virtual BOOL Suspend(){
		ASSERT(m_hThread);
		return SuspendThread(m_hThread);
	}
	virtual BOOL Resume(){
		ASSERT(m_hThread);
		return ResumeThread(m_hThread);
	}
	virtual BOOL GetExitCode(LPDWORD lpExitCode) const {
		ASSERT(m_hThread);
		return GetExitCodeThread(m_hThread,lpExitCode);
	}
	virtual BOOL StillActive() const {
		DWORD dwExitCode;
		return
			GetExitCode(&dwExitCode) &&
			dwExitCode==STILL_ACTIVE;
	}

};

class CThreadProc : public CThread {
protected:
	typedef struct _PARAM_THIS {
		LPVOID lpParam;
		LPVOID pThis;
	} PARAM_THIS, *PPARAM_THIS;

	virtual DWORD ThreadProc(LPVOID) = 0;

private:
	PARAM_THIS stParamThis;

	static DWORD WINAPI StartProc(LPVOID);

public:
	virtual BOOL Create(LPVOID lpParameter,BOOL bSuspended = FALSE, SIZE_T dwStackSize = 0){
		stParamThis.lpParam=lpParameter;
		stParamThis.pThis=(LPVOID)this;
		return CThread::Create(&stParamThis,&StartProc,bSuspended,dwStackSize);
	}
};


//回调消息
typedef struct _CALLBACK_MSG {
	HWND hwnd; //窗口句柄
	DWORD dwThreadID; //如果窗口句柄为 NULL 则指定此线程 ID 来回调线程消息
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
} CALLBACK_MSG, *PCALLBACK_MSG;


class CMsgThread : public CThreadProc {
private:
	CEvent MsgFinished;

protected:
	virtual DWORD ThreadProc(LPVOID);
	virtual void OnMsg(UINT message, WPARAM wParam, LPARAM lParam, PCALLBACK_MSG pResultMsg) = 0;
	static BOOL PostCallBackMessage(PCALLBACK_MSG msg);

public:

	virtual BOOL Create(PCALLBACK_MSG pCallBackFinishedMsg=NULL,SIZE_T dwStackSize=0);

	virtual BOOL WaitForMessageFinished(DWORD dwMilliseconds=INFINITE) const {
		ASSERT((HANDLE)MsgFinished);
		return MsgFinished.Wait(dwMilliseconds);
	}
	virtual BOOL IsBusy() const {
		if ((HANDLE)MsgFinished==NULL) return FALSE;
		return !MsgFinished.IsSeted();
	}
	virtual BOOL PostMessage(UINT Msg,WPARAM wParam = 0,LPARAM lParam = 0){
		ASSERT(m_hThread);
		ASSERT(m_dwThreadID);
		MsgFinished.Reset();
		if (!PostThreadMessage(m_dwThreadID,Msg,wParam,lParam)){
			DWORD dwLastError=GetLastError();
			ASSERT(0);
			return FALSE;
		}
		return TRUE;
	}
	virtual BOOL PostQuitMessage(){
		return PostMessage(WM_QUIT);
	}
};




