#pragma once

#include "MFCLibrary\FloatingWindowDlg.h"


#define FloatingWindowWidth 108
#define FloatingWindowHeight 42


// CSuperTaskmgrFloatingWindowDlg 对话框

class CSuperTaskmgrFloatingWindowDlg : public CFloatingWindowDlg
{
	DECLARE_DYNAMIC(CSuperTaskmgrFloatingWindowDlg)

#define SHADOW_COLOR RGB(127,63,127)
#define FORE_COLOR RGB(0,0,0)

	CBrush m_brBack;
	CBrush m_brBack1;
	CBrush brShadow;
	CBrush brFore;
	CPen m_penBack;
	CFont m_Font;
	LPTSTR m_szUpText,m_szDownText;

	CWnd *pParentWindow;

public:
	CSuperTaskmgrFloatingWindowDlg(CWnd* pParent =NULL); // 标准构造函数;

	~CSuperTaskmgrFloatingWindowDlg();

// 对话框数据
	enum { IDD = IDD_FLOATWINDOW_DIALOG };

	void Setup(SETTING &setting);

	void SetBackColor(COLORREF crBack);
	void SetSpeedText(LPTSTR szUpText,LPTSTR szDownText);
	void Redraw(COLORREF crBack,LPTSTR szUpText,LPTSTR szDownText);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	LRESULT OnDisplayChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#include "SuperTaskmgrFloatingWindowDlg.inl"


