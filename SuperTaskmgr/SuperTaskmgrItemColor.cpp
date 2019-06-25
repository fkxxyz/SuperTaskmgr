
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrItemColor.h"

//获取列表中某一项的字体颜色和背景
void CSuperTaskmgrDlg::GetItemColor(DWORD dwRow,DWORD dwCol,COLORREF &crText,COLORREF &crBack,HFONT &hFont){
	int a;
	float fb;
	switch (setting.column[dwCol].cindex){
	case cProcessName: //进程名称
		if (pListPro[dwRow]->inBlack)
			crText=RGB(255,0,0);
		else if (pListPro[dwRow]->inWhite||pListPro[dwRow]->inSystem)
			crText=RGB(0,0,0);
		else
			crText=RGB(0,0,255);

		hFont=pListPro[dwRow]->inSystem ? hFontBold : hFontNomal;
		crBack=RGB(255,255,255);
		if (pListPro[dwRow]->isSuspended) crBack = RGB(234,234,234);
		break;
	case cCPU: //CPU 使用率
		crText=RGB(0,0,0);
		fb=pListPro[dwRow]->CPU;
		if (pListPro[dwRow]->PID==0)
			a=255;
		else
			a=(fMaxColorCpu>fb) ?
				(int)(Sqare(fMaxColorCpu-fb)*255.0f/(fMaxColorCpu*fMaxColorCpu)):
			0;
		ASSERT(a>=0 && a<=255);
		crBack=RGB(255,a,a);
		hFont=hFontNomal;
		break;
	case cNetRecvSpeed://网络下载速率
		fb=(float)pListPro[dwRow]->NetRecvSpeed;
		goto nscalc;
	case cNetSendSpeed: //网络上传速率
		fb=(float)pListPro[dwRow]->NetSendSpeed;
		goto nscalc;
	case cNetTotalSpeed: //网络传输速率
		fb=(float)pListPro[dwRow]->NetTotalSpeed;
		goto nscalc;
nscalc:
		if (fb<0.0l) fb=0.0l;
		a=(fMaxColorNetSpeed>fb) ?
			(int)(Sqare(fMaxColorNetSpeed-fb)*255.0f/(fMaxColorNetSpeed*fMaxColorNetSpeed)):
			0;
		ASSERT(a>=0 && a<=255);
		crText=RGB(0,0,0);
		if (pListPro[dwRow]->bDisabledNetwork)
			crBack = RGB(223,223,223);
		else
			crBack=RGB(255,a,255);
		hFont=hFontNomal;
		break;
	case cIOSpeed: //IO读写速度占用
		fb=(float)pListPro[dwRow]->IOSpeed;
		a=(fMaxColorIo>fb) ?
			(int)(Sqare(fMaxColorIo-fb)*255.0f/(fMaxColorIo*fMaxColorIo)):
			0;
		ASSERT(a>=0 && a<=255);
		crText=RGB(0,0,0);
		crBack=RGB(a,a,255);
		hFont=hFontNomal;
		break;
	case cPrivateWorkingSet: //专用工作集
		fb=(float)pListPro[dwRow]->PrivateWorkingSet;
		goto memcalc;
	case cPeakVirtualSize: //虚拟内存峰值
		fb=(float)pListPro[dwRow]->PeakVirtualSize;
		goto memcalc;
	case cVirtualSize: //虚拟内存
		fb=(float)pListPro[dwRow]->VirtualSize;
		goto memcalc;
	case cPeakWorkingSetSize: //内存峰值
		fb=(float)pListPro[dwRow]->PeakWorkingSetSize;
		goto memcalc;
	case cWorkingSetSize: //内存
		fb=(float)pListPro[dwRow]->WorkingSetSize;
		goto memcalc;
	case cQuotaPeakPagedPoolUsage: //页面缓冲池峰值
		fb=(float)pListPro[dwRow]->QuotaPeakPagedPoolUsage;
		goto memcalc;
	case cQuotaPagedPoolUsage: //页面缓冲池
		fb=(float)pListPro[dwRow]->QuotaPagedPoolUsage;
		goto memcalc;
	case cQuotaPeakNonPagedPoolUsage: //非页面缓冲池峰值
		fb=(float)pListPro[dwRow]->QuotaPeakNonPagedPoolUsage;
		goto memcalc;
	case cQuotaNonPagedPoolUsage: //非页面缓冲池
		fb=(float)pListPro[dwRow]->QuotaNonPagedPoolUsage;
		goto memcalc;
	case cPageFileUsage: //页面文件
		fb=(float)pListPro[dwRow]->PageFileUsage;
		goto memcalc;
	case cPeakPageFileUsage: //页面文件峰值
		fb=(float)pListPro[dwRow]->PeakPageFileUsage;
		goto memcalc;
	case cPrivateUsage: //虚拟内存(任务管理器)
		fb=(float)pListPro[dwRow]->PrivateUsage;

memcalc:
		a=(fMaxColorMem>fb) ?
			(int)(Sqare(fMaxColorMem-fb)*255.0f/(fMaxColorMem*fMaxColorMem)):
			0;
		ASSERT(a>=0 && a<=255);
		crText=RGB(0,0,0);
		crBack=RGB(255,255,a);
		hFont=hFontNomal;
		break;
	default:
		crText=RGB(0,0,0);
		crBack=RGB(255,255,255);
		hFont=hFontNomal;
	}
	//SelectObject(hdc,hFont);
}

//获取状态栏静态文本框的字体颜色和背景
HBRUSH CSuperTaskmgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何属性
	switch (pWnd->GetDlgCtrlID()){
	case IDC_STATIC_CPU:
		pDC->SetBkColor(crCpuTotal);
		break;
	case IDC_STATIC_SEND_SPEED:
		pDC->SetBkColor(crSendTotal);
		break;
	case IDC_STATIC_RECV_SPEED:
		pDC->SetBkColor(crRecvTotal);
		break;
	case IDC_STATIC_IO:
		pDC->SetBkColor(crIoTotal);
		break;
	case IDC_STATIC_MEMORY:
		pDC->SetBkColor(crMemTotal);
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

