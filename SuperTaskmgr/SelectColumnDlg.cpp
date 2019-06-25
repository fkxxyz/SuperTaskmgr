// SelectColumnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SelectColumnDlg.h"


// CSelectColumnDlg 对话框

IMPLEMENT_DYNAMIC(CSelectColumnDlg, CDialog)

CSelectColumnDlg::CSelectColumnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectColumnDlg::IDD, pParent)
{

}

CSelectColumnDlg::~CSelectColumnDlg()
{
}

void CSelectColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COLOMN, m_LstColumn);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
}


BEGIN_MESSAGE_MAP(CSelectColumnDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectColumnDlg::OnBnClickedOk)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSelectColumnDlg 消息处理程序

BOOL CSelectColumnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int i;

	VERIFY(ModifyStyleEx(0,WS_EX_DLGMODALFRAME));

	//初始化列表控件
	m_LstColumn.SetExtendedStyle(LVS_EX_CHECKBOXES);
	int ItemN=m_LstColumn.GetItemCount();
	for (i=ItemN;i<cENUM_COLUMN_END;i++)
		m_LstColumn.InsertItem(i,CSuperTaskmgrDlg::ColumnItemName[i].name);

	for (i=0;i<psetting->ColumnNumber;i++)
		m_LstColumn.SetCheck(psetting->column[i].cindex);

	SendMessage(WM_SIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSelectColumnDlg::OnBnClickedOk()
{
	int i,j;
	BOOL ColumnCheck[cENUM_COLUMN_END];
	BOOL SettingCheck[cENUM_COLUMN_END]={0};

	//获取列表所有项目的复选框
	int ItemN=m_LstColumn.GetItemCount();
	for (i=0;i<ItemN;i++)
		ColumnCheck[i]=m_LstColumn.GetCheck(i);

	//获取设置中已有的项目
	for (i=0;i<psetting->ColumnNumber;i++)
		SettingCheck[psetting->column[i].cindex]=TRUE;

	//删除已取消勾选的项目
	for (i=0;i<psetting->ColumnNumber;i++){
		if (!ColumnCheck[psetting->column[i].cindex]){ //如果该项已被删除
			//后面所有的项目向前移一位
			for (j=i+1;j<psetting->ColumnNumber;j++)
				psetting->column[j-1].cindex=psetting->column[j].cindex;
			psetting->ColumnNumber--;
			i--;
		}
	}

	//添加新勾选的项目
	for (i=0;i<ItemN;i++){
		if (ColumnCheck[i] && !SettingCheck[i]){//如果该项是新添加的项
			psetting->column[psetting->ColumnNumber].cindex=(ENUM_COLUMN)i;
			psetting->column[psetting->ColumnNumber++].width=CSuperTaskmgrDlg::ColumnItemName[i].xDefaultWidth;
		}
	}
	OnOK();
}

void CSelectColumnDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (!m_LstColumn.m_hWnd) return;
	if (nType==SIZE_MINIMIZED) return;

#define BorderWidth 7

	RECT ClientRect,ButtonRect;
	SIZE ClientSize,ButtonSize;
	GetClientRect(&ClientRect);
	ClientSize.cx=ClientRect.right-ClientRect.left;
	ClientSize.cy=ClientRect.bottom-ClientRect.top;
	m_Cancel.GetWindowRect(&ButtonRect);
	ButtonSize.cx=ButtonRect.right-ButtonRect.left;
	ButtonSize.cy=ButtonRect.bottom-ButtonRect.top;

	m_LstColumn.MoveWindow(
		BorderWidth,
		BorderWidth,
		ClientSize.cx-(BorderWidth*2),
		ClientSize.cy-ButtonSize.cy-(BorderWidth*3)
	);
	m_Cancel.MoveWindow(
		ClientRect.right-ButtonSize.cx-BorderWidth,
		ClientRect.bottom-ButtonSize.cy-BorderWidth,
		ButtonSize.cx,
		ButtonSize.cy
	);
	m_OK.MoveWindow(
		ClientRect.right-(ButtonSize.cx*2)-(BorderWidth*3),
		ClientRect.bottom-ButtonSize.cy-BorderWidth,
		ButtonSize.cx,
		ButtonSize.cy
	);
}

