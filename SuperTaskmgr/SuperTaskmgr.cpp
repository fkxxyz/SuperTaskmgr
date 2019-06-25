// SuperTaskmgr.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


LPCTSTR AppName=WINDOW_APP_NAME;
LPCTSTR frmClassName=WINDOW_CLASS_NAME;
TCHAR lpszSystemRoot[MAX_PATH];//ϵͳĿ¼·��

// CSuperTaskmgrApp

BEGIN_MESSAGE_MAP(CSuperTaskmgrApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSuperTaskmgrApp ����

CSuperTaskmgrApp::CSuperTaskmgrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSuperTaskmgrApp ����

CSuperTaskmgrApp theApp;

// CSuperTaskmgrApp ��ʼ��


BOOL CSuperTaskmgrApp::InitInstance()
{
	CWinApp::InitInstance();
	//_CrtSetBreakAlloc(874);
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//�������ô�������
	WNDCLASS wc;
	if (!::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc)){
		ShowError(0,AppName,_T("��ʼ��ʱ�����޷�������ȡʵ������Ϣ��\n"),NULL,GetLastError(),MB_OK | MB_ICONERROR);
		ASSERT(0);
		return FALSE;
	}
	wc.lpszClassName = frmClassName;
	if (!AfxRegisterClass(&wc)){
		ShowError(0,AppName,_T("��ʼ��ʱ�����޷�ע��ʵ���ࣺ\n"),NULL,GetLastError(),MB_OK | MB_ICONERROR);
		ASSERT(0);
		return FALSE;
	}

	if (!GetAppVersion(&m_Version)){
		ASSERT(0);
		return FALSE;
	}

	//��ʼ�� GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
	ULONG_PTR gdiplusToken; 
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	//ͨ��������ȷ��ֻ����һ��ʵ��
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
		ShowError(0,AppName,_T("��ʼ��ʱ�����޷����������壺\n"),NULL,GetLastError(),MB_OK | MB_ICONERROR);
		ASSERT(0);
		return FALSE;
	}

	//����ʵ��
	CSuperTaskmgrDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	//�ͷ� GDI+
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


