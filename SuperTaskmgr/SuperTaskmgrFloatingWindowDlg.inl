


inline BYTE HalfWhiteValue(BYTE value){
	return value/2+128;
}

inline COLORREF HalfWhiteColor(COLORREF color){
	return RGB(
		HalfWhiteValue(GetRValue(color)),
		HalfWhiteValue(GetGValue(color)),
		HalfWhiteValue(GetBValue(color))
		);
}



inline CSuperTaskmgrFloatingWindowDlg::CSuperTaskmgrFloatingWindowDlg(CWnd* pParent): // 标准构造函数
	CFloatingWindowDlg(CSuperTaskmgrFloatingWindowDlg::IDD, pParent),
	m_szUpText(NULL),
	m_szDownText(NULL),
	pParentWindow(pParent)
{
	m_Font.CreateFont(16,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Arial"));
}
inline CSuperTaskmgrFloatingWindowDlg::~CSuperTaskmgrFloatingWindowDlg(){
	m_brBack.DeleteObject();
	m_brBack1.DeleteObject();
	m_penBack.DeleteObject();
	m_Font.DeleteObject();
}


inline void CSuperTaskmgrFloatingWindowDlg::SetBackColor(COLORREF crBack) {
	if (m_brBack.GetSafeHandle())
		m_brBack.DeleteObject();
	m_brBack.CreateSolidBrush(crBack);
	if (m_brBack1.GetSafeHandle())
		m_brBack1.DeleteObject();
	m_brBack1.CreateSolidBrush(HalfWhiteColor(crBack));
	if (m_penBack.GetSafeHandle())
		m_penBack.DeleteObject();
	m_penBack.CreatePen(PS_SOLID,0,crBack);
}

inline void CSuperTaskmgrFloatingWindowDlg::SetSpeedText(LPTSTR szUpText,LPTSTR szDownText) {
	m_szUpText=szUpText;
	m_szDownText=szDownText;
}

inline void CSuperTaskmgrFloatingWindowDlg::Redraw(COLORREF crBack,LPTSTR szUpText,LPTSTR szDownText){
	SetBackColor(crBack);
	SetSpeedText(szUpText,szDownText);
	Invalidate();
}

