// SuperTaskmgrFloatingWindowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrFloatingWindowDlg.h"
#include "MFCLibrary\MFCDoubleBuffer.h"
#include "MFCLibrary\SetWindowPos.h"

#define ROUND_VALUE 15

// CSuperTaskmgrFloatingWindowDlg 对话框

IMPLEMENT_DYNAMIC(CSuperTaskmgrFloatingWindowDlg, CFloatingWindowDlg)

void CSuperTaskmgrFloatingWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSuperTaskmgrFloatingWindowDlg, CFloatingWindowDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CSuperTaskmgrFloatingWindowDlg::OnInitDialog()
{
	CFloatingWindowDlg::OnInitDialog();

	//设置悬浮窗大小
	WINDOWPLACEMENT stWinPlaceMent;
	VERIFY(GetWindowPlacement(&stWinPlaceMent));
	stWinPlaceMent.rcNormalPosition.left=0;
	stWinPlaceMent.rcNormalPosition.top=0;
	stWinPlaceMent.rcNormalPosition.right=FloatingWindowWidth;
	stWinPlaceMent.rcNormalPosition.bottom=FloatingWindowHeight;

	//设置悬浮窗形状
	CRgn m_rgn;
	m_rgn.CreateRoundRectRgn(
		0,0,
		stWinPlaceMent.rcNormalPosition.right+1,stWinPlaceMent.rcNormalPosition.bottom+1,
		ROUND_VALUE,ROUND_VALUE
		);
	SetWindowRgn(m_rgn,FALSE);

	VERIFY(SetWindowPlacement(&stWinPlaceMent));

	//创建 gdi 对象
	brShadow.CreateSolidBrush(SHADOW_COLOR);
	brFore.CreateSolidBrush(FORE_COLOR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSuperTaskmgrFloatingWindowDlg::OnPaint()
{
	CMemPaintDC dc(this); // device context for painting

	//绘制背景色
	dc.FillRect(&dc.m_rect,&m_brBack1);

	//绘制圆角矩形
	dc.SelectObject(&m_brBack);
	dc.SelectObject(&m_penBack);
	dc.RoundRect(1,1,dc.m_rect.right-1,dc.m_rect.bottom-1,ROUND_VALUE,ROUND_VALUE);

	//绘制向上箭头
	static const POINT paTriUpShadow[3]={12,6,6,12,18,12};
	static const POINT paTriUpFore[3]={13,7,7,13,19,13};
	static const RECT rRectUpShadow={11,12,14,19};
	static const RECT rRectUpFore={12,13,15,20};
	//dc.SelectObject(&brShadow);
	//dc.Polygon(paTriUpShadow,3);
	//dc.FillRect(&rRectUpShadow,&brShadow);
	dc.SelectObject(&brFore);
	dc.Polygon(paTriUpFore,3);
	dc.FillRect(&rRectUpFore,&brFore);

	//绘制向下箭头
	static const POINT paTriDownShadow[3]={12,35,6,29,18,29};
	static const POINT paTriDownFore[3]={13,36,7,30,19,30};
	static const RECT rRectDownShadow={11,23,14,30};
	static const RECT rRectDownFore={12,24,15,31};
	//dc.SelectObject(&brShadow);
	//dc.Polygon(paTriDownShadow,3);
	//dc.FillRect(&rRectDownShadow,&brShadow);
	dc.SelectObject(&brFore);
	dc.Polygon(paTriDownFore,3);
	dc.FillRect(&rRectDownFore,&brFore);

	//绘制文字
	if (m_szUpText && m_szDownText) {
		CRect rect=dc.m_rect;
		rect.bottom/=2;
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextAlign(TA_RIGHT);
		dc.SelectObject(&m_Font);

		//dc.SetTextColor(SHADOW_COLOR);
		//dc.TextOut(98,5,m_szUpText,(int)_tcslen(m_szUpText));
		dc.SetTextColor(FORE_COLOR);
		dc.TextOut(99,6,m_szUpText,(int)_tcslen(m_szUpText));

		//dc.SetTextColor(SHADOW_COLOR);
		//dc.TextOut(98,21,m_szDownText,(int)_tcslen(m_szDownText));
		dc.SetTextColor(FORE_COLOR);
		dc.TextOut(99,22,m_szDownText,(int)_tcslen(m_szDownText));
	}

	dc.Paint();
}

BOOL CSuperTaskmgrFloatingWindowDlg::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

LRESULT CSuperTaskmgrFloatingWindowDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam){
	return 0;
}

void CSuperTaskmgrFloatingWindowDlg::Setup(SETTING &setting){
	if (setting.ShowFloatingWindow){
		//设置悬浮窗透明度
		VERIFY(SetLayeredWindowAttributes(0, (BYTE)setting.bAlpha, LWA_ALPHA));

		//设置窗口位置
		RECT stWorkArea;
		VERIFY(SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&stWorkArea,0));

		ASSERT(setting.dwFloatWindowX<=1.0l && setting.dwFloatWindowX>=0.0l);
		ASSERT(setting.dwFloatWindowY<=1.0l && setting.dwFloatWindowY>=0.0l);

		POINT fp={
			stWorkArea.left+(LONG)((stWorkArea.right-stWorkArea.left)*setting.dwFloatWindowX)-FloatingWindowWidth/2,
			stWorkArea.top+(LONG)((stWorkArea.bottom-stWorkArea.top)*setting.dwFloatWindowY)-FloatingWindowHeight/2
		};

		switch(setting.iCloseToBorder){
		case 1:
			fp.x=stWorkArea.left;
			break;
		case 3:
			fp.x=stWorkArea.right-FloatingWindowWidth;
			break;
		case 2:
			fp.y=stWorkArea.top;
			break;
		case 4:
			fp.y=stWorkArea.bottom-FloatingWindowHeight;
			break;
		}

		VERIFY(SetWindowPosition(*this,fp.x,fp.y));
		ShowWindow(SW_SHOW);
	} else {
		ShowWindow(SW_HIDE);
	}
}

void CSuperTaskmgrFloatingWindowDlg::OnMove(int x, int y)
{
	CFloatingWindowDlg::OnMove(x, y);

	if (x>0 || y>0){
		RECT stWorkArea;
		VERIFY(SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&stWorkArea,0));

		SETTING &setting=CSuperTaskmgrDlg::setting;
		setting.dwFloatWindowX=(double)(x+FloatingWindowWidth/2)/(stWorkArea.right-stWorkArea.left);
		setting.dwFloatWindowY=(double)(y+FloatingWindowHeight/2)/(stWorkArea.bottom-stWorkArea.top);

		if (x==stWorkArea.left)
			setting.iCloseToBorder=1;
		else if (y==stWorkArea.top)
			setting.iCloseToBorder=2;
		else if (x+FloatingWindowWidth==stWorkArea.right)
			setting.iCloseToBorder=3;
		else if (y+FloatingWindowHeight==stWorkArea.bottom)
			setting.iCloseToBorder=4;
		else
			setting.iCloseToBorder=0;

		ASSERT(setting.dwFloatWindowX<=1.0l && setting.dwFloatWindowX>=0.0l);
		ASSERT(setting.dwFloatWindowY<=1.0l && setting.dwFloatWindowY>=0.0l);
	}
}

void CSuperTaskmgrFloatingWindowDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	::PostMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),WM_SHOW,0,0);
	CFloatingWindowDlg::OnLButtonDblClk(nFlags, point);
}




void CSuperTaskmgrFloatingWindowDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CFloatingWindowDlg::OnRButtonDown(nFlags, point);
}


