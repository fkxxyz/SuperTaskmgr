#pragma once

#include "SuperTaskmgrProcess.h"
#include "WinLibrary\360Netmon.h"
#include "afxwin.h"

// CLimitSpeedDlg 对话框

class CLimitSpeedDlg : public CDialog
{
	DECLARE_DYNAMIC(CLimitSpeedDlg)

public:
	CLimitSpeedDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLimitSpeedDlg();

// 对话框数据
	enum { IDD = IDD_LIMIT_SPEED };

	void SetProcess(PLIMIT_SPEED pLimitSpeed,LPPROCESS pProcess);

private:
	LPPROCESS lpNowProcess;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
	CStatic m_Icon;
	CStatic m_Process;
	CEdit m_LimitSend;
	CEdit m_LimitRecv;
	CButton m_Disable;
	afx_msg void OnBnClickedCheckDisable();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnSetfocusLimitRecv();
	afx_msg void OnEnSetfocusLimitSend();
};



