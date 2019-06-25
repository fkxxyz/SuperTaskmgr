
// MFCDoubleBuffer.h
// 双缓冲的 CPaintDC 类

#pragma once

class CMemPaintDC : public CDC {
	CBitmap m_bmpMem; //内存设备上下文的位图
	CPaintDC m_dc; //真正绘图的设备上下文

public:
	CRect m_rect; //绘图的区域

	explicit CMemPaintDC(CWnd* pWnd)
		:m_dc(pWnd)
	{
		pWnd->GetClientRect(&m_rect);
		CDC::CreateCompatibleDC(&m_dc);
		m_bmpMem.CreateCompatibleBitmap(&m_dc,m_rect.Width(),m_rect.Height());
		CDC::SelectObject(&m_bmpMem);
	}
	~CMemPaintDC(){
		CDC::DeleteDC();
		m_bmpMem.DeleteObject();
	}
	BOOL Paint(){
		return m_dc.BitBlt(0,0,m_rect.Width(),m_rect.Height(),this,0,0,SRCCOPY);
	}
	BOOL Paint(int x, int y, int xSrc, int ySrc){
		return m_dc.BitBlt(x,y,m_rect.Width(),m_rect.Height(),this,xSrc,ySrc,SRCCOPY);
	}
	BOOL Paint(int x, int y, int nWidth, int nHeight, int xSrc, int ySrc){
		return m_dc.BitBlt(x,y,nWidth,nHeight,this,xSrc,ySrc,SRCCOPY);
	}
};

