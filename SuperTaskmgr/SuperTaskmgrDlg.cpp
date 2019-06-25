// SuperTaskmgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "WinKillProcess.h"
#include "MFCLibrary\SetWindowPos.h"

#include <psapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT indicators[] =
{
	ID_SEPARATOR,
	//ID_SEPARATOR,
	//ID_SEPARATOR,
	//ID_SEPARATOR,
};
const int CSuperTaskmgrDlg::BorderSize=4;//�߽���



CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_Version);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnConnectMe)
END_MESSAGE_MAP()


// CSuperTaskmgrDlg �Ի���


CSuperTaskmgrDlg *CSuperTaskmgrDlg::pcThis=NULL;
ENUM_COLUMN CSuperTaskmgrDlg::SortColItem=cENUM_COLUMN_END;//�����б�ͷ�������
int CSuperTaskmgrDlg::RevSort=1;//�Ƿ�����

CSuperTaskmgrDlg::CSuperTaskmgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperTaskmgrDlg::IDD, pParent)
{

	pcThis=this;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bSizeInitilaized=FALSE;
	sWorkDir=NULL;
	sWhiteListFile=NULL;
	sBlackListFile=NULL;
	sSystemProcessFile=NULL;
	sCFGFile=NULL;
	memset(&setting,0,sizeof(setting));
	proN=0;

	qCPUTime=0l; //��CPUʱ��
	qReadTransferCount=0l; //��I/O��ȡ�ֽ�
	qWriteTransferCount=0l; //��I/Oд���ֽ�

	qNetRecvSpeed=0l;
	qNetSendSpeed=0l;
	qNetTotalSpeed=0l;

	SortColIndex=-1;//�����б�ͷ��ţ�-1��ʾĬ�ϣ�
	LastSortColIndex=-1;//��һ�������б�ͷ��ţ������ظ����ͬһ���б�ͷ�Ƿ�����

	pbrushBlue=NULL;
	pbrushRed=NULL;
	pbrushYellow=NULL;
	pbrushBack=NULL;
	ppenFore=NULL;
	pogNotifyIcon=NULL;
	pobmNotifyIcon=NULL;
	hNotifyIcon=NULL;

	pogNotifyIcon;
	pobmNotifyIcon;
}

CSuperTaskmgrDlg::~CSuperTaskmgrDlg()
{
}

void CSuperTaskmgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTPROC, m_LstProc);

	m_Menu.LoadMenu(IDR_MENU_MAIN);
	m_MenuNotify.LoadMenu(IDR_MENU_NOTIFY);
	DDX_Control(pDX, ID_ONEKEY_KILL, m_OneKeyKill);
	DDX_Control(pDX, ID_BUTTOM_SEL_ALL, m_SelAll);
	DDX_Control(pDX, ID_BUTTOM_SEL_NOT_WHITE, m_SelNoWhite);
	DDX_Control(pDX, ID_BUTTOM_SEL_REV, m_SelRev);
	DDX_Control(pDX, ID_BUTTOM_SEL_BLACK, m_SelBlack);
	DDX_Control(pDX, IDC_STATIC_PROCESS_NUMBER, m_ProcessNumber);
	DDX_Control(pDX, IDC_STATIC_CPU, m_CPU);
	DDX_Control(pDX, IDC_STATIC_IO, m_IO);
	DDX_Control(pDX, IDC_STATIC_MEMORY, m_Memory);
	DDX_Control(pDX, IDC_STATIC_SEND_SPEED, m_SendSpeed);
	DDX_Control(pDX, IDC_STATIC_RECV_SPEED, m_RecvSpeed);
}

BEGIN_MESSAGE_MAP(CSuperTaskmgrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_NEW_TASK, &CSuperTaskmgrDlg::OnNewTask)
	ON_COMMAND(ID_KILL, &CSuperTaskmgrDlg::OnKill)
	ON_NOTIFY(NM_CLICK, IDC_LSTPROC, &CSuperTaskmgrDlg::OnNMClickLstproc)
	ON_NOTIFY(NM_RCLICK, IDC_LSTPROC, &CSuperTaskmgrDlg::OnNMRclickLstproc)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LSTPROC, &CSuperTaskmgrDlg::OnLvnColumnclickLstproc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTPROC, &CSuperTaskmgrDlg::OnLvnItemchangedLstproc)

	ON_BN_CLICKED(ID_BUTTOM_SEL_ALL, &CSuperTaskmgrDlg::OnBnClickedButtomSelAll)
	ON_BN_CLICKED(ID_BUTTOM_SEL_REV, &CSuperTaskmgrDlg::OnBnClickedSelRev)
	ON_BN_CLICKED(ID_BUTTOM_SEL_NOT_WHITE, &CSuperTaskmgrDlg::OnBnClickedSelNotWhite)
	ON_BN_CLICKED(ID_BUTTOM_SEL_BLACK, &CSuperTaskmgrDlg::OnBnClickedSelBlack)
	ON_COMMAND(ID_SEL_ALL, &CSuperTaskmgrDlg::OnSelAll)
	ON_COMMAND(ID_SEL_REV, &CSuperTaskmgrDlg::OnSelRev)
	ON_COMMAND(ID_SEL_NOT_WHITE, &CSuperTaskmgrDlg::OnSelNotWhite)
	ON_COMMAND(ID_SEL_BLACK, &CSuperTaskmgrDlg::OnSelBlack)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LSTPROC, &CSuperTaskmgrDlg::OnNMCustomdrawLstproc)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_ONEKEY_KILL, &CSuperTaskmgrDlg::OnBnClickedOnekeyKill)
	ON_UPDATE_COMMAND_UI(ID_KILL, &CSuperTaskmgrDlg::OnUpdateKill)
	ON_UPDATE_COMMAND_UI(ID_FORCE_KILL, &CSuperTaskmgrDlg::OnUpdateForceKill)
	ON_COMMAND(ID_SUSPEND, &CSuperTaskmgrDlg::OnSuspend)
	ON_UPDATE_COMMAND_UI(ID_SUSPEND, &CSuperTaskmgrDlg::OnUpdateSuspend)
	ON_COMMAND(ID_RESUME, &CSuperTaskmgrDlg::OnResume)
	ON_UPDATE_COMMAND_UI(ID_RESUME, &CSuperTaskmgrDlg::OnUpdateResume)
	ON_COMMAND(ID_SET_BLACK, &CSuperTaskmgrDlg::OnSetBlack)
	ON_COMMAND(ID_SET_SYSTEM, &CSuperTaskmgrDlg::OnSetSystem)
	ON_COMMAND(ID_SET_WHITE, &CSuperTaskmgrDlg::OnSetWhite)
	ON_UPDATE_COMMAND_UI(ID_SET_BLACK, &CSuperTaskmgrDlg::OnUpdateSetBlack)
	ON_UPDATE_COMMAND_UI(ID_SET_SYSTEM, &CSuperTaskmgrDlg::OnUpdateSetSystem)
	ON_UPDATE_COMMAND_UI(ID_SET_WHITE, &CSuperTaskmgrDlg::OnUpdateSetWhite)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_ATTRIB, &CSuperTaskmgrDlg::OnAttrib)
	ON_UPDATE_COMMAND_UI(ID_ATTRIB, &CSuperTaskmgrDlg::OnUpdateAttrib)
	ON_COMMAND(ID_DIRECTORY, &CSuperTaskmgrDlg::OnDirectory)
	ON_UPDATE_COMMAND_UI(ID_DIRECTORY, &CSuperTaskmgrDlg::OnUpdateDirectory)

	ON_COMMAND(40501, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(40501, OnUpdateMenuItemNull)
	ON_COMMAND(40502, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(40502, OnUpdateMenuItemNull)
	ON_COMMAND(40503, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(40503, OnUpdateMenuItemNull)
	ON_COMMAND(40504, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(40504, OnUpdateMenuItemNull)
	ON_COMMAND(40505, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(40505, OnUpdateMenuItemNull)
	ON_COMMAND(40506, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(40506, OnUpdateMenuItemPriority)

	ON_COMMAND(41300, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(41300, OnUpdateMenuItemNull)
	ON_COMMAND(41301, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(41301, OnUpdateMenuItemNull)
	ON_COMMAND(41302, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(41302, OnUpdateMenuItemNull)
	ON_COMMAND(41303, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(41303, OnUpdateMenuItemNull)
	ON_COMMAND(41304, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(41304, OnUpdateMenuItemNull)
	ON_COMMAND(41305, OnMenuItemNull)
	ON_UPDATE_COMMAND_UI(41305, OnUpdateMenuItemNull)

	ON_COMMAND(ID_SHOW_IN_FRONT, &CSuperTaskmgrDlg::OnShowInFront)
	ON_UPDATE_COMMAND_UI(ID_SHOW_IN_FRONT, &CSuperTaskmgrDlg::OnUpdateShowInFront)
	ON_COMMAND(ID_MIN_HIDE, &CSuperTaskmgrDlg::OnMinHide)
	ON_UPDATE_COMMAND_UI(ID_MIN_HIDE, &CSuperTaskmgrDlg::OnUpdateMinHide)
	ON_COMMAND(ID_REFRESH, &CSuperTaskmgrDlg::OnRefresh)
	ON_COMMAND(ID_SHOW_SYSTEM, &CSuperTaskmgrDlg::OnShowSystem)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SYSTEM, &CSuperTaskmgrDlg::OnUpdateShowSystem)
	ON_COMMAND(ID_LOAD_DEFAULT, &CSuperTaskmgrDlg::OnLoadDefault)
	ON_WM_MOVE()
	ON_MESSAGE(WM_ONNOTIFY,OnNotify)
	ON_MESSAGE(WM_SHOW,OnShow)
	ON_MESSAGE(WM_REFRESH,OnRefresh)
	ON_COMMAND(ID_SHOW, &CSuperTaskmgrDlg::OnShow)
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_COMMAND(ID_APP_ABOUT, &CSuperTaskmgrDlg::OnAppAbout)
	ON_COMMAND(ID_EDIT_WHITE, &CSuperTaskmgrDlg::OnEditWhite)
	ON_COMMAND(ID_EDIT_BLACK, &CSuperTaskmgrDlg::OnEditBlack)
	ON_COMMAND(ID_HIDE_THIS_COLUMN, &CSuperTaskmgrDlg::OnHideThisColumn)
	ON_UPDATE_COMMAND_UI(ID_HIDE_THIS_COLUMN, &CSuperTaskmgrDlg::OnUpdateHideThisColumn)
	ON_COMMAND(ID_SELECT_COLUMN, &CSuperTaskmgrDlg::OnSelectColumn)

	ON_MESSAGE(WM_WATCH_DATA_SEND,OnWatchDataSend)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_DISABLE_NETWORK, &CSuperTaskmgrDlg::OnDisableNetwork)
	ON_UPDATE_COMMAND_UI(ID_DISABLE_NETWORK, &CSuperTaskmgrDlg::OnUpdateDisableNetwork)
	ON_COMMAND(ID_ALLOW_NETWORK, &CSuperTaskmgrDlg::OnAllowNetwork)
	ON_UPDATE_COMMAND_UI(ID_ALLOW_NETWORK, &CSuperTaskmgrDlg::OnUpdateAllowNetwork)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SHOW_FLOATINGWINDOW, &CSuperTaskmgrDlg::OnShowFloatingwindow)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FLOATINGWINDOW, &CSuperTaskmgrDlg::OnUpdateShowFloatingwindow)
	ON_COMMAND(ID_EMPTY_WORKING_SET, &CSuperTaskmgrDlg::OnEmptyWorkingSet)
	ON_UPDATE_COMMAND_UI(ID_EMPTY_WORKING_SET, &CSuperTaskmgrDlg::OnUpdateEmptyWorkingSet)

	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED,OnTaskBarCreated)
	ON_STN_CLICKED(IDC_STATIC_MEMORY, &CSuperTaskmgrDlg::OnStnClickedStaticMemory)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WHITE, &CSuperTaskmgrDlg::OnUpdateNotepadEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BLACK, &CSuperTaskmgrDlg::OnUpdateNotepadEdit)
//	ON_NOTIFY(LVN_GETDISPINFO, IDC_LSTPROC, &CSuperTaskmgrDlg::OnLvnGetdispinfoLstproc)
	ON_NOTIFY(NM_DBLCLK, IDC_LSTPROC, &CSuperTaskmgrDlg::OnNMDblclkLstproc)
	ON_COMMAND(ID_LIMIT_SPEED, &CSuperTaskmgrDlg::OnLimitSpeed)
	ON_UPDATE_COMMAND_UI(ID_LIMIT_SPEED, &CSuperTaskmgrDlg::OnUpdateLimitSpeed)
	END_MESSAGE_MAP()


// CSuperTaskmgrDlg ��Ϣ�������

BOOL CSuperTaskmgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitSucceed=MainInit();
	if (!InitSucceed){
		ShowError(m_hWnd,AppName,_T("���������������ʼ��ʧ�ܣ�\n"),_T("\n\n�޷�����������ϵ�����ߡ�"),GetLastError());
		SendMessage(WM_CLOSE);
		return TRUE;
	}

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	TCHAR szTitle[128];
	VERSION_NUMBER &ver=((CSuperTaskmgrApp*)AfxGetApp())->m_Version;
	_stprintf_s<128>(szTitle,_T("�������������    V%d.%d.%d"),ver.dwMajor,ver.dwMinor,ver.dwBuild);
	SetWindowText(szTitle);

	//���ò˵�
	VERIFY(SetMenu(&m_Menu));

	VERIFY(LimitSpeedDlg.Create(IDD_LIMIT_SPEED));

	//��ʼ�����̶�̬ͼ��
	initIconBitmap();

	//�������̲˵�
	VERIFY(m_MenuNotify.GetSubMenu(0)->SetDefaultItem(0,TRUE));

	//���ò˵���ݼ�
	hAccel=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));

	//ע��ȫ���ȼ�
	BOOL R=RegisterHotKey(this->GetSafeHwnd(),IDH_SHOW, MOD_CONTROL | MOD_ALT, VK_INSERT);

	//����״̬��
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		//TRACE0("Failed to create status barn");
		return -1; // fail to create
	}
	UINT nID=0; //����״̬������ķ���

	//��ȡ�ؼ��ߴ���Ϣ
	RECT tr;//��ʱ���α���

	//��ȡ״̬���߶�
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	//��ȡ��һ����������ť�ߴ�
	m_OneKeyKill.GetWindowRect(&tr);
	sOneKeyKill.cx=tr.right-tr.left;
	sOneKeyKill.cy=tr.bottom-tr.top;

	//��ȡѡ��ť�ߴ�
	m_SelAll.GetWindowRect(&tr);
	sBSel.cx=tr.right-tr.left;
	sBSel.cy=tr.bottom-tr.top;
	m_SelNoWhite.GetWindowRect(&tr);
	sBSelList.cx=tr.right-tr.left;
	sBSelList.cy=tr.bottom-tr.top;

	//��ȡ״̬����̬�ı���ߴ�
	m_ProcessNumber.GetWindowRect(&tr);
	sSStatus.cx=tr.right-tr.left;
	sSStatus.cy=tr.bottom-tr.top;

	bSizeInitilaized=TRUE;

	//��ȡ����
	bReadCFGFile=TRUE;
	bReadWhiteList=TRUE;
	bReadBlackList=TRUE;
	bReadSystemList=TRUE;
	if (!LoadCFG(FALSE)){
		ShowError(m_hWnd,AppName,_T("�޷���ȡ���ã�\n"),NULL,GetLastError());
	}

	if (!ApplyCFG()){
		ShowError(m_hWnd,AppName,_T("�޷�Ӧ�����ã�\n"),NULL,GetLastError());
	}

	PostMessage(WM_SIZE);
	PostMessage(WM_SHOW);
	//PostMessage(WM_REFRESH,(WPARAM)TRUE);

	Refresh(TRUE);
	m_LstProc.SetFocus();
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//֪ͨ����Ϣ����
LRESULT CSuperTaskmgrDlg::OnNotify(WPARAM wParam,LPARAM lParam){
	//�������̲˵���Ϣ
	if (wParam==MainNotifyID){
		switch (lParam){
		case WM_RBUTTONUP: //�Ҽ����̲˵�
			{
				POINT pCur;
				GetCursorPos(&pCur);
				m_wndStatusBar.EnableWindow(FALSE);
				m_wndStatusBar.SetForegroundWindow();
				m_MenuNotify.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,pCur.x,pCur.y,this);
				m_wndStatusBar.EnableWindow(TRUE);
				break;
			}
		case WM_LBUTTONDBLCLK: //���˫�����̲˵�
			OnShow();
			break;
		}
	}
	return 0;
}

BOOL CSuperTaskmgrDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	//�����Ҽ��б�ͷ��Ϣ
	if ((((LPNMHDR)lParam)->code == NM_RCLICK)){
		CPoint pt, pt2;
		if (!GetCursorPos(&pt)) goto end;
		pt2 = pt;
		m_LstProc.ScreenToClient(&pt);
		CWnd* pWnd = m_LstProc.ChildWindowFromPoint(pt);
		CHeaderCtrl* pHeader = m_LstProc.GetHeaderCtrl();
		if(pWnd && (pWnd->GetSafeHwnd() == pHeader->GetSafeHwnd()))
		{
			HDHITTESTINFO info = {0};
			info.pt = pt;
			info.pt.x+=m_LstProc.GetScrollPos(SB_HORZ)+7;
			pHeader->SendMessage(HDM_HITTEST, 0, (LPARAM)&info);
			OnLvnColumnRClickLstproc(info.iItem);
		}
	}
end:
	return CDialog::OnNotify(wParam, lParam, pResult);
}

LRESULT CSuperTaskmgrDlg::OnWatchDataSend(WPARAM wParam, LPARAM lParam)
{
	ASSERT(0);
	TRACE(_T("Received: %d\t%d\n"),wParam,lParam);
	return RECV_SUCCESS;
}

BOOL CSuperTaskmgrDlg::PreTranslateMessage(MSG* pMsg)
{
	//���λس���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_ERASEBKGND)
		return TRUE;
	if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CSuperTaskmgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		VERIFY(ShowWindow(SW_HIDE));
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSuperTaskmgrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSuperTaskmgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CSuperTaskmgrDlg::OnSize(UINT nType, int cx, int cy)
{
	static BOOL bMinimized=FALSE;
	CDialog::OnSize(nType, cx, cy);
	if (!m_LstProc.m_hWnd) return;
	if (nType==SIZE_MINIMIZED) {
		//������ڱ���С��
		if (setting.HideWhenMinimize)
			VERIFY(ShowWindow(SW_HIDE));
		bMinimized=TRUE;
		return ;
	}
	if (!bSizeInitilaized) return;
	RECT WindowRect;//���ڿͻ�������
	RECT ClientRect;//���ھ���
	GetWindowRect(&WindowRect);
	GetClientRect(&ClientRect);

	if (nType==SIZE_RESTORED){
		setting.left=WindowRect.left;
		setting.top=WindowRect.top;
		setting.width=WindowRect.right-WindowRect.left;
		setting.height=WindowRect.bottom-WindowRect.top;
	}

	//��Ӧ״̬��
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);

	int xNowTop=ClientRect.bottom-ClientRect.top-sSStatus.cy-BorderSize;

	//�ƶ�״̬����̬�ı���
	SetWindowPosition(m_ProcessNumber,
		BorderSize,
		xNowTop
		);
	SetWindowPosition(m_CPU,
		sSStatus.cx+BorderSize*2,
		xNowTop
		);
	SetWindowPosition(m_RecvSpeed,
		sSStatus.cx*2+BorderSize*3,
		xNowTop
		);
	SetWindowPosition(m_SendSpeed,
		sSStatus.cx*3+BorderSize*7,
		xNowTop
		);
	SetWindowPosition(m_IO,
		sSStatus.cx*4+BorderSize*11,
		xNowTop
		);
	SetWindowPosition(m_Memory,
		sSStatus.cx*5+BorderSize*15,
		xNowTop
		);

	xNowTop-=sOneKeyKill.cy+BorderSize;

	//�ƶ�ѡ��ť
	SetWindowPosition(m_SelAll,
		BorderSize,
		xNowTop
		);
	SetWindowPosition(m_SelRev,
		sBSel.cx+BorderSize*2,
		xNowTop
		);
	SetWindowPosition(m_SelNoWhite,
		sBSel.cx*2+BorderSize*7,
		xNowTop
		);
	SetWindowPosition(m_SelBlack,
		sBSel.cx*2+sBSelList.cx+BorderSize*8,
		xNowTop
		);

	//�ƶ���һ����������ť
	SetWindowPosition(m_OneKeyKill,
		ClientRect.right-ClientRect.left-sOneKeyKill.cx-BorderSize,
		xNowTop
		);

	//�ƶ��б��
	SetWindowLayout(m_LstProc,BorderSize,BorderSize,
		ClientRect.right-ClientRect.left-BorderSize*2,
		xNowTop-BorderSize*2
		);

	if (bMinimized)
		Refresh();
	bMinimized=FALSE;
}

void CSuperTaskmgrDlg::OnMove(int x, int y)
{
	RECT WindowRect;//���ھ���
	CDialog::OnMove(x, y);
	if (!bSizeInitilaized) return;
	if (!this->IsIconic() && !this->IsZoomed()){
		GetWindowRect(&WindowRect);
		setting.left=WindowRect.left;
		setting.top=WindowRect.top;
		setting.width=WindowRect.right-WindowRect.left;
		setting.height=WindowRect.bottom-WindowRect.top;
	}
}

//֪ͨ����Ϣ����
LRESULT CSuperTaskmgrDlg::OnShow(WPARAM wParam,LPARAM lParam){
	OnShow();
	return 0;
}

//�ȼ���Ϣ����
LRESULT CSuperTaskmgrDlg::OnHotKey(WPARAM wParam,LPARAM lParam){
	if (wParam==IDH_SHOW)
		OnShow();
	return 0;
}

void CSuperTaskmgrDlg::OnDestroy()
{
	if (!InitSucceed) goto end;

	KillTimer(1);

	//��ע���ȼ�
	VERIFY(UnregisterHotKey(this->GetSafeHwnd(),IDH_SHOW));

	//��������
	RefreshColumnCFG();
	if (!SaveCFG()){
		ShowError(m_hWnd,AppName,_T("�޷��������ã�\n"),NULL,GetLastError());
	}

	//�ͷ����̶�̬ͼ��
	freeIconBitmap();

	//ɾ��������
	VERIFY(m_FloatingWindow.Destory());

end:
	//�ͷ���Դ
	VERIFY(MainTerm());

	CDialog::OnDestroy();
}

void CSuperTaskmgrDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	ASSERT(pPopupMenu != NULL);
	// check the enabled state of various menu items

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // parent == child for tracking popup
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	//CCmdTarget *pTarget = static_cast<CCmdTarget*>(this);

	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
	  state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(this, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, TRUE);
		}

		// adjust for menu deletions and additions
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}


void CAboutDlg::OnConnectMe()
{
	ShellExecute(this->GetSafeHwnd(),
		_T("open"),
		_T("tencent://message/?Menu=yes&uin=396519827&Site=&Service=201&sigT=ea6900e4512ad8b58da878037641291ed697bb55cec2278659d82fe191f63a4d7af5f0fcc717dd16c6679bc9244eafee&sigU=ec4e4c7844eb99e785d57bef70ca04391b3dcd69ad73da94e5a2c1b4943c71e8a23b2b48a797df4b"),
		NULL,
		lpszSystemRoot,
		SW_NORMAL
	);
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(0,WS_EX_DLGMODALFRAME);

	TCHAR szTitle[128];
	VERSION_NUMBER &ver=((CSuperTaskmgrApp*)AfxGetApp())->m_Version;
	_stprintf_s<128>(szTitle,_T("%s %d.%d.%d ��"),AppName,ver.dwMajor,ver.dwMinor,ver.dwBuild);
	m_Version.SetWindowText(szTitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BEGIN_MESSAGE_MAP(CButtonEx, CButton)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CButtonEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect =  lpDrawItemStruct->rcItem;
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	int nSaveDC=pDC->SaveDC();

	UINT state = lpDrawItemStruct->itemState;
	POINT pt ;
	TCHAR strText[MAX_PATH + 1];
	::GetWindowText(m_hWnd, strText, MAX_PATH);
	pt.x = 5;
	pt.y = 5;
	CPen m_BoundryPen;
	m_BoundryPen.CreatePen(PS_SOLID,4,RGB(0,0,0));
	CPen* hOldPen = pDC->SelectObject(&m_BoundryPen);
	pDC->RoundRect(&rect, pt);


	pDC->SelectObject(hOldPen);

	rect.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
	m_BoundryPen.DeleteObject();
}

void CButtonEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

BOOL CButtonEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return FALSE;
}


void CSuperTaskmgrDlg::OnStnClickedStaticMemory()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

