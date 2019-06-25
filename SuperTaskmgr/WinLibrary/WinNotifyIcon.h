
// WinNotifyIcon.h

#pragma once

typedef struct _BALLON_TIP {
	LPTSTR szInfo;
	LPTSTR szInfoTitle;
	DWORD dwInfoFlags;
	HICON hBalloonIcon;
} BALLON_TIP, *PBALLON_TIP;


class CNotifyIcon {
	struct {
		NOTIFYICONDATA m_Data;
		HICON hBalloonIcon;
	};

public:
	CNotifyIcon(HWND hWnd=NULL);
	~CNotifyIcon();

	void SetHWnd(HWND hWnd);

	BOOL Add(
		UINT uID,
		UINT uCallbackMessage=0,
		HICON hIcon=NULL,
		LPCTSTR szTip=NULL,
		PBALLON_TIP pBallonTip=NULL,
		DWORD dwState=0,
		DWORD dwStateMask=0
		);

	BOOL Modify(
		UINT uID,
		UINT uCallbackMessage=0,
		HICON hIcon=NULL,
		LPCTSTR szTip=NULL,
		PBALLON_TIP pBallonTip=NULL,
		DWORD dwState=0,
		DWORD dwStateMask=0
		);

	BOOL Delete(UINT uID);

private:
	void SetData(
		UINT uID,
		UINT uCallbackMessage=0,
		HICON hIcon=NULL,
		LPCTSTR szTip=NULL,
		PBALLON_TIP pBallonTip=NULL,
		DWORD dwState=0,
		DWORD dwStateMask=0
		);

	friend class CNotifyIconThread;
};


#define WM_NOTIFY_ICON_ADD (WM_USER + 1)
#define WM_NOTIFY_ICON_MODIFY (WM_USER + 2)
#define WM_NOTIFY_ICON_DELETE (WM_USER + 3)

class CNotifyIconThread : public CMsgThread {
	CNotifyIcon m_NotifyIcon;

	void OnMsg(UINT message, WPARAM wParam, LPARAM lParam, PCALLBACK_MSG pResultMsg);

	BOOL bPending;

	DWORD dwLastErrorCode;

public:
	CNotifyIconThread::CNotifyIconThread():bPending(FALSE){}

	void SetHWnd(HWND hWnd);

	DWORD GetErrorCode() const { return dwLastErrorCode; }

	BOOL Add(
		UINT uID,
		UINT uCallbackMessage=0,
		HICON hIcon=NULL,
		LPCTSTR szTip=NULL,
		PBALLON_TIP pBallonTip=NULL,
		DWORD dwState=0,
		DWORD dwStateMask=0
		);

	BOOL Modify(
		UINT uID,
		UINT uCallbackMessage=0,
		HICON hIcon=NULL,
		LPCTSTR szTip=NULL,
		PBALLON_TIP pBallonTip=NULL,
		DWORD dwState=0,
		DWORD dwStateMask=0
		);

	BOOL Delete(UINT uID);

private:
	BOOL SetData(
		UINT uID,
		UINT uCallbackMessage=0,
		HICON hIcon=NULL,
		LPCTSTR szTip=NULL,
		PBALLON_TIP pBallonTip=NULL,
		DWORD dwState=0,
		DWORD dwStateMask=0
		);
};







#include "WinNotifyIcon.inl"





