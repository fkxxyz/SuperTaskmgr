// SuperTaskmgrDlg.h : ͷ�ļ�
//

#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include <GdiPlus.h>
#pragma comment(lib,"gdiplus.lib")

#include "QazLibrary\ListDispInfo.h"
#include "MFCLibrary\SuperListCtrl.h"
#include "WinLibrary\WinNotifyIcon.h"
#include "WinLibrary\360Netmon.h"

#include "SuperTaskmgrConfig.h"
#include "SuperTaskmgrRefresh.h"
#include "SuperTaskmgrProcess.h"
#include "SuperTaskmgrCommand.h"
#include "SuperTaskmgrFloatingWindowDlg.h"
#include "LimitSpeedDlg.h"

extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);

#define MaxProcessNumber 4096

#define IDH_SHOW 4001

#define MainNotifyID 1


class CButtonEx : public CButton {
	
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
protected:
	virtual void PreSubclassWindow();
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnConnectMe();
public:
	virtual BOOL OnInitDialog();
public:
	CStatic m_Version;
};

// CSuperTaskmgrDlg �Ի���
#define __CSuperTaskmgrDlg
class CSuperTaskmgrDlg  : public CDialog
{
public:
	static CSuperTaskmgrDlg *pcThis;
// ����
public:
	CSuperTaskmgrDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSuperTaskmgrDlg();

// �Ի�������
	enum { IDD = IDD_SUPERTASKMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//�ؼ�����
private:
	BOOL bSizeInitilaized;
	static const int BorderSize;
	SIZE sOneKeyKill;//��һ����������ť�ߴ�
	SIZE sBSel;//��ȫѡ����ѡ����ť�ߴ�
	SIZE sBSelList;//���ǰ�����������������ť�ߴ�
	SIZE sSStatus; //״̬����̬�ı��ؼ��ĳߴ�

//��ʼ��
public:
	BOOL InitSucceed;//��ʼ���Ƿ�ɹ�
private:
	OSVERSIONINFO stOsVerInfo; //ϵͳ�汾��
	DWORD dwCurrentProcessId; //��ǰ���̵� ID
	DWORD dwPageSize; //�ڴ��ҳ��С
	DWORD dwSystemBits; //��ǰ����ϵͳλ��
	HFONT hFontBold,hFontNomal; //����
	HICON hIconNull;
	BOOL initGetWorkDir();
	void initString();
	BOOL initCurrentProcessInfo();
	void initConstant();
	BOOL initIconBitmap();
	BOOL RefreshNotifyIcon(FLOAT fCpuUsage,LONGLONG qNetTotalSpeed,LONGLONG qIoSpeed,int iMemPercent);
	void freeIconBitmap();
	BOOL initDriver();
	BOOL MainInit();
	void FreeAll();
	BOOL MainTerm();

//����
public:
	static const COLUMN_ITEM_STRUCT ColumnItemName[cENUM_COLUMN_END];
private:
	static const SETTING DefaultSetting;//Ĭ������
	static LPTSTR HeadString_White,HeadString_Black,HeadString_System,
		DefaultSetting_WhiteListFile,DefaultSetting_BlackListFile,DefaultSetting_AllSystemProcessFile;
	LPTSTR DefaultSetting_SystemProcessFile;
	static const int TimerInterval[6];//ˢ��Ƶ�ʣ����룩
	static const DWORD PriorityClass[7];
	static const DWORD KernelPriorityClass[32];
	static const UINT WM_TASKBARCREATED;

//������
private:
	LPTSTR sWorkDir,sWhiteListFile,sBlackListFile,sSystemProcessFile,sCFGFile;//�����ļ�·��
	CNewAlloc<TCHAR,512,2> FilePathContainer; //�����б��ַ�������
	static SETTING setting;
	friend class CSuperTaskmgrFloatingWindowDlg;
	CNewAlloc<TCHAR,1024,4> SPNameContainer; //�����б��ַ�������
	int ColumnOrd[cENUM_COLUMN_END];//�б�ͷ���
	BOOL bReadCFGFile,bReadWhiteList,bReadBlackList,bReadSystemList;

	void CopySetting(SETTING &Dest,const SETTING &Src);
	BOOL ReadListFile(LPCTSTR sFileName,LPTSTR *Dest,int &DestN,LPCTSTR szNoFileDefaultHeadString,LPCTSTR szNoFileDefaultString);
	BOOL WriteListFile(LPCTSTR sFileName,LPTSTR *Src,int &SrcN,LPCTSTR szHeadString);
	BOOL ReadCFGFile(LPCTSTR sFileName,SETTING &Dest,CONST SETTING &DefaultSetting);
	BOOL WriteCFGFile(LPCTSTR sFileName,const SETTING &Src,const int *ColumnOrd);
	BOOL LoadCFG(BOOL lDefault);
	BOOL SaveCFG();
	void RefreshColumnCFG();
	void InitListCtrl();
	BOOL ApplyCFG();

//��ɫ
private:
	float fMaxColorCpu,fMaxColorNetSpeed,fMaxColorIo,fMaxColorMem; //������ɫ��Ӧ�����ֵ
	//״̬��
	COLORREF crCpuTotal,crIoTotal,crSendTotal,crRecvTotal,crMemTotal; //״̬������ɫ

//����ͼ�����
	CNotifyIconThread TrayNotifyIcon;
	Gdiplus::Bitmap *pobmNotifyIcon;
	Gdiplus::Graphics *pogNotifyIcon;
	Gdiplus::SolidBrush *pbrushBlue;
	Gdiplus::SolidBrush *pbrushRed;
	Gdiplus::SolidBrush *pbrushYellow;
	Gdiplus::SolidBrush *pbrushBack;
	Gdiplus::Pen *ppenFore;
	HICON hNotifyIcon;

//ϵͳ��Ϣ
private:
	SYSTEM_BASIC_INFORMATION SystemBasicInfo;
	SYSTEM_TIMEOFDAY_INFORMATION TimeOfDayInfo;
	SYSTEM_PERFORMANCE_INFORMATION PerformanceInfo;
	CDMAlloc<SYSTEM_PROCESSES**> pProcessInfo;int ProcessNumber;
	CDMAlloc<SYSTEM_ALL_HANDLE_INFORMATION *> pSystemHandleInfo;

//������Ϣ
public:
	LPPROCESS pro[MaxProcessNumber];int proN;
	CNewAlloc<PROCESS,128> ProcessContainer;
	CNewAlloc<TCHAR,16384> ProcessStringContainer;
private:
	//������Ϣ��ȫ�֣�
	FLOAT fCPUUsage; //��CPUռ����
	LONGLONG qCPUTime; //��CPUʱ��

	LONGLONG qNetRecvSpeed;
	LONGLONG qNetSendSpeed;
	LONGLONG qNetTotalSpeed;

	LONGLONG qIoSpeed; //��I/O��д�ٶ�
	LONGLONG qReadTransferCount; //��I/O��ȡ�ֽ�
	LONGLONG qWriteTransferCount; //��I/Oд���ֽ�
	LONGLONG qOtherTransferCount; //��I/O�����ֽ�

	MEMORYSTATUSEX stMem;//�ڴ���Ϣ


	//������������
	LPPROCESS pListPro[MaxProcessNumber];int nListItemCount;

	int SortColIndex;//�����б�ͷ��ţ�-1��ʾĬ�ϣ�
	static ENUM_COLUMN SortColItem;//�����б�ͷ�������
	int LastSortColIndex;//��һ�������б�ͷ��ţ������ظ����ͬһ���б�ͷ�Ƿ�����
	static int RevSort;//�Ƿ�����

	DWORD dwCSRSS_PID;
	DWORD GetCsrssPID();
	HANDLE GetProcessHandle(DWORD ProcessID);
	BOOL KillProcess(DWORD PID);

//��������
	static C360TdiFilter TdiFilter;
	PROCESS_FLOW stProcessFlow;
	PROCESS_LIMITED stProcessLimited;
	GLOBAL_FLOW stGlobalFlow;
	CLimitSpeedDlg LimitSpeedDlg;
	friend class CLimitSpeedDlg;

//�˵�����
	void ListMenuCommand(BOOL &bSelAllVar,BOOL bChangeSelected,BOOL bInListSelect,LPTSTR *List,int &ListN,INT_PTR iProStOffset);
	BOOL bSelAllWhite,bSelAllBlack,bSelAllSystem;
	inline void ListUpdateMenu(BOOL &bSelAllVar,INT_PTR iProStOffset);
	static DWORD WINAPI WaitEditList(LPVOID lpParam);
	void NotepadEditListFile(LPTSTR szListFile,BOOL *lpbListSign);
	CProcess NotepadProcess;
	CThread WaitNotepadThread;

//ˢ���б�
private:
	static LPTSTR ConvULONGStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvTIMEStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvCPUStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvPriorityStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvSuspendStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvMEMMBStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvMEMKBStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvSpeedStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);
	static LPTSTR ConvNetSpeedStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro);

	static int CmpStr(LPVOID,LPVOID);
	static int CmpFloat(LPVOID,LPVOID);
	static int CmpQword(LPVOID,LPVOID);
	static int CmpDwordPos(LPVOID,LPVOID);
	static int CmpDwordInv(LPVOID,LPVOID);
	static int CmpPriority(LPVOID,LPVOID);

	static BOOL InList(LPTSTR *list,INT_PTR listN,LPCTSTR str);
	static int PriorityIndex(DWORD Priority);
	static int SortCmp(const void *p1,const void *p2);
	BOOL SortList();
	static LPTSTR AddNtUnicodeToStringContainer(
		CNewAlloc<TCHAR,16384> &Container,UNICODE_STRING &us,TCHAR szBuffer[]);
	BOOL RefreshProcessInfo();
	BOOL CalcGlobalPerformanceInfo();
	BOOL RefreshGlobalInfo();
	BOOL RefreshStatusBar(CStatusBar *statusbar);
	BOOL RefreshListInfo();
	BOOL RefreshList();
	BOOL RefreshFloatingWindow();
	afx_msg void Refresh(BOOL bForceFreshAll=FALSE);

	TCHAR szUpSpeed[32];
	TCHAR szDownSpeed[32];

//�б���Ϣ
	CListDispInfo m_ListInfo;
	static BOOL WINAPI GetCellString(int,int,LPCTSTR*,LPTSTR);

//
	int xRightColumn; //��ǰ�Ҽ��������б�ͷ

//��Ŀ��ɫ
	void GetItemColor(DWORD dwRow,DWORD dwCol,COLORREF &crText,COLORREF &crBack,HFONT &hFont);

//�ؼ�����
public:
	CSuperListCtrl m_LstProc;
	HACCEL hAccel;

	CSuperTaskmgrFloatingWindowDlg m_FloatingWindow;
	CStatusBar m_wndStatusBar;
	CImageList m_ImageList;
	CMenu m_Menu;
	CMenu m_MenuNotify;
	CButton m_OneKeyKill;
	CButton m_SelAll;
	CButton m_SelNoWhite;
	CButton m_SelRev;
	CButton m_SelBlack;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnRefresh();
public:
	afx_msg void OnNewTask();
public:
	afx_msg void OnKill();
public:
	afx_msg void OnNMClickLstproc(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickLstproc(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnColumnclickLstproc(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnItemchangedLstproc(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnSelAll();
public:
	afx_msg void OnBnClickedButtomSelAll();
public:
	afx_msg void OnBnClickedSelRev();
public:
	afx_msg void OnSelRev();
public:
	afx_msg void OnNMCustomdrawLstproc(NMHDR *pNMHDR, LRESULT *pResult);
	void OnLvnColumnRClickLstproc(int col);
public:
	afx_msg void OnBnClickedSelNotWhite();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedSelBlack();
public:
	afx_msg void OnSelNotWhite();
public:
	afx_msg void OnSelBlack();
public:
	afx_msg void OnBnClickedOnekeyKill();
public:
	afx_msg void OnUpdateKill(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateForceKill(CCmdUI *pCmdUI);
public:
	afx_msg void OnSuspend();
public:
	afx_msg void OnUpdateSuspend(CCmdUI *pCmdUI);
public:
	afx_msg void OnResume();
public:
	afx_msg void OnUpdateResume(CCmdUI *pCmdUI);
public:
	afx_msg void OnSetBlack();
public:
	afx_msg void OnSetSystem();
public:
	afx_msg void OnSetWhite();
public:
	afx_msg void OnUpdateSetBlack(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateSetSystem(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateSetWhite(CCmdUI *pCmdUI);
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
public:
	afx_msg void OnAttrib();
public:
	afx_msg void OnUpdateAttrib(CCmdUI *pCmdUI);
public:
	afx_msg void OnDirectory();
public:
	afx_msg void OnUpdateDirectory(CCmdUI *pCmdUI);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
public:
	afx_msg void OnMenuItemNull();
	afx_msg void OnUpdateMenuItemNull(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMenuItemPriority(CCmdUI* pCmdUI);
public:
	afx_msg void OnShowInFront();
public:
	afx_msg void OnUpdateShowInFront(CCmdUI *pCmdUI);
public:
	afx_msg void OnMinHide();
public:
	afx_msg void OnUpdateMinHide(CCmdUI *pCmdUI);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnShowSystem();
public:
	afx_msg void OnUpdateShowSystem(CCmdUI *pCmdUI);
public:
	afx_msg void OnLoadDefault();
public:
	afx_msg void OnMove(int x, int y);
public:
	afx_msg LRESULT OnNotify(WPARAM wParam,LPARAM lParam);
public:
	afx_msg LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnShow();
public:
	afx_msg LRESULT OnShow(WPARAM wParam,LPARAM lParam);
public:
	afx_msg LRESULT OnRefresh(WPARAM wParam,LPARAM lParam);
public:
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnAppAbout();
public:
	afx_msg void OnEditWhite();
public:
	afx_msg void OnEditBlack();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnHideThisColumn();
public:
	afx_msg void OnUpdateHideThisColumn(CCmdUI *pCmdUI);
public:
	afx_msg void OnSelectColumn();
public:
//	afx_msg void OnLvnEndScrollLstproc(NMHDR *pNMHDR, LRESULT *pResult);
public:
//	afx_msg void OnLvnGetdispinfoLstproc(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnWatchDataSend(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CStatic m_ProcessNumber;
public:
	CStatic m_CPU;
public:
	CStatic m_IO;
public:
	CStatic m_Memory;
public:
	CStatic m_SendSpeed;
public:
	CStatic m_RecvSpeed;
public:
	afx_msg void OnDisableNetwork();
public:
	afx_msg void OnUpdateDisableNetwork(CCmdUI *pCmdUI);
public:
	afx_msg void OnAllowNetwork();
public:
	afx_msg void OnUpdateAllowNetwork(CCmdUI *pCmdUI);
public:
	afx_msg void OnShowFloatingwindow();
public:
	afx_msg void OnUpdateShowFloatingwindow(CCmdUI *pCmdUI);
public:
	afx_msg void OnEmptyWorkingSet();
public:
//	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnUpdateEmptyWorkingSet(CCmdUI *pCmdUI);
	afx_msg void OnStnClickedStaticMemory();
	afx_msg void OnUpdateNotepadEdit(CCmdUI *pCmdUI);
//	afx_msg void OnLvnGetdispinfoLstproc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstproc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLimitSpeed();
	afx_msg void OnUpdateLimitSpeed(CCmdUI *pCmdUI);
};


