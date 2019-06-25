#pragma once
#include "SuperTaskmgrConfig.h"
#include "afxcmn.h"
#include "afxwin.h"

// CSelectColumnDlg 对话框

class CSelectColumnDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectColumnDlg)

public:
	CSelectColumnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectColumnDlg();

// 对话框数据
	enum { IDD = IDD_SELECT_COLUMN };

public:
	SETTING *psetting;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_LstColumn;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CButton m_Cancel;
	CButton m_OK;
};
