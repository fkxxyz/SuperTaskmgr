
// SuperListCtrl.cpp

#include "stdafx.h"
#include "SuperListCtrl.h"

BEGIN_MESSAGE_MAP(CSuperListCtrl, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CSuperListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CSuperListCtrl::OnPaint()
{
	CMemPaintDC dc(this); // device context for painting
	dc.FillSolidRect(&dc.m_rect,RGB(255,255,255));
	DefWindowProc(WM_PAINT,(WPARAM)dc.m_hDC,(LPARAM)0);

#ifdef SUPER_LIST_CTRL_PAINT_EX
	PaintEx(&dc);
#endif

	CRect headerRect;
	GetHeaderCtrl()->GetWindowRect(&headerRect);
	dc.Paint(0,headerRect.Height(),0,headerRect.Height());
}
