
// FloatingWindowDlg.cpp


#include "stdafx.h"
#include "FloatingWindowDlg.h"
#include "SetWindowPos.h"

#define ID_EDGE_HIDE 3992


DWORD CFloatingWindowThread::ThreadProc(LPVOID lpParam){
	return ((CFloatingWindowDlg *)lpParam)->DoModal() != -1;
}


IMPLEMENT_DYNAMIC(CFloatingWindowDlg, CDialog)

BEGIN_MESSAGE_MAP(CFloatingWindowDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVING()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_DISPLAYCHANGE,&CFloatingWindowDlg::OnDisplayChange)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


BOOL CFloatingWindowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置悬浮窗置顶
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

	//设置悬浮窗样式
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE | WS_EX_LAYERED);

	// 初始化完成
	InitFinished.Set();
	return TRUE;
}

VOID CALLBACK Timer_LeftHide(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
	ASSERT(IsWindow(hwnd));
	ASSERT(uMsg==WM_TIMER);
	ASSERT(idEvent==ID_EDGE_HIDE);

	RECT rWin;
	::GetWindowRect(hwnd,&rWin);
	::SetWindowPos(hwnd,NULL,rWin.left-1,rWin.top,0,0,SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER);
	if (rWin.right<3){
		KillTimer(hwnd,ID_EDGE_HIDE);
	}
}

void CFloatingWindowDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);

	// TODO: 在此处添加消息处理程序代码
}


void CFloatingWindowDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	isDraging=TRUE;
}

void CFloatingWindowDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	RECT stWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&stWorkArea,0);

	//防止拖动拖出工作区域
	if (isDraging) {
		SIZE stWindowSize={
			pRect->right-pRect->left,
			pRect->bottom-pRect->top,
		};
		if (pRect->right>stWorkArea.right)
			pRect->left=stWorkArea.right-stWindowSize.cx;
		if (pRect->bottom>stWorkArea.bottom)
			pRect->top=stWorkArea.bottom-stWindowSize.cy;
		if (pRect->left<stWorkArea.left)
			pRect->left=stWorkArea.left;
		if (pRect->top<stWorkArea.top)
			pRect->top=stWorkArea.top;
		pRect->right=pRect->left+stWindowSize.cx;
		pRect->bottom=pRect->top+stWindowSize.cy;
		goto end;
	}

	//检查是否贴边
	if (bEdgeHide){
		//检查是否贴左边
		if (stWorkArea.left==0 && pRect->left<3 && pRect->left>=0){
			SetTimer(ID_EDGE_HIDE,1,&Timer_LeftHide);
			goto end;
		}
	}

end:
	CDialog::OnMoving(fwSide, pRect);
}


void CFloatingWindowDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	isDraging=FALSE;
	CDialog::OnLButtonUp(nFlags, point);
}


// 屏幕分辨率改变时执行此函数，防止悬浮窗超出屏幕边界
LRESULT CFloatingWindowDlg::OnDisplayChange(WPARAM BitsPerPixel, LPARAM size){

	//获取工作区域
	RECT rWorkArea;
	VERIFY(SystemParametersInfo(SPI_GETWORKAREA,0,&rWorkArea,0));

	//获取悬浮窗窗口布局
	RECT rWinRect;
	GetWindowRect(&rWinRect);

	//得到当前悬浮窗坐标
	POINT p;
	p.x=rWinRect.left;
	p.y=rWinRect.top;

	//适当调整位置
	if (rWinRect.right>rWorkArea.right)
		p.x-=rWinRect.right-rWorkArea.right;
	if (rWinRect.bottom>rWorkArea.bottom)
		p.y-=rWinRect.bottom-rWorkArea.bottom;

	//设置位置
	SetWindowPosition(*this, p.x, p.y);
	return 0;
}


