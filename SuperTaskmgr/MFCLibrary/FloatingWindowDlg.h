
// FloatingWindowDlg.h

#pragma once


class CFloatingWindowThread : public CThreadProc {
protected:
	DWORD CThreadProc::ThreadProc(LPVOID);
};

class CFloatingWindowDlg : public CDialog {
	DECLARE_DYNAMIC(CFloatingWindowDlg)

private:
	CFloatingWindowThread WinThread;

	//创建完成的事件
	CEvent InitFinished;

	//是否正在拖动
	BOOL isDraging;

	//是否贴边
	BOOL bEdgeHide;

public:
	CFloatingWindowDlg::CFloatingWindowDlg();
	CFloatingWindowDlg::CFloatingWindowDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CFloatingWindowDlg::~CFloatingWindowDlg();

	BOOL Initialize(DWORD dwMilliseconds=INFINITE);

	BOOL Show();
	BOOL Hide();

	BOOL Destory(DWORD dwMilliseconds=INFINITE);

public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg LRESULT OnDisplayChange(WPARAM BitsPerPixel, LPARAM size);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

#include "FloatingWindowDlg.inl"


