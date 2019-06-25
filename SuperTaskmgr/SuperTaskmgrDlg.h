// SuperTaskmgrDlg.h : 头文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// CSuperTaskmgrDlg 对话框
#define __CSuperTaskmgrDlg
class CSuperTaskmgrDlg  : public CDialog
{
public:
	static CSuperTaskmgrDlg *pcThis;
// 构造
public:
	CSuperTaskmgrDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSuperTaskmgrDlg();

// 对话框数据
	enum { IDD = IDD_SUPERTASKMGR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

//控件属性
private:
	BOOL bSizeInitilaized;
	static const int BorderSize;
	SIZE sOneKeyKill;//“一键结束”按钮尺寸
	SIZE sBSel;//“全选、反选”按钮尺寸
	SIZE sBSelList;//“非白名单、黑名单”按钮尺寸
	SIZE sSStatus; //状态栏静态文本控件的尺寸

//初始化
public:
	BOOL InitSucceed;//初始化是否成功
private:
	OSVERSIONINFO stOsVerInfo; //系统版本号
	DWORD dwCurrentProcessId; //当前进程的 ID
	DWORD dwPageSize; //内存的页大小
	DWORD dwSystemBits; //当前操作系统位数
	HFONT hFontBold,hFontNomal; //字体
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

//常量
public:
	static const COLUMN_ITEM_STRUCT ColumnItemName[cENUM_COLUMN_END];
private:
	static const SETTING DefaultSetting;//默认设置
	static LPTSTR HeadString_White,HeadString_Black,HeadString_System,
		DefaultSetting_WhiteListFile,DefaultSetting_BlackListFile,DefaultSetting_AllSystemProcessFile;
	LPTSTR DefaultSetting_SystemProcessFile;
	static const int TimerInterval[6];//刷新频率（毫秒）
	static const DWORD PriorityClass[7];
	static const DWORD KernelPriorityClass[32];
	static const UINT WM_TASKBARCREATED;

//设置项
private:
	LPTSTR sWorkDir,sWhiteListFile,sBlackListFile,sSystemProcessFile,sCFGFile;//设置文件路径
	CNewAlloc<TCHAR,512,2> FilePathContainer; //名单列表字符串容器
	static SETTING setting;
	friend class CSuperTaskmgrFloatingWindowDlg;
	CNewAlloc<TCHAR,1024,4> SPNameContainer; //名单列表字符串容器
	int ColumnOrd[cENUM_COLUMN_END];//列表头序号
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

//颜色
private:
	float fMaxColorCpu,fMaxColorNetSpeed,fMaxColorIo,fMaxColorMem; //最深颜色对应的最大值
	//状态栏
	COLORREF crCpuTotal,crIoTotal,crSendTotal,crRecvTotal,crMemTotal; //状态栏背景色

//托盘图标绘制
	CNotifyIconThread TrayNotifyIcon;
	Gdiplus::Bitmap *pobmNotifyIcon;
	Gdiplus::Graphics *pogNotifyIcon;
	Gdiplus::SolidBrush *pbrushBlue;
	Gdiplus::SolidBrush *pbrushRed;
	Gdiplus::SolidBrush *pbrushYellow;
	Gdiplus::SolidBrush *pbrushBack;
	Gdiplus::Pen *ppenFore;
	HICON hNotifyIcon;

//系统信息
private:
	SYSTEM_BASIC_INFORMATION SystemBasicInfo;
	SYSTEM_TIMEOFDAY_INFORMATION TimeOfDayInfo;
	SYSTEM_PERFORMANCE_INFORMATION PerformanceInfo;
	CDMAlloc<SYSTEM_PROCESSES**> pProcessInfo;int ProcessNumber;
	CDMAlloc<SYSTEM_ALL_HANDLE_INFORMATION *> pSystemHandleInfo;

//进程信息
public:
	LPPROCESS pro[MaxProcessNumber];int proN;
	CNewAlloc<PROCESS,128> ProcessContainer;
	CNewAlloc<TCHAR,16384> ProcessStringContainer;
private:
	//性能信息（全局）
	FLOAT fCPUUsage; //总CPU占用率
	LONGLONG qCPUTime; //总CPU时间

	LONGLONG qNetRecvSpeed;
	LONGLONG qNetSendSpeed;
	LONGLONG qNetTotalSpeed;

	LONGLONG qIoSpeed; //总I/O读写速度
	LONGLONG qReadTransferCount; //总I/O读取字节
	LONGLONG qWriteTransferCount; //总I/O写入字节
	LONGLONG qOtherTransferCount; //总I/O其它字节

	MEMORYSTATUSEX stMem;//内存信息


	//进程排序索引
	LPPROCESS pListPro[MaxProcessNumber];int nListItemCount;

	int SortColIndex;//排序列表头序号（-1表示默认）
	static ENUM_COLUMN SortColItem;//排序列表头类型序号
	int LastSortColIndex;//上一次排序列表头序号（用于重复点击同一个列表头是否逆序）
	static int RevSort;//是否逆序

	DWORD dwCSRSS_PID;
	DWORD GetCsrssPID();
	HANDLE GetProcessHandle(DWORD ProcessID);
	BOOL KillProcess(DWORD PID);

//网络驱动
	static C360TdiFilter TdiFilter;
	PROCESS_FLOW stProcessFlow;
	PROCESS_LIMITED stProcessLimited;
	GLOBAL_FLOW stGlobalFlow;
	CLimitSpeedDlg LimitSpeedDlg;
	friend class CLimitSpeedDlg;

//菜单命令
	void ListMenuCommand(BOOL &bSelAllVar,BOOL bChangeSelected,BOOL bInListSelect,LPTSTR *List,int &ListN,INT_PTR iProStOffset);
	BOOL bSelAllWhite,bSelAllBlack,bSelAllSystem;
	inline void ListUpdateMenu(BOOL &bSelAllVar,INT_PTR iProStOffset);
	static DWORD WINAPI WaitEditList(LPVOID lpParam);
	void NotepadEditListFile(LPTSTR szListFile,BOOL *lpbListSign);
	CProcess NotepadProcess;
	CThread WaitNotepadThread;

//刷新列表
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

//列表信息
	CListDispInfo m_ListInfo;
	static BOOL WINAPI GetCellString(int,int,LPCTSTR*,LPTSTR);

//
	int xRightColumn; //当前右键单击的列表头

//项目颜色
	void GetItemColor(DWORD dwRow,DWORD dwCol,COLORREF &crText,COLORREF &crBack,HFONT &hFont);

//控件变量
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


