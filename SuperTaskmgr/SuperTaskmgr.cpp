// SuperTaskmgr.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LPCTSTR AppName=WINDOW_APP_NAME;
LPCTSTR frmClassName=WINDOW_CLASS_NAME;
TCHAR lpszSystemRoot[MAX_PATH];//系统目录路径

// CSuperTaskmgrApp

BEGIN_MESSAGE_MAP(CSuperTaskmgrApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSuperTaskmgrApp 构造

CSuperTaskmgrApp::CSuperTaskmgrApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSuperTaskmgrApp 对象

CSuperTaskmgrApp theApp;

// CSuperTaskmgrApp 初始化


BOOL CSuperTaskmgrApp::InitInstance()
{
	CWinApp::InitInstance();
	//_CrtSetBreakAlloc(874);
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//重新设置窗体类名
	WNDCLASS wc;
	if (!::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc)){
		ShowError(0,AppName,_T("初始化时出错！无法创建获取实例类信息：\n"),NULL,GetLastError(),MB_OK | MB_ICONERROR);
		ASSERT(0);
		return FALSE;
	}
	wc.lpszClassName = frmClassName;
	if (!AfxRegisterClass(&wc)){
		ShowError(0,AppName,_T("初始化时出错！无法注册实例类：\n"),NULL,GetLastError(),MB_OK | MB_ICONERROR);
		ASSERT(0);
		return FALSE;
	}

	if (!GetAppVersion(&m_Version)){
		ASSERT(0);
		return FALSE;
	}

	//初始化 GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
	ULONG_PTR gdiplusToken; 
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	//通过互斥体确保只运行一个实例
	CMutex hMutex;
	if (hMutex.Create(TRUE,AppName)){
		if (GetLastError()==ERROR_ALREADY_EXISTS){
			HWND hWindow;
			hWindow=::FindWindow(frmClassName,NULL);
			if (hWindow)
				::SendMessage(hWindow,WM_SHOW,0,0);
			return FALSE;
		}
	} else {
		ShowError(0,AppName,_T("初始化时出错！无法创建互斥体：\n"),NULL,GetLastError(),MB_OK | MB_ICONERROR);
		ASSERT(0);
		return FALSE;
	}

	//启动实例
	CSuperTaskmgrDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	//释放 GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}


