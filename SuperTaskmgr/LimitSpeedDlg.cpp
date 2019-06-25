// LimitSpeedDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "LimitSpeedDlg.h"
#include "SuperTaskmgrDlg.h"


// CLimitSpeedDlg 对话框

IMPLEMENT_DYNAMIC(CLimitSpeedDlg, CDialog)

CLimitSpeedDlg::CLimitSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLimitSpeedDlg::IDD, pParent)
{

}

CLimitSpeedDlg::~CLimitSpeedDlg()
{
}

void CLimitSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ICON, m_Icon);
	DDX_Control(pDX, IDC_STATIC_PROCESS, m_Process);
	DDX_Control(pDX, IDC_LIMIT_SEND, m_LimitSend);
	DDX_Control(pDX, IDC_LIMIT_RECV, m_LimitRecv);
	DDX_Control(pDX, IDC_CHECK_DISABLE, m_Disable);
}


BEGIN_MESSAGE_MAP(CLimitSpeedDlg, CDialog)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CLimitSpeedDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_DISABLE, &CLimitSpeedDlg::OnBnClickedCheckDisable)
	ON_EN_SETFOCUS(IDC_LIMIT_RECV, &CLimitSpeedDlg::OnEnSetfocusLimitRecv)
	ON_EN_SETFOCUS(IDC_LIMIT_SEND, &CLimitSpeedDlg::OnEnSetfocusLimitSend)
END_MESSAGE_MAP()



BOOL CLimitSpeedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

	return FALSE;
}

void CLimitSpeedDlg::SetProcess(PLIMIT_SPEED pLimitSpeed,LPPROCESS pProcess){
	//设置显示的进程
	if (pProcess->hIcon)
		VERIFY(m_Icon.SetIcon(pProcess->hIcon));
	m_Process.SetWindowText(pProcess->FileDescription);

	if (pLimitSpeed->nLimitSend==LLONG_MAX) pLimitSpeed->nLimitSend=0l;
	if (pLimitSpeed->nLimitRecv==LLONG_MAX) pLimitSpeed->nLimitRecv=0l;

	//设置此进程的网络限制状态
	TCHAR szBuffer[64];
	_stprintf_s<64>(szBuffer,_T("%lld"),pLimitSpeed->nLimitSend/1024);
	m_LimitSend.SetWindowText(szBuffer);
	_stprintf_s<64>(szBuffer,_T("%lld"),pLimitSpeed->nLimitRecv/1024);
	m_LimitRecv.SetWindowText(szBuffer);

	m_Disable.SetCheck(pProcess->bDisabledNetwork);
	OnBnClickedCheckDisable();

	lpNowProcess=pProcess;
}

// CLimitSpeedDlg 消息处理程序
void CLimitSpeedDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (nState==WA_INACTIVE)
		ShowWindow(SW_HIDE);
	else if (nState==WA_ACTIVE){
		if (!m_Disable.GetCheck()){
			m_LimitRecv.SetFocus();
		}
	}
}

void CLimitSpeedDlg::OnEnSetfocusLimitRecv()
{
	m_LimitRecv.PostMessage(EM_SETSEL, 0, -1);
}

void CLimitSpeedDlg::OnEnSetfocusLimitSend()
{
	m_LimitSend.PostMessage(EM_SETSEL, 0, -1);
}

void CLimitSpeedDlg::OnBnClickedCheckDisable()
{
	BOOL bCheck=m_Disable.GetCheck();
	m_LimitSend.EnableWindow(!bCheck);
	m_LimitRecv.EnableWindow(!bCheck);
}

void CLimitSpeedDlg::OnBnClickedOk()
{
	if (m_Disable.GetCheck()){
		VERIFY(CSuperTaskmgrDlg::TdiFilter.DisableProcessNetwork(lpNowProcess->path));
	} else {
		LONGLONG lSend=GetDlgItemInt(IDC_LIMIT_SEND);
		LONGLONG lRecv=GetDlgItemInt(IDC_LIMIT_RECV);

		if (lSend>1024*1024*128 || lRecv>1024*1024*128){
			ShowError(0,AppName,_T("输入的数字太大！"),NULL,ERROR_INVALID_DATA,MB_OK | MB_ICONERROR);
			goto end;
		}

		lSend*=1024;lRecv*=1024;
		if (lSend==0u) lSend=LLONG_MAX;
		if (lRecv==0u) lRecv=LLONG_MAX;
		VERIFY(CSuperTaskmgrDlg::TdiFilter.LimitProcessSpeed(lpNowProcess->path,lSend,lRecv));
	}

end:
	OnOK();
}




