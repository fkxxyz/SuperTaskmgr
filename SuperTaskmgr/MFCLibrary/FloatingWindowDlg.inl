
// FloatingWindowDlg.inl


inline CFloatingWindowDlg::CFloatingWindowDlg(){

}


inline CFloatingWindowDlg::CFloatingWindowDlg(UINT nIDTemplate, CWnd* pParentWnd)
	:CDialog(nIDTemplate,pParentWnd),
	bEdgeHide(TRUE),
	isDraging(FALSE)
{

}

inline CFloatingWindowDlg::~CFloatingWindowDlg(){
	if (m_hWnd)
		VERIFY(Destory());
}

inline BOOL CFloatingWindowDlg::Initialize(DWORD dwMilliseconds){
	//创建初始化完成的事件
	if (!InitFinished.Create())
		return FALSE;

	//创建 DoModal 线程
	if (!WinThread.Create(this))
		return FALSE;

	//等待初始化完成
	if (!InitFinished.Wait(dwMilliseconds))
		return FALSE;

	return TRUE;
}


inline BOOL CFloatingWindowDlg::Show(){
	return ShowWindow(SW_NORMAL);
}

inline BOOL CFloatingWindowDlg::Hide(){
	return ShowWindow(SW_HIDE);
}

inline BOOL CFloatingWindowDlg::Destory(DWORD dwMilliseconds){
	//发送销毁窗体的消息
	if (SendMessage(WM_CLOSE) != 0)
		return FALSE;

	//等待线程执行结束
	if (!WinThread.Wait(dwMilliseconds))
		return FALSE;

	//关闭线程句柄
	if (!WinThread.Close())
		return FALSE;

	return TRUE;
}


