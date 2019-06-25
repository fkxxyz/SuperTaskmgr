// SuperTaskmgr.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "SuperTaskmgrWindowData.h"


// CSuperTaskmgrApp:
// 有关此类的实现，请参阅 SuperTaskmgr.cpp
//

class CSuperTaskmgrApp : public CWinApp
{
public:
	CSuperTaskmgrApp();

// 重写
	public:
	virtual BOOL InitInstance();

//版本信息
public:
	VERSION_NUMBER m_Version;

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSuperTaskmgrApp theApp;
extern LPCTSTR AppName;
extern LPCTSTR frmClassName;
extern TCHAR lpszSystemRoot[];




