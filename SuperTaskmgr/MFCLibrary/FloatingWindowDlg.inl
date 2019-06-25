
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
	//������ʼ����ɵ��¼�
	if (!InitFinished.Create())
		return FALSE;

	//���� DoModal �߳�
	if (!WinThread.Create(this))
		return FALSE;

	//�ȴ���ʼ�����
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
	//�������ٴ������Ϣ
	if (SendMessage(WM_CLOSE) != 0)
		return FALSE;

	//�ȴ��߳�ִ�н���
	if (!WinThread.Wait(dwMilliseconds))
		return FALSE;

	//�ر��߳̾��
	if (!WinThread.Close())
		return FALSE;

	return TRUE;
}


