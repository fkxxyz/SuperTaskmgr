
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrRefresh.h"
#include "SuperTaskmgrItemColor.h"
#include "WinKillProcess.h"
#include <math.h>
#include <locale.h>

//��������ת����ULONG �� LPTSTR��
LPTSTR CSuperTaskmgrDlg::ConvULONGStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	_stprintf_s(szBuf,16,_T("%lu"),*(ULONG*)pItem);
	return szBuf;
}

//ʱ��ת����LONGLONG �� LPTSTR "x:xx:xx"��
LPTSTR CSuperTaskmgrDlg::ConvTIMEStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	ULONG_PTR h,m,s;
	h=(ULONG)(*(LONGLONG*)pItem/10000000);
	s=(INT_PTR)h%60;
	h/=60;
	m=(INT_PTR)h%60;
	h/=60;
	h=(INT_PTR)h;
	_stprintf_s(szBuf,64,_T("%u:%02u:%02u"),h,m,s);
	return szBuf;
}

//CPU ʹ����ת����FLOAT �� LPTSTR "xx.x %"��
LPTSTR CSuperTaskmgrDlg::ConvCPUStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	_stprintf_s(szBuf,16,_T("%.1f %%"),*(FLOAT*)pItem);
	return szBuf;
}

//���ȼ�ת����DWORD �� LPTSTR "��׼"��
LPTSTR CSuperTaskmgrDlg::ConvPriorityStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	switch (*(DWORD*)pItem){
	case IDLE_PRIORITY_CLASS:
		return _T("��");
	case BELOW_NORMAL_PRIORITY_CLASS:
		return _T("�ϵ�");
		break;
	case NORMAL_PRIORITY_CLASS:
		return _T("��׼");
		break;
	case ABOVE_NORMAL_PRIORITY_CLASS:
		return _T("�ϸ�");
		break;
	case HIGH_PRIORITY_CLASS:
		return _T("��");
		break;
	case REALTIME_PRIORITY_CLASS:
		return _T("ʵʱ");
		break;
	default:
		return _T("δ֪");
	}
	return szBuf;
}

//�����Ƿ�����ת����BOOL �� LPTSTR "����"��
LPTSTR CSuperTaskmgrDlg::ConvSuspendStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	return *(BOOL*)pItem ? _T("����") : _T("����");
}

//�ڴ�ת����ULONG �� LPTSTR "xxx,xxx,xxx.x MB"��
LPTSTR CSuperTaskmgrDlg::ConvMEMMBStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	if (*(SSIZE_T*)pItem<0){
		szBuf[0]=_T('-');
		szBuf[1]=_T('\0');
		return szBuf;
	}
	FLOAT fItem=(FLOAT)*(SIZE_T*)pItem;
	fItem/=1024*1024;
	_stprintf_s(szBuf,32,_T("%.1f MB"),fItem);
	return szBuf;
}

//�ڴ�ת����ULONG �� LPTSTR "xxx,xxx,xxx KB"��
LPTSTR CSuperTaskmgrDlg::ConvMEMKBStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	if (*(SSIZE_T*)pItem<0){
		szBuf[0]=_T('-');
		szBuf[1]=_T('\0');
		return szBuf;
	}
	FLOAT fItem=(FLOAT)*(SIZE_T*)pItem;
	fItem/=1024;
	_stprintf_s(szBuf,32,_T("%.1f KB"),fItem);
	return szBuf;
}

//�ٶ�ת����LONGLONG �� LPTSTR "xxx.x MB/s"��
LPTSTR CSuperTaskmgrDlg::ConvSpeedStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	FLOAT fItem=(FLOAT)*(LONGLONG*)pItem;
	if (fItem<0.0f){
		szBuf[0]=_T('-');
		szBuf[1]=_T('\0');
	} else if (fItem<1000.0f)
		_stprintf_s(szBuf,32,_T("%.0f B/s"),fItem);
	else {
		fItem/=1024.0f;
		if (fItem<1000.0f)
			_stprintf_s(szBuf,32,_T("%.1f KB/s"),fItem);
		else {
			fItem/=1024.0f;
			if (fItem<1000.0f)
				_stprintf_s(szBuf,32,_T("%.1f MB/s "),fItem);
			else {
				fItem/=1024.0f;
				if (fItem<1000.0f)
					_stprintf_s(szBuf,32,_T("%.1f GB/s  "),fItem);
				else {
					fItem/=1024.0f;
					if (fItem<1000.0f)
						_stprintf_s(szBuf,32,_T("%.1f TB/s   "),fItem);
				}
			}
		}
	}
	return szBuf;
}

LPTSTR CSuperTaskmgrDlg::ConvNetSpeedStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	if (ppro->bDisabledNetwork){
		_tcscpy_s(szBuf,32,_T("�ѽ�ֹ"));
		return szBuf;
	}
	return ConvSpeedStr(szBuf,pItem,ppro);
}

int CSuperTaskmgrDlg::CmpStr(LPVOID p1,LPVOID p2){
	return _tcscmp(*(LPTSTR*)p1,*(LPTSTR*)p2);
}
int CSuperTaskmgrDlg::CmpFloat(LPVOID p1,LPVOID p2){
	return *(float*)p2>*(float*)p1 ? 1 : -1;
}
int CSuperTaskmgrDlg::CmpQword(LPVOID p1,LPVOID p2){
	return *(LONGLONG*)p2>*(LONGLONG*)p1 ? 1 : -1;
}
int CSuperTaskmgrDlg::CmpDwordPos(LPVOID p1,LPVOID p2){
	return *(LONG*)p1>*(LONG*)p2 ? 1 : -1;
}
int CSuperTaskmgrDlg::CmpDwordInv(LPVOID p1,LPVOID p2){
	return *(LONG*)p1>*(LONG*)p2 ? -1 : 1;
}
int CSuperTaskmgrDlg::CmpPriority(LPVOID p1,LPVOID p2){
	return PriorityIndex(*(DWORD*)p2)-PriorityIndex(*(DWORD*)p1);
}

//�ж� str �Ƿ��� list �б���
BOOL CSuperTaskmgrDlg::InList(LPTSTR *list,INT_PTR listN,LPCTSTR str){
	INT_PTR i;
	for (i=0;i<listN;i++)
		if (_tcsicmp(str,list[i])==0)
			return TRUE;
	return FALSE;
}


int CSuperTaskmgrDlg::PriorityIndex(DWORD Priority){
	switch (Priority){
	case 0: //δ֪
		return 0;
	case IDLE_PRIORITY_CLASS: //��
		return 1;
	case BELOW_NORMAL_PRIORITY_CLASS: //�ϵ�
		return 2;
	case NORMAL_PRIORITY_CLASS: //��׼
		return 3;
	case ABOVE_NORMAL_PRIORITY_CLASS: //�ϸ�
		return 4;
	case HIGH_PRIORITY_CLASS: //��
		return 5;
	case REALTIME_PRIORITY_CLASS: //ʵʱ
		return 6;
	}
	return 0;
}

inline int sgn(int x){
	if (x>0)
		return 1;
	if (x=0)
		return 0;
	if (x<0)
		return -1;
	return 0;
}

//qsort ���ź����Ļص�����
int CSuperTaskmgrDlg::SortCmp(const void *p1,const void *p2){
	return ColumnItemName[SortColItem].lpfunCmp(
		(LPVOID)((INT_PTR)*(LPPROCESS*)p1+ColumnItemName[SortColItem].lpvDataOffset),
		(LPVOID)((INT_PTR)*(LPPROCESS*)p2+ColumnItemName[SortColItem].lpvDataOffset)
	)*RevSort;
}

//�����б��е���Ŀ
BOOL CSuperTaskmgrDlg::SortList(){
	int i;
	if (setting.ShowSystemProcess){
		for (nListItemCount=0;nListItemCount<proN;nListItemCount++)
			pListPro[nListItemCount]=pro[nListItemCount];
	} else {
		nListItemCount=0;
		for (i=0;i<proN;i++){
			if (!pro[i]->inSystem){
				pListPro[nListItemCount++]=pro[i];
			}
		}
	}

	if (SortColIndex==-1){
	} else {
		setlocale(LC_COLLATE, "");
		qsort(pListPro,nListItemCount,sizeof(*pListPro),&SortCmp);
	}
	return TRUE;
}


BOOL CSuperTaskmgrDlg::CalcGlobalPerformanceInfo(){

	//���������һ��ˢ��ʱ����
	static LONGLONG LastTime; //��һ��ˢ��ȫ�����ܵ�ʱ��
	LONGLONG llTimeSpan;
	if (LastTime==0l)
		llTimeSpan=0l;
	else
		llTimeSpan=TimeOfDayInfo.CurrentTime.QuadPart-LastTime;
	LastTime=TimeOfDayInfo.CurrentTime.QuadPart;

	//����ȫ��CPUʹ���ʡ�IO��д�ٶ�
	fCPUUsage=0.0f;
	qIoSpeed=0l;
	if (llTimeSpan){
		if (qCPUTime!=0l){
			//����CPUʹ����
			fCPUUsage =	100.0f-(FLOAT)(PerformanceInfo.IdleProcessTime.QuadPart-qCPUTime)*100.0f
						/(FLOAT)(SystemBasicInfo.NumberOfProcessors*llTimeSpan);
			if (fCPUUsage<0.0f)
				fCPUUsage=0.0f;
			if (fCPUUsage>100.0f)
				fCPUUsage=100.0f;
		}

		//���� IO ��д�ٶ�
		qIoSpeed=	(
						(
							(PerformanceInfo.IoReadTransferCount.QuadPart-qReadTransferCount)+
							(PerformanceInfo.IoWriteTransferCount.QuadPart-qWriteTransferCount)+
							(PerformanceInfo.IoOtherTransferCount.QuadPart-qOtherTransferCount)
						)*1000/(ULONG)(llTimeSpan/10000)
					);
		if (qIoSpeed<0l)
			qIoSpeed=0l;
	}
	qCPUTime=PerformanceInfo.IdleProcessTime.QuadPart;
	qReadTransferCount=PerformanceInfo.IoReadTransferCount.QuadPart;
	qWriteTransferCount=PerformanceInfo.IoWriteTransferCount.QuadPart;
	qOtherTransferCount=PerformanceInfo.IoOtherTransferCount.QuadPart;

	qNetSendSpeed=stGlobalFlow.SendSpeed;
	qNetRecvSpeed=stGlobalFlow.RecvSpeed;
	qNetTotalSpeed=qNetSendSpeed+qNetRecvSpeed;


	//���������ɫʱ��ֵ
	if (fCPUUsage<10.0f){
		fMaxColorCpu=fCPUUsage*8+1.0f;
	} else if (fCPUUsage<25.0f){
		fMaxColorCpu=fCPUUsage*4;
	} else if (fCPUUsage<50.0f){
		fMaxColorCpu=fCPUUsage*2;
	} else {
		fMaxColorCpu=100.0f;
	}
	if (qNetTotalSpeed<1024l){ //С�� 1 KB/s
		fMaxColorNetSpeed=(float)(qNetTotalSpeed*32+1);
	} else if (qNetTotalSpeed<1024l*100l){ //С�� 100 KB/s
		fMaxColorNetSpeed=(float)(qNetTotalSpeed*8);
	} else if (qNetTotalSpeed<1024l*1024l*100l){ //С�� 1 MB/s
		fMaxColorNetSpeed=(float)(qNetTotalSpeed*2);
	} else {
		fMaxColorNetSpeed=(float)(qNetTotalSpeed);
	}
	if (qIoSpeed<1024l){ //С�� 1 KB/s
		fMaxColorIo=(float)(qIoSpeed*32+1);
	} else if (qIoSpeed<1024l*1024l){ //С�� 1 MB/s
		fMaxColorIo=(float)(qIoSpeed*8);
	} else if (qIoSpeed<1024l*1024l*100l){ //С�� 100 MB/s
		fMaxColorIo=(float)(qIoSpeed*2);
	} else {
		fMaxColorIo=(float)(qIoSpeed);
	}
	return TRUE;
}

//ˢ��ȫ��������Ϣ
BOOL CSuperTaskmgrDlg::RefreshGlobalInfo(){
	VERIFY(NT_SUCCESS(NtQueryTimeOfDayInfo(&TimeOfDayInfo)));
	VERIFY(NT_SUCCESS(NtQueryPerformanceInfo(&PerformanceInfo)));
	VERIFY(TdiFilter.GetGlobalFlow(&stGlobalFlow));
	VERIFY(GlobalMemoryStatusEx(&stMem));
	return CalcGlobalPerformanceInfo();
}

#define MAX_BUF 32767
inline LPTSTR CSuperTaskmgrDlg::AddNtUnicodeToStringContainer(
	CNewAlloc<TCHAR,16384> &Container,UNICODE_STRING &us,TCHAR szBuffer[]
	)
{
	int cchChar=us.Length/2;
	LPTSTR result;
#ifdef UNICODE
	result=Container.NewData(us.Buffer,cchChar+1);
#else
	cchChar=WideCharToMultiByte(CP_ACP,0,us.Buffer,cchChar,szBuffer,MAX_BUF,"?",NULL);
	result=Container.NewData(szBuffer,cchChar+1);
#endif
	result[cchChar]=_T('\0');
	return result;
}

//ˢ�½�����Ϣ
BOOL CSuperTaskmgrDlg::RefreshProcessInfo(){
	static LPCTSTR EmptyStr=_T("");

	//��ȡ���̡�ʱ�䡢����������Ϣ
	if (!NT_SUCCESS(NtQueryProcessInfo(pProcessInfo,&ProcessNumber)))
		return FALSE;
	if (!NT_SUCCESS(NtQueryTimeOfDayInfo(&TimeOfDayInfo)))
		return FALSE;
	if (!NT_SUCCESS(NtQueryPerformanceInfo(&PerformanceInfo)))
		return FALSE;
	VERIFY(GlobalMemoryStatusEx(&stMem));

	//��������������Ϣ
	VERIFY(TdiFilter.GetProcessFlow(&stProcessFlow));

	//���������һ��ˢ��ʱ����
	static LONGLONG LastTime=0l; //��һ�ηֱ�ˢ�����н��̵�ʱ��
	ULONG lTimeSpan; //������һ��ˢ�µ�ʱ���� (��λ��0.1����)
	LONGLONG llTimeSpan; //������һ��ˢ�µ�ʱ���� (��λ��10����)
	if (LastTime==0l) {
		llTimeSpan=0l;
		lTimeSpan=0;
	} else
		lTimeSpan=(ULONG)((llTimeSpan=TimeOfDayInfo.CurrentTime.QuadPart-LastTime)/10000);
	LastTime=TimeOfDayInfo.CurrentTime.QuadPart;

	//�ӽ���������ɾ���Ѿ������ڵĽ���
	int i,j;
	for (i=0;i<proN;i++){
		PROCESS &npro=*pro[i];
		for (j=0;j<ProcessNumber;j++){
			if (pro[i]->PID==(DWORD)(DWORD_PTR)pProcessInfo[j]->ProcessId)
				break;
		}
		if (j==ProcessNumber){//������̲�����
			proN--;

			if (npro.Process)
				npro.Process.Close();
			if (npro.hIcon)
				DestroyIcon(npro.hIcon);
			//���������н���ǰ��
			for (j=i;j<proN;j++)
				pro[j]=pro[j+1];
			i--;
		}
	}

	//��ʼˢ����Ϣ
	memset(&stGlobalFlow,0,sizeof(stGlobalFlow));
	TCHAR szBuffer[MAX_BUF];
	for (j=0;j<ProcessNumber;j++){
		SYSTEM_PROCESSES &sp=*pProcessInfo[j]; // pi Ϊ��ǰ�½�����Ϣ

		//�ӽ����������ҵ� pid ����Ľ���
		for (i=0;i<proN;i++){
			if (pro[i]->PID == (DWORD)(DWORD_PTR)sp.ProcessId)
				break;
		}

		//������½���
		if (i==proN) {
			pro[proN]=ProcessContainer.NewData();
			PROCESS &npro=*pro[proN];
			npro.PID=(DWORD)(DWORD_PTR)sp.ProcessId;//��ȡ���� pid
			npro.pPID=(DWORD)(DWORD_PTR)sp.InheritedFromProcessId;//��ȡ������ pid
			npro.SessionId=sp.SessionId; //��ȡ���̵ĻỰ ID

			//��ȡ��������
			if (npro.PID == 0){
				npro.name=ProcessStringContainer.NewData(Pid0Name,_tcslen(Pid0Name)+1);
			} else {
				npro.name=AddNtUnicodeToStringContainer(
					ProcessStringContainer,sp.ProcessName,szBuffer);
			}

			//��ȡ���� ID ����ʾ�ַ���
			_itow_s<32767>(npro.PID,szBuffer,10);
			npro.szPID=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

			//��ȡ������ ID ����ʾ�ַ���
			_itow_s<32767>(npro.pPID,szBuffer,10);
			npro.szParentID=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

			//��ȡ���̻Ự ID ����ʾ�ַ���
			_itow_s<32767>(npro.SessionId,szBuffer,10);
			npro.szSessionId=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

			//���ҽ����Ƿ��������б���
			npro.isCurrentProcess=npro.PID==dwCurrentProcessId;
			npro.inWhite=InList(setting.WhiteList,setting.nWhiteList,npro.name);
			npro.inBlack=InList(setting.BlackList,setting.nBlackList,npro.name);
			npro.inSystem=InList(setting.SystemProcess,setting.nSystemProcess,npro.name);

			npro.Checked=FALSE;
			npro.Selected=FALSE;

			//�򿪽��̣��õ�α���
			if (!npro.Process.Open(npro.PID,PROCESS_QUERY_LIMITED_INFORMATION))
				if (!npro.Process.Open(npro.PID))
					npro.Process.Attach(GetProcessHandle(npro.PID));

			if (npro.Process){
				//��ȡ����λ��
				DWORD_PTR dwpWow64Information=-1;
				if (NT_SUCCESS(npro.Process.NtQueryWow64Information(&dwpWow64Information)))
					npro.isWow64Process=dwpWow64Information!=0;
				else
					npro.isWow64Process=FALSE;

				//��ȡ����·��
				if (!npro.Process.GetImagePath(szBuffer,MAX_BUF))
					npro.path=EmptyStr;
				else {
					CheckFormatPath(szBuffer, lpszSystemRoot);
					npro.path=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);
				}

				//��ȡ����������
				if (!npro.Process.GetCmdLine(szBuffer,MAX_BUF))
					npro.cmdline=EmptyStr;
				else
					npro.cmdline=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

				//��ȡ�����û���
				if (!npro.Process.GetUserName(szBuffer,MAX_BUF)){
					DWORD dwLastError=GetLastError();
					npro.szUserName=EmptyStr;
				}
				else
					npro.szUserName=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

				//��ȡ�����ļ�ͼ��
				npro.hIcon=GetFileIcon(npro.path);
				if (npro.hIcon){
					npro.nImage=m_ImageList.Add(npro.hIcon);
					VERIFY(npro.nImage>=0);
				} else {
					npro.nImage=0;
				}

				//��ȡ�����ļ�����
				int cchBuf=MAX_BUF;
				if (!GetPeFileVersionInfoItem(npro.path,L"FileDescription",szBuffer,&cchBuf))
					npro.FileDescription=EmptyStr;
				else
					npro.FileDescription=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);
			} else {
				npro.isWow64Process=FALSE;
				npro.path=EmptyStr;
				npro.cmdline=EmptyStr;
				npro.szUserName=EmptyStr;
				npro.FileDescription=EmptyStr;
			}

			if (npro.isWow64Process){
				_stprintf_s<32767>(szBuffer,_T("%s *32"),npro.name);
				npro.szDispName=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);
				npro.szPlatform=_T("32λ");
			} else {
				npro.szDispName=npro.name;
				switch (dwSystemBits){
				case 64:
					npro.szPlatform=_T("64λ");
				case 32:
					npro.szPlatform=_T("32λ");
				}
			}

			//��ʼ��CPU��IO��д����
			npro.CPUTime=0l;
			npro.ReadTransferCount = 0l; //I/O��ȡ�ֽ�
			npro.WriteTransferCount = 0l; //I/Oд���ֽ�
			npro.OtherTransferCount = 0l; //I/O�����ֽ�
			proN++;
		}

		//��ʼ��ȡ������ϸ��Ϣ
		PROCESS &npro=*pro[i];

		npro.inWhite|=npro.isCurrentProcess;

		npro.bDisabledNetwork=FALSE;

		//ר�ù��������ֽڣ�
		if (stOsVerInfo.dwMajorVersion<6){ //�����С�� NT6 ϵ��ϵͳ
			if (npro.Process){
				if (!npro.Process.GetPrivateWorkingSet(&npro.PrivateWorkingSet))
					npro.PrivateWorkingSet=0;
			} else {
				npro.PrivateWorkingSet=0;
			}
		} else {
			npro.PrivateWorkingSet=(SIZE_T)sp.PrivateWorkingSetSize; //ר�ù��������ֽڣ�
		}

		npro.PrivateUsage = sp.PrivateUsage; //�����ڴ�(���������)���ֽڣ�
		npro.WorkingSetSize = sp.WorkingSetSize; //�ڴ棨�ֽڣ�
		npro.PeakWorkingSetSize = sp.PeakWorkingSetSize; //�ڴ��ֵ���ֽڣ�
#ifdef _WIN64
		npro.PeakVirtualSize = sp.PeakVirtualSize & 0x00000000FFFFFFFF; //�����ڴ��ֵ���ֽڣ�
		ASSERT(npro.PeakVirtualSize<4294967296);
#else
		if ((npro.PeakVirtualSize=sp.PeakWorkingSetSize)==0xFFFFFFFF)
			npro.PeakVirtualSize=0;
#endif
#ifdef _WIN64
		npro.VirtualSize = sp.VirtualSize & 0x00000000FFFFFFFF; //�����ڴ棨�ֽڣ�
#else
		if ((npro.VirtualSize=sp.VirtualSize)==0xFFFFFFFF)
			npro.VirtualSize=0;
#endif
		npro.QuotaPagedPoolUsage = sp.QuotaPagedPoolUsage; //ҳ�滺��أ��ֽڣ�
		npro.QuotaPeakPagedPoolUsage = sp.QuotaPeakPagedPoolUsage; //ҳ�滺��ط�ֵ���ֽڣ�
		npro.QuotaNonPagedPoolUsage = sp.QuotaNonPagedPoolUsage; //��ҳ�滺��أ��ֽڣ�
		npro.QuotaPeakNonPagedPoolUsage = sp.QuotaPeakNonPagedPoolUsage; //��ҳ�滺��ط�ֵ���ֽڣ�
		npro.PageFileUsage = sp.PageFileUsage; //ҳ���ļ�
		npro.PeakPageFileUsage = sp.PeakPageFileUsage; //ҳ���ļ���ֵ
		npro.PageFaultCount = sp.PageFaultCount; //ҳ����󣨸�����
		npro.ReadOperationCount = sp.ReadOperationCount.QuadPart; //I/O��ȡ
		npro.WriteOperationCount = sp.WriteOperationCount.QuadPart; //I/Oд��
		npro.OtherOperationCount = sp.OtherOperationCount.QuadPart; //I/O����
		npro.NumberOfThreads = sp.NumberOfThreads; //�߳�����
		npro.CreateTime = TimeOfDayInfo.CurrentTime.QuadPart-sp.CreateTime.QuadPart; //����������ʱ��
		npro.HandleCount = sp.HandleCount; //�������
		npro.Priority = KernelPriorityClass[sp.BasePriority]; //�������ȼ�

		npro.isSuspended = sp.NumberOfThreads>0 && sp.Threads[0].ThreadState == 5 && sp.Threads[0].WaitReason == Suspended;

		npro.CPU=0.0f;//��ʼ��CPUʹ����
		npro.IOSpeed=0l;//��ʼ��IO��д����
		npro.IOReadSpeed=0l;
		npro.IOWriteSpeed=0l;

		if (lTimeSpan){
			if (npro.CPUTime!=0l){
				//����CPUʹ����
				npro.CPU =	(FLOAT)(sp.KernelTime.QuadPart+sp.UserTime.QuadPart-npro.CPUTime)*100.0f
							/SystemBasicInfo.NumberOfProcessors
							/(FLOAT)llTimeSpan;
				if (npro.CPU<0.0f)
					npro.CPU=0.0f;
				if (npro.CPU>100.0f)
					npro.CPU=100.0f;
			}
			//����IO��ȡ����
			if (npro.ReadTransferCount!=0l){
				npro.IOReadSpeed =
					(sp.ReadTransferCount.QuadPart-npro.ReadTransferCount)*1000
					/lTimeSpan;
				if (npro.IOReadSpeed<0l)
					npro.IOReadSpeed=0l;
			}
			//����IOд������
			if (npro.WriteTransferCount!=0l){
				npro.IOWriteSpeed =
					(sp.WriteTransferCount.QuadPart-npro.WriteTransferCount)*1000
					/lTimeSpan;
				if (npro.IOWriteSpeed<0l)
					npro.IOWriteSpeed=0l;
			}
			//����IO������
			if (npro.ReadTransferCount!=0l || npro.WriteTransferCount!=0l || npro.OtherTransferCount!=0l){
				npro.IOSpeed =
					(
						(sp.ReadTransferCount.QuadPart-npro.ReadTransferCount)+
						(sp.WriteTransferCount.QuadPart-npro.WriteTransferCount)+
						(sp.OtherTransferCount.QuadPart-npro.OtherTransferCount)
					)*1000
					/lTimeSpan;
				if (npro.IOSpeed<0l)
					npro.IOSpeed=0l;
			}

			if (npro.PID==1608)
				npro.PID=npro.PID;
		}

		npro.CPUTime = sp.KernelTime.QuadPart+sp.UserTime.QuadPart; //CPUʱ��
		npro.ReadTransferCount = sp.ReadTransferCount.QuadPart; //I/O��ȡ�ֽ�
		npro.WriteTransferCount = sp.WriteTransferCount.QuadPart; //I/Oд���ֽ�
		npro.OtherTransferCount = sp.OtherTransferCount.QuadPart; //I/O�����ֽ�

		for (i=0;i<stProcessFlow.nProcess;i++){
			if (stProcessFlow.flow[i].dwPid==npro.PID)
				break;
		}
		if (i==stProcessFlow.nProcess){
			npro.NetSendSpeed=-2l;
			npro.NetRecvSpeed=-2l;
			npro.NetTotalSpeed=-2l;

			npro.NetSendFlow=-2l;
			npro.NetRecvFlow=-2l;
			npro.NetTotalFlow=-2l;

			npro.bDisabledNetwork=FALSE;
		} else {
			npro.NetSendSpeed=stProcessFlow.flow[i].SendSpeed;
			npro.NetRecvSpeed=stProcessFlow.flow[i].RecvSpeed;
			npro.NetTotalSpeed=npro.NetSendSpeed+npro.NetRecvSpeed;

			npro.NetSendFlow=stProcessFlow.flow[i].SendFlow;
			npro.NetRecvFlow=stProcessFlow.flow[i].RecvFlow;
			npro.NetTotalFlow=npro.NetSendFlow+npro.NetRecvFlow;

			npro.bDisabledNetwork=stProcessFlow.flow[i].nDisabled==LLONG_MAX;

			if (npro.bDisabledNetwork){
				npro.NetSendSpeed=-1l;
				npro.NetRecvSpeed=-1l;
				npro.NetTotalSpeed=-1l;
			} else {
				stGlobalFlow.SendSpeed+=npro.NetSendSpeed;
				stGlobalFlow.RecvSpeed+=npro.NetRecvSpeed;
				stGlobalFlow.SendFlow+=npro.NetSendFlow;
				stGlobalFlow.RecvFlow+=npro.NetRecvFlow;
			}
		}
	}
	qNetTotalSpeed=qNetSendSpeed+qNetRecvSpeed;
	pro[0]->inSystem=TRUE;
	return TRUE;
}

BOOL CSuperTaskmgrDlg::RefreshStatusBar(CStatusBar *statusbar){
	TCHAR szBuffer[1024];
	int i;

	_stprintf_s(szBuffer,1024,_T(" ������: %d"),ProcessNumber);
	m_ProcessNumber.SetWindowText(szBuffer);

	_stprintf_s(szBuffer,1024,_T(" CPU: %.1f%%"),fCPUUsage);
	SetWidth(szBuffer,_T(' '),16);
	i=240-(int)(fCPUUsage*240)/100;
	if (i<0) i=0;
	if (i>240) i=240;
	crCpuTotal=RGB(240,i,i);
	m_CPU.SetWindowText(szBuffer);

	_tcscpy_s(szBuffer,1024,_T("����: "));
	ConvSpeedStr(szBuffer+_tcslen(szBuffer),&qNetRecvSpeed,NULL);
	SetWidth(szBuffer,_T(' '),16);
	i=(int)((8.0f-log10((float)qNetRecvSpeed+1))*60.0f);
	if (i<0) i=0;
	if (i>240) i=240;
	crRecvTotal=RGB(240,i,240);
	m_RecvSpeed.SetWindowText(szBuffer);

	_tcscpy_s(szBuffer,1024,_T("�ϴ�: "));
	ConvSpeedStr(szBuffer+_tcslen(szBuffer),&qNetSendSpeed,NULL);
	SetWidth(szBuffer,_T(' '),16);
	i=(int)((8.0f-log10((float)qNetSendSpeed+1))*60.0f);
	if (i<0) i=0;
	if (i>240) i=240;
	crSendTotal=RGB(240,i,240);
	m_SendSpeed.SetWindowText(szBuffer);

	_tcscpy_s(szBuffer,1024,_T(" I/O: "));
	ConvSpeedStr(szBuffer+_tcslen(szBuffer),&qIoSpeed,NULL);
	SetWidth(szBuffer,_T(' '),16);
	i=(int)((8.0f-log10((float)qIoSpeed+1))*60.0f);
	if (i<0) i=0;
	if (i>240) i=240;
	crIoTotal=RGB(i,i,240);
	m_IO.SetWindowText(szBuffer);

	_stprintf_s(szBuffer,1024,_T(" �ڴ�: %d%%"),stMem.dwMemoryLoad);
	SetWidth(szBuffer,_T(' '),16);
	i=240-(int)(stMem.dwMemoryLoad*240)/100;
	if (i<0) i=0;
	if (i>240) i=240;
	crMemTotal=RGB(240,240,i);
	m_Memory.SetWindowText(szBuffer);

	return TRUE;
}

BOOL WINAPI CSuperTaskmgrDlg::GetCellString(int iItem,int iSubItem,LPCTSTR *str,LPTSTR szBuffer){
	const BOOL &bUpdateWhenRefresh=ColumnItemName[pcThis->setting.column[iSubItem].cindex].bUpdateWhenRefresh;
	if (bUpdateWhenRefresh){
		*str=(*ColumnItemName[pcThis->setting.column[iSubItem].cindex].lpfunConv)(
				szBuffer,
				(LPVOID)((INT_PTR)pcThis->pListPro[iItem]+ColumnItemName[pcThis->setting.column[iSubItem].cindex].lpvDataOffset),
				pcThis->pListPro[iItem]
			);
	} else {
		*str=*(LPCTSTR*)((INT_PTR)pcThis->pListPro[iItem]+ColumnItemName[pcThis->setting.column[iSubItem].cindex].lpvDispOffset);
	}
	return bUpdateWhenRefresh;
}

BOOL CSuperTaskmgrDlg::RefreshListInfo(){
	int i,j;

	//�б�����
	if (!SortList())
		return FALSE;
	//�����б�Ĵ�С��������������
	if (!
		m_ListInfo.SetListSize(setting.ColumnNumber,nListItemCount)
		)
		return FALSE;

	//�����б���ÿһ������ݣ��ı���
	if (!
		m_ListInfo.SetAllCellsString(GetCellString)
		)
		return FALSE;

	//�����б���ÿһ�����ʽ������ɫ��������ɫ��ͼ�꣩
	for (i=0;i<nListItemCount;i++){
		CListDispInfo::_DISP_ITEM &stDispItem=
			*m_ListInfo.GetList()->pdiaItem[i];

		stDispItem.bChecked=pListPro[i]->Checked;
		stDispItem.bSelected=pListPro[i]->Selected;
		stDispItem.nImage=pListPro[i]->nImage;
		for (j=0;j<setting.ColumnNumber;j++){
			CListDispInfo::DISP_CELL &stDispCell=
				stDispItem.dcaCell[j];
			GetItemColor(i,j,
				stDispCell.crText,
				stDispCell.crBack,
				stDispCell.hFont);
		}
	}

	return TRUE;
}

BOOL CSuperTaskmgrDlg::RefreshList(){
	//ˢ���б���Ϣ
	VERIFY(RefreshListInfo());

	int i,j;

	//�����б�ؼ��е�����
	VERIFY(m_LstProc.SetItemCount(nListItemCount));


	//�����б��е�ÿһ��Ԫ��
	for (i=0;i<nListItemCount;i++){
		CListDispInfo::_DISP_ITEM &stDispItem=
			*m_ListInfo.GetList()->pdiaItem[i];

		//���ø��е��ı�
		for (j=0;j<setting.ColumnNumber;j++){
			m_LstProc.SetItemText<32767>(i,j,stDispItem.dcaCell[j].szText);
		}

		LVITEM lvi;
		lvi.mask=LVIF_IMAGE | LVIF_STATE;
		lvi.stateMask=LVIS_STATEIMAGEMASK;
		lvi.iItem=i;
		lvi.iSubItem=0;

		//���ø��е�ͼ��
		lvi.iImage=(int)stDispItem.nImage;

		//���ø��еĸ�ѡ��
		lvi.state = INDEXTOSTATEIMAGEMASK((stDispItem.bChecked ? 2 : 1));

		//Ӧ�ø��е�����
		VERIFY(m_LstProc.SetItem(&lvi));

		//���ø��е�ѡ��״̬
		m_LstProc.SetItemState(i,(stDispItem.bSelected ? LVIS_SELECTED : 0),LVIS_SELECTED);
	}

	m_LstProc.Invalidate();

	return TRUE;
}


BOOL CSuperTaskmgrDlg::RefreshFloatingWindow(){

	//��������
	ConvLLNetSpeedStr(szUpSpeed,qNetSendSpeed);
	ConvLLNetSpeedStr(szDownSpeed,qNetRecvSpeed);

	//��ʼ����������
	m_FloatingWindow.Redraw(RGB(96,191,255),szUpSpeed,szDownSpeed);
	return TRUE;
}

//ˢ��
void CSuperTaskmgrDlg::Refresh(BOOL bForceFreshAll){

	//�˺����������룬��ֹ���߳�����
	static BOOL bRefreshSucceed=TRUE;
	if (!bRefreshSucceed) return;

	bRefreshSucceed=FALSE;

	//��ʼˢ��
	if (bForceFreshAll || (!IsIconic() && IsWindowVisible())){
		VERIFY(LoadCFG(FALSE));
		VERIFY(RefreshProcessInfo());
		VERIFY(CalcGlobalPerformanceInfo());
		VERIFY(RefreshStatusBar(&m_wndStatusBar));
		VERIFY(RefreshList());
	} else {
		//���������С��ʱֻˢ��ȫ��������Ϣ
		VERIFY(RefreshGlobalInfo());
	}

	//ˢ��֪ͨ��ͼ��
	RefreshNotifyIcon(fCPUUsage,qNetTotalSpeed,qIoSpeed,stMem.dwMemoryLoad);

	//ˢ��������
	if (setting.ShowFloatingWindow){
		VERIFY(RefreshFloatingWindow());
	}

	bRefreshSucceed=TRUE;
}

LRESULT CSuperTaskmgrDlg::OnRefresh(WPARAM wParam,LPARAM lParam){
	Refresh((BOOL)wParam);
	return 0;
}

void CSuperTaskmgrDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	PostMessage(WM_REFRESH);
	//Refresh();
	//for (int i=0;i<1000;i++) Refresh();
}



