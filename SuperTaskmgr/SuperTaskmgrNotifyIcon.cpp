
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrNotifyIcon.h"

using namespace Gdiplus;

LRESULT CSuperTaskmgrDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam){
	TrayNotifyIcon.Add(MainNotifyID,WM_ONNOTIFY,NULL,AppName);
	RefreshNotifyIcon(0.0f,0l,0l,0);
	return 0;
}

BOOL CSuperTaskmgrDlg::initIconBitmap(){
	//初始化绘图对象
	pobmNotifyIcon=new Bitmap(16,16);
	pogNotifyIcon=Graphics::FromImage(pobmNotifyIcon);

	//初始化画笔、画刷
	pbrushBlue=new SolidBrush(Color(0,0,255));
	pbrushRed=new SolidBrush(Color(255,0,127));
	pbrushYellow=new SolidBrush(Color(128,128,11));
	pbrushBack=new SolidBrush(Color(11,11,11));
	ppenFore=new Pen(Color(0,128,0));

	//绘制图标框架
	Pen penWhite(Color(255,255,255));
	Pen penGray(Color(128,128,128));
	pogNotifyIcon->DrawLine(&penGray,2,1,13,1);
	pogNotifyIcon->DrawLine(&penWhite,2,13,13,13);
	pogNotifyIcon->DrawLine(&penGray,2,1,2,13);
	pogNotifyIcon->DrawLine(&penWhite,13,1,13,13);

	if (!TrayNotifyIcon.Create()) return FALSE;
	TrayNotifyIcon.SetHWnd(GetSafeHwnd());
	if (!TrayNotifyIcon.Add(MainNotifyID,WM_ONNOTIFY,NULL,AppName)) {ASSERT(0);return FALSE;}
	if (!TrayNotifyIcon.WaitForMessageFinished(32)) return FALSE;
	switch (TrayNotifyIcon.GetErrorCode()){case ERROR_SUCCESS:case 0x80004005:break;default:ASSERT(0);return FALSE;}
	if (!RefreshNotifyIcon(0.0f,0l,0l,0)) {return FALSE;}
	return TRUE;
}

BOOL CSuperTaskmgrDlg::RefreshNotifyIcon(FLOAT fCpuUsage,LONGLONG qNetTotalSpeed,LONGLONG qIoSpeed,int iMemPercent){
	int i;
	//绘制背景色
	pogNotifyIcon->FillRectangle(pbrushBack,3,2,10,11);

	//绘制内存占用率
	i=iMemPercent*12/100;
	if (i<0) i=0;
	if (i>11) i=11;
	pogNotifyIcon->FillRectangle(pbrushYellow,3,13-i,10,i);

	//绘制背景线条
	for (i=3;i<13;i+=2)
		pogNotifyIcon->DrawLine(ppenFore,3,i,12,i);
	for (i=4;i<13;i+=2)
		pogNotifyIcon->DrawLine(ppenFore,i,2,i,12);

	//绘制IO读写速度
	i=(int)(qIoSpeed/1024)*12/(1024*50);
	if (i<0) i=0;
	if (i>11) i=11;
	pogNotifyIcon->FillRectangle(pbrushBlue,3,13-i,10,i);

	//绘制CPU使用率
	i=(int)(fCpuUsage/8);
	if (i<0) i=0;
	if (i>11) i=11;
	pogNotifyIcon->FillRectangle(pbrushRed,3,13-i,10,i);

	//设置托盘图标
	if (hNotifyIcon) VERIFY(DestroyIcon(hNotifyIcon));
	Status ss=pobmNotifyIcon->GetHICON(&hNotifyIcon);

	TCHAR szTip[1024];
	_stprintf_s<1024>(szTip,
		_T("CPU %.1f%%\n")
		_T("内存 %d%%\n")
		_T("网速 %.1fKb/s\n")
		_T("IO %.1fKb/s\n"),
		fCpuUsage,iMemPercent,(float)qNetTotalSpeed/1024,(float)qIoSpeed/1024
		);

	BOOL result=TrayNotifyIcon.Modify(MainNotifyID,NULL,hNotifyIcon,szTip);
	if (result)
		result=TrayNotifyIcon.WaitForMessageFinished(32);
	if (result){
		switch (TrayNotifyIcon.GetErrorCode()){
		case ERROR_SUCCESS: case 0x80004005:
			break;
		default:
			ASSERT(0);
		}
	}

	return result;
}

void CSuperTaskmgrDlg::freeIconBitmap(){
	//删除通知栏图标
	TrayNotifyIcon.Delete(MainNotifyID);

	if (pbrushBlue) delete pbrushBlue;
	if (pbrushRed) delete pbrushRed;
	if (pbrushYellow) delete pbrushYellow;
	if (pbrushBack) delete pbrushBack;
	if (ppenFore) delete ppenFore;

	if (pogNotifyIcon) delete pogNotifyIcon;
	if (pobmNotifyIcon) delete pobmNotifyIcon;

	if (hNotifyIcon) VERIFY(DestroyIcon(hNotifyIcon));
}

