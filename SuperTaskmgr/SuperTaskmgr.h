// SuperTaskmgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SuperTaskmgrWindowData.h"


// CSuperTaskmgrApp:
// �йش����ʵ�֣������ SuperTaskmgr.cpp
//

class CSuperTaskmgrApp : public CWinApp
{
public:
	CSuperTaskmgrApp();

// ��д
	public:
	virtual BOOL InitInstance();

//�汾��Ϣ
public:
	VERSION_NUMBER m_Version;

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSuperTaskmgrApp theApp;
extern LPCTSTR AppName;
extern LPCTSTR frmClassName;
extern TCHAR lpszSystemRoot[];




