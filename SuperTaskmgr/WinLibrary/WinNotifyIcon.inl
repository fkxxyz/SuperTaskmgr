

// WinNotifyIcon.inl
#pragma once


inline CNotifyIcon::CNotifyIcon(HWND hWnd){
	m_Data.hWnd=hWnd;
	m_Data.cbSize=sizeof(NOTIFYICONDATA);
}

inline CNotifyIcon::~CNotifyIcon(){
}


inline void CNotifyIcon::SetHWnd(HWND hWnd){
	assert(IsWindow(hWnd));
	m_Data.hWnd=hWnd;
}

inline BOOL CNotifyIcon::Add(
	UINT uID,
	UINT uCallbackMessage,
	HICON hIcon,
	LPCTSTR szTip,
	PBALLON_TIP pBallonTip,
	DWORD dwState,
	DWORD dwStateMask
	)
{
	SetData(
		uID,
		uCallbackMessage,
		hIcon,
		szTip,
		pBallonTip,
		dwState,
		dwStateMask
		);
	return Shell_NotifyIcon(NIM_ADD,&m_Data);
}

inline BOOL CNotifyIcon::Modify(
	UINT uID,
	UINT uCallbackMessage,
	HICON hIcon,
	LPCTSTR szTip,
	PBALLON_TIP pBallonTip,
	DWORD dwState,
	DWORD dwStateMask
	)
{
	SetData(
		uID,
		uCallbackMessage,
		hIcon,
		szTip,
		pBallonTip,
		dwState,
		dwStateMask
		);
	return Shell_NotifyIcon(NIM_MODIFY,&m_Data);
}

inline BOOL CNotifyIcon::Delete(UINT uID){
	assert(IsWindow(m_Data.hWnd));
	m_Data.uID=uID;
	m_Data.uFlags=0;
	return Shell_NotifyIcon(NIM_DELETE,&m_Data);
}

inline void CNotifyIcon::SetData(
	UINT uID,
	UINT uCallbackMessage,
	HICON hIcon,
	LPCTSTR szTip,
	PBALLON_TIP pBallonTip,
	DWORD dwState,
	DWORD dwStateMask
	)
{
	assert(IsWindow(m_Data.hWnd));
	m_Data.uID=uID;
	m_Data.uFlags = 0;
	if (uCallbackMessage){
		m_Data.uFlags |= NIF_MESSAGE;
		m_Data.uCallbackMessage=uCallbackMessage;
	}
	if (hIcon){
		m_Data.uFlags |= NIF_ICON;
		m_Data.hIcon=hIcon;
	}
	if (szTip){
		m_Data.uFlags |= NIF_TIP;
		_tcscpy_s<128>(m_Data.szTip,szTip);
	}
	if (pBallonTip){
		m_Data.uFlags |= NIF_INFO;
		_tcscpy_s<256>(m_Data.szInfo,pBallonTip->szInfo);
		_tcscpy_s<64>(m_Data.szInfoTitle,pBallonTip->szInfoTitle);
		m_Data.dwInfoFlags=pBallonTip->dwInfoFlags;
		hBalloonIcon=pBallonTip->hBalloonIcon;
#if (NTDDI_VERSION >= NTDDI_XPSP2 && (_WIN32_IE >= 0x0600))
		assert(
			(m_Data.dwInfoFlags==NIIF_USER && hBalloonIcon) ||
			(m_Data.dwInfoFlags!=NIIF_USER && hBalloonIcon==NULL)
			);
#else
		assert(m_Data.dwInfoFlags!=0x00000004);
#endif
	}
	if (dwState){
		m_Data.uFlags |= NIF_STATE;
		m_Data.dwState=dwState;
		m_Data.dwStateMask=dwStateMask;
		assert((m_Data.dwState | m_Data.dwStateMask) != m_Data.dwStateMask);
	}
}





inline void CNotifyIconThread::SetHWnd(HWND hWnd){
	m_NotifyIcon.SetHWnd(hWnd);
}

inline BOOL CNotifyIconThread::Add(
	UINT uID,
	UINT uCallbackMessage,
	HICON hIcon,
	LPCTSTR szTip,
	PBALLON_TIP pBallonTip,
	DWORD dwState,
	DWORD dwStateMask
	)
{
	if (!SetData(
		uID,
		uCallbackMessage,
		hIcon,
		szTip,
		pBallonTip,
		dwState,
		dwStateMask
		))
		return FALSE;
	return CMsgThread::PostMessage(WM_NOTIFY_ICON_ADD);
}

inline BOOL CNotifyIconThread::Modify(
	UINT uID,
	UINT uCallbackMessage,
	HICON hIcon,
	LPCTSTR szTip,
	PBALLON_TIP pBallonTip,
	DWORD dwState,
	DWORD dwStateMask
	)
{
	if (!SetData(
		uID,
		uCallbackMessage,
		hIcon,
		szTip,
		pBallonTip,
		dwState,
		dwStateMask
		))
		return FALSE;
	return CMsgThread::PostMessage(WM_NOTIFY_ICON_MODIFY);
}

inline BOOL CNotifyIconThread::Delete(UINT uID){
	return CMsgThread::PostMessage(WM_NOTIFY_ICON_DELETE,(WPARAM)uID);
}

inline BOOL CNotifyIconThread::SetData(
	UINT uID,
	UINT uCallbackMessage,
	HICON hIcon,
	LPCTSTR szTip,
	PBALLON_TIP pBallonTip,
	DWORD dwState,
	DWORD dwStateMask
	)
{
	if (CMsgThread::IsBusy() || bPending){
		SetLastError(ERROR_BUSY);
		return FALSE;
	}
	m_NotifyIcon.SetData(
		uID,
		uCallbackMessage,
		hIcon,
		szTip,
		pBallonTip,
		dwState,
		dwStateMask
		);
	bPending=FALSE;
	return TRUE;
}

inline void CNotifyIconThread::OnMsg(UINT message, WPARAM wParam, LPARAM lParam, PCALLBACK_MSG pResultMsg){
	dwLastErrorCode = 0;

	switch (message){
	case WM_NOTIFY_ICON_ADD:
		if (!Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon.m_Data)){
			dwLastErrorCode=GetLastError();
		}
		break;

	case WM_NOTIFY_ICON_MODIFY:
		if (!Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIcon.m_Data)){
			dwLastErrorCode=GetLastError();
		}
		break;

	case WM_NOTIFY_ICON_DELETE:
		m_NotifyIcon.m_Data.uID=(UINT)wParam;
		m_NotifyIcon.m_Data.uFlags=0;
		if (!Shell_NotifyIcon(NIM_DELETE,&m_NotifyIcon.m_Data)){
			dwLastErrorCode=GetLastError();
		}
		break;

	}

}




