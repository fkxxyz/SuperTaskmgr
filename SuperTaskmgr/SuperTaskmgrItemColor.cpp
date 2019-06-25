
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrItemColor.h"

//��ȡ�б���ĳһ���������ɫ�ͱ���
void CSuperTaskmgrDlg::GetItemColor(DWORD dwRow,DWORD dwCol,COLORREF &crText,COLORREF &crBack,HFONT &hFont){
	int a;
	float fb;
	switch (setting.column[dwCol].cindex){
	case cProcessName: //��������
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
	case cCPU: //CPU ʹ����
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
	case cNetRecvSpeed://������������
		fb=(float)pListPro[dwRow]->NetRecvSpeed;
		goto nscalc;
	case cNetSendSpeed: //�����ϴ�����
		fb=(float)pListPro[dwRow]->NetSendSpeed;
		goto nscalc;
	case cNetTotalSpeed: //���紫������
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
	case cIOSpeed: //IO��д�ٶ�ռ��
		fb=(float)pListPro[dwRow]->IOSpeed;
		a=(fMaxColorIo>fb) ?
			(int)(Sqare(fMaxColorIo-fb)*255.0f/(fMaxColorIo*fMaxColorIo)):
			0;
		ASSERT(a>=0 && a<=255);
		crText=RGB(0,0,0);
		crBack=RGB(a,a,255);
		hFont=hFontNomal;
		break;
	case cPrivateWorkingSet: //ר�ù�����
		fb=(float)pListPro[dwRow]->PrivateWorkingSet;
		goto memcalc;
	case cPeakVirtualSize: //�����ڴ��ֵ
		fb=(float)pListPro[dwRow]->PeakVirtualSize;
		goto memcalc;
	case cVirtualSize: //�����ڴ�
		fb=(float)pListPro[dwRow]->VirtualSize;
		goto memcalc;
	case cPeakWorkingSetSize: //�ڴ��ֵ
		fb=(float)pListPro[dwRow]->PeakWorkingSetSize;
		goto memcalc;
	case cWorkingSetSize: //�ڴ�
		fb=(float)pListPro[dwRow]->WorkingSetSize;
		goto memcalc;
	case cQuotaPeakPagedPoolUsage: //ҳ�滺��ط�ֵ
		fb=(float)pListPro[dwRow]->QuotaPeakPagedPoolUsage;
		goto memcalc;
	case cQuotaPagedPoolUsage: //ҳ�滺���
		fb=(float)pListPro[dwRow]->QuotaPagedPoolUsage;
		goto memcalc;
	case cQuotaPeakNonPagedPoolUsage: //��ҳ�滺��ط�ֵ
		fb=(float)pListPro[dwRow]->QuotaPeakNonPagedPoolUsage;
		goto memcalc;
	case cQuotaNonPagedPoolUsage: //��ҳ�滺���
		fb=(float)pListPro[dwRow]->QuotaNonPagedPoolUsage;
		goto memcalc;
	case cPageFileUsage: //ҳ���ļ�
		fb=(float)pListPro[dwRow]->PageFileUsage;
		goto memcalc;
	case cPeakPageFileUsage: //ҳ���ļ���ֵ
		fb=(float)pListPro[dwRow]->PeakPageFileUsage;
		goto memcalc;
	case cPrivateUsage: //�����ڴ�(���������)
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

//��ȡ״̬����̬�ı����������ɫ�ͱ���
HBRUSH CSuperTaskmgrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  �ڴ˸��� DC ���κ�����
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
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

