
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrRefresh.h"
#include "SuperTaskmgrItemColor.h"
#include "WinKillProcess.h"
#include <math.h>
#include <locale.h>

//纯正整数转换（ULONG 到 LPTSTR）
LPTSTR CSuperTaskmgrDlg::ConvULONGStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	_stprintf_s(szBuf,16,_T("%lu"),*(ULONG*)pItem);
	return szBuf;
}

//时间转换（LONGLONG 到 LPTSTR "x:xx:xx"）
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

//CPU 使用率转换（FLOAT 到 LPTSTR "xx.x %"）
LPTSTR CSuperTaskmgrDlg::ConvCPUStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	_stprintf_s(szBuf,16,_T("%.1f %%"),*(FLOAT*)pItem);
	return szBuf;
}

//优先级转换（DWORD 到 LPTSTR "标准"）
LPTSTR CSuperTaskmgrDlg::ConvPriorityStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	switch (*(DWORD*)pItem){
	case IDLE_PRIORITY_CLASS:
		return _T("低");
	case BELOW_NORMAL_PRIORITY_CLASS:
		return _T("较低");
		break;
	case NORMAL_PRIORITY_CLASS:
		return _T("标准");
		break;
	case ABOVE_NORMAL_PRIORITY_CLASS:
		return _T("较高");
		break;
	case HIGH_PRIORITY_CLASS:
		return _T("高");
		break;
	case REALTIME_PRIORITY_CLASS:
		return _T("实时");
		break;
	default:
		return _T("未知");
	}
	return szBuf;
}

//进程是否挂起的转换（BOOL 到 LPTSTR "挂起"）
LPTSTR CSuperTaskmgrDlg::ConvSuspendStr(LPTSTR szBuf,LPVOID pItem,LPPROCESS ppro){
	return *(BOOL*)pItem ? _T("挂起") : _T("正常");
}

//内存转换（ULONG 到 LPTSTR "xxx,xxx,xxx.x MB"）
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

//内存转换（ULONG 到 LPTSTR "xxx,xxx,xxx KB"）
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

//速度转换（LONGLONG 到 LPTSTR "xxx.x MB/s"）
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
		_tcscpy_s(szBuf,32,_T("已禁止"));
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

//判断 str 是否在 list 列表中
BOOL CSuperTaskmgrDlg::InList(LPTSTR *list,INT_PTR listN,LPCTSTR str){
	INT_PTR i;
	for (i=0;i<listN;i++)
		if (_tcsicmp(str,list[i])==0)
			return TRUE;
	return FALSE;
}


int CSuperTaskmgrDlg::PriorityIndex(DWORD Priority){
	switch (Priority){
	case 0: //未知
		return 0;
	case IDLE_PRIORITY_CLASS: //低
		return 1;
	case BELOW_NORMAL_PRIORITY_CLASS: //较低
		return 2;
	case NORMAL_PRIORITY_CLASS: //标准
		return 3;
	case ABOVE_NORMAL_PRIORITY_CLASS: //较高
		return 4;
	case HIGH_PRIORITY_CLASS: //高
		return 5;
	case REALTIME_PRIORITY_CLASS: //实时
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

//qsort 快排函数的回调函数
int CSuperTaskmgrDlg::SortCmp(const void *p1,const void *p2){
	return ColumnItemName[SortColItem].lpfunCmp(
		(LPVOID)((INT_PTR)*(LPPROCESS*)p1+ColumnItemName[SortColItem].lpvDataOffset),
		(LPVOID)((INT_PTR)*(LPPROCESS*)p2+ColumnItemName[SortColItem].lpvDataOffset)
	)*RevSort;
}

//排序列表中的项目
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

	//计算距离上一次刷新时间间隔
	static LONGLONG LastTime; //上一次刷新全局性能的时间
	LONGLONG llTimeSpan;
	if (LastTime==0l)
		llTimeSpan=0l;
	else
		llTimeSpan=TimeOfDayInfo.CurrentTime.QuadPart-LastTime;
	LastTime=TimeOfDayInfo.CurrentTime.QuadPart;

	//计算全局CPU使用率、IO读写速度
	fCPUUsage=0.0f;
	qIoSpeed=0l;
	if (llTimeSpan){
		if (qCPUTime!=0l){
			//计算CPU使用率
			fCPUUsage =	100.0f-(FLOAT)(PerformanceInfo.IdleProcessTime.QuadPart-qCPUTime)*100.0f
						/(FLOAT)(SystemBasicInfo.NumberOfProcessors*llTimeSpan);
			if (fCPUUsage<0.0f)
				fCPUUsage=0.0f;
			if (fCPUUsage>100.0f)
				fCPUUsage=100.0f;
		}

		//计算 IO 读写速度
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


	//设置最大颜色时的值
	if (fCPUUsage<10.0f){
		fMaxColorCpu=fCPUUsage*8+1.0f;
	} else if (fCPUUsage<25.0f){
		fMaxColorCpu=fCPUUsage*4;
	} else if (fCPUUsage<50.0f){
		fMaxColorCpu=fCPUUsage*2;
	} else {
		fMaxColorCpu=100.0f;
	}
	if (qNetTotalSpeed<1024l){ //小于 1 KB/s
		fMaxColorNetSpeed=(float)(qNetTotalSpeed*32+1);
	} else if (qNetTotalSpeed<1024l*100l){ //小于 100 KB/s
		fMaxColorNetSpeed=(float)(qNetTotalSpeed*8);
	} else if (qNetTotalSpeed<1024l*1024l*100l){ //小于 1 MB/s
		fMaxColorNetSpeed=(float)(qNetTotalSpeed*2);
	} else {
		fMaxColorNetSpeed=(float)(qNetTotalSpeed);
	}
	if (qIoSpeed<1024l){ //小于 1 KB/s
		fMaxColorIo=(float)(qIoSpeed*32+1);
	} else if (qIoSpeed<1024l*1024l){ //小于 1 MB/s
		fMaxColorIo=(float)(qIoSpeed*8);
	} else if (qIoSpeed<1024l*1024l*100l){ //小于 100 MB/s
		fMaxColorIo=(float)(qIoSpeed*2);
	} else {
		fMaxColorIo=(float)(qIoSpeed);
	}
	return TRUE;
}

//刷新全局性能信息
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

//刷新进程信息
BOOL CSuperTaskmgrDlg::RefreshProcessInfo(){
	static LPCTSTR EmptyStr=_T("");

	//获取进程、时间、处理器等信息
	if (!NT_SUCCESS(NtQueryProcessInfo(pProcessInfo,&ProcessNumber)))
		return FALSE;
	if (!NT_SUCCESS(NtQueryTimeOfDayInfo(&TimeOfDayInfo)))
		return FALSE;
	if (!NT_SUCCESS(NtQueryPerformanceInfo(&PerformanceInfo)))
		return FALSE;
	VERIFY(GlobalMemoryStatusEx(&stMem));

	//进程网络流量信息
	VERIFY(TdiFilter.GetProcessFlow(&stProcessFlow));

	//计算距离上一次刷新时间间隔
	static LONGLONG LastTime=0l; //上一次分别刷新所有进程的时间
	ULONG lTimeSpan; //距离上一次刷新的时间间隔 (单位：0.1毫秒)
	LONGLONG llTimeSpan; //距离上一次刷新的时间间隔 (单位：10纳秒)
	if (LastTime==0l) {
		llTimeSpan=0l;
		lTimeSpan=0;
	} else
		lTimeSpan=(ULONG)((llTimeSpan=TimeOfDayInfo.CurrentTime.QuadPart-LastTime)/10000);
	LastTime=TimeOfDayInfo.CurrentTime.QuadPart;

	//从进程数组中删除已经不存在的进程
	int i,j;
	for (i=0;i<proN;i++){
		PROCESS &npro=*pro[i];
		for (j=0;j<ProcessNumber;j++){
			if (pro[i]->PID==(DWORD)(DWORD_PTR)pProcessInfo[j]->ProcessId)
				break;
		}
		if (j==ProcessNumber){//如果进程不存在
			proN--;

			if (npro.Process)
				npro.Process.Close();
			if (npro.hIcon)
				DestroyIcon(npro.hIcon);
			//将后面所有进程前移
			for (j=i;j<proN;j++)
				pro[j]=pro[j+1];
			i--;
		}
	}

	//开始刷新信息
	memset(&stGlobalFlow,0,sizeof(stGlobalFlow));
	TCHAR szBuffer[MAX_BUF];
	for (j=0;j<ProcessNumber;j++){
		SYSTEM_PROCESSES &sp=*pProcessInfo[j]; // pi 为当前新进程信息

		//从进程数组中找到 pid 相符的进程
		for (i=0;i<proN;i++){
			if (pro[i]->PID == (DWORD)(DWORD_PTR)sp.ProcessId)
				break;
		}

		//如果是新进程
		if (i==proN) {
			pro[proN]=ProcessContainer.NewData();
			PROCESS &npro=*pro[proN];
			npro.PID=(DWORD)(DWORD_PTR)sp.ProcessId;//获取进程 pid
			npro.pPID=(DWORD)(DWORD_PTR)sp.InheritedFromProcessId;//获取父进程 pid
			npro.SessionId=sp.SessionId; //获取进程的会话 ID

			//获取进程名称
			if (npro.PID == 0){
				npro.name=ProcessStringContainer.NewData(Pid0Name,_tcslen(Pid0Name)+1);
			} else {
				npro.name=AddNtUnicodeToStringContainer(
					ProcessStringContainer,sp.ProcessName,szBuffer);
			}

			//获取进程 ID 的显示字符串
			_itow_s<32767>(npro.PID,szBuffer,10);
			npro.szPID=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

			//获取父进程 ID 的显示字符串
			_itow_s<32767>(npro.pPID,szBuffer,10);
			npro.szParentID=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

			//获取进程会话 ID 的显示字符串
			_itow_s<32767>(npro.SessionId,szBuffer,10);
			npro.szSessionId=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

			//查找进程是否在三个列表中
			npro.isCurrentProcess=npro.PID==dwCurrentProcessId;
			npro.inWhite=InList(setting.WhiteList,setting.nWhiteList,npro.name);
			npro.inBlack=InList(setting.BlackList,setting.nBlackList,npro.name);
			npro.inSystem=InList(setting.SystemProcess,setting.nSystemProcess,npro.name);

			npro.Checked=FALSE;
			npro.Selected=FALSE;

			//打开进程，得到伪句柄
			if (!npro.Process.Open(npro.PID,PROCESS_QUERY_LIMITED_INFORMATION))
				if (!npro.Process.Open(npro.PID))
					npro.Process.Attach(GetProcessHandle(npro.PID));

			if (npro.Process){
				//获取进程位数
				DWORD_PTR dwpWow64Information=-1;
				if (NT_SUCCESS(npro.Process.NtQueryWow64Information(&dwpWow64Information)))
					npro.isWow64Process=dwpWow64Information!=0;
				else
					npro.isWow64Process=FALSE;

				//获取进程路径
				if (!npro.Process.GetImagePath(szBuffer,MAX_BUF))
					npro.path=EmptyStr;
				else {
					CheckFormatPath(szBuffer, lpszSystemRoot);
					npro.path=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);
				}

				//获取进程命令行
				if (!npro.Process.GetCmdLine(szBuffer,MAX_BUF))
					npro.cmdline=EmptyStr;
				else
					npro.cmdline=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

				//获取进程用户名
				if (!npro.Process.GetUserName(szBuffer,MAX_BUF)){
					DWORD dwLastError=GetLastError();
					npro.szUserName=EmptyStr;
				}
				else
					npro.szUserName=ProcessStringContainer.NewData(szBuffer,_tcslen(szBuffer)+1);

				//获取进程文件图标
				npro.hIcon=GetFileIcon(npro.path);
				if (npro.hIcon){
					npro.nImage=m_ImageList.Add(npro.hIcon);
					VERIFY(npro.nImage>=0);
				} else {
					npro.nImage=0;
				}

				//获取进程文件描述
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
				npro.szPlatform=_T("32位");
			} else {
				npro.szDispName=npro.name;
				switch (dwSystemBits){
				case 64:
					npro.szPlatform=_T("64位");
				case 32:
					npro.szPlatform=_T("32位");
				}
			}

			//初始化CPU、IO读写速率
			npro.CPUTime=0l;
			npro.ReadTransferCount = 0l; //I/O读取字节
			npro.WriteTransferCount = 0l; //I/O写入字节
			npro.OtherTransferCount = 0l; //I/O其他字节
			proN++;
		}

		//开始获取进程详细信息
		PROCESS &npro=*pro[i];

		npro.inWhite|=npro.isCurrentProcess;

		npro.bDisabledNetwork=FALSE;

		//专用工作集（字节）
		if (stOsVerInfo.dwMajorVersion<6){ //如果是小于 NT6 系列系统
			if (npro.Process){
				if (!npro.Process.GetPrivateWorkingSet(&npro.PrivateWorkingSet))
					npro.PrivateWorkingSet=0;
			} else {
				npro.PrivateWorkingSet=0;
			}
		} else {
			npro.PrivateWorkingSet=(SIZE_T)sp.PrivateWorkingSetSize; //专用工作集（字节）
		}

		npro.PrivateUsage = sp.PrivateUsage; //虚拟内存(任务管理器)（字节）
		npro.WorkingSetSize = sp.WorkingSetSize; //内存（字节）
		npro.PeakWorkingSetSize = sp.PeakWorkingSetSize; //内存峰值（字节）
#ifdef _WIN64
		npro.PeakVirtualSize = sp.PeakVirtualSize & 0x00000000FFFFFFFF; //虚拟内存峰值（字节）
		ASSERT(npro.PeakVirtualSize<4294967296);
#else
		if ((npro.PeakVirtualSize=sp.PeakWorkingSetSize)==0xFFFFFFFF)
			npro.PeakVirtualSize=0;
#endif
#ifdef _WIN64
		npro.VirtualSize = sp.VirtualSize & 0x00000000FFFFFFFF; //虚拟内存（字节）
#else
		if ((npro.VirtualSize=sp.VirtualSize)==0xFFFFFFFF)
			npro.VirtualSize=0;
#endif
		npro.QuotaPagedPoolUsage = sp.QuotaPagedPoolUsage; //页面缓冲池（字节）
		npro.QuotaPeakPagedPoolUsage = sp.QuotaPeakPagedPoolUsage; //页面缓冲池峰值（字节）
		npro.QuotaNonPagedPoolUsage = sp.QuotaNonPagedPoolUsage; //非页面缓冲池（字节）
		npro.QuotaPeakNonPagedPoolUsage = sp.QuotaPeakNonPagedPoolUsage; //非页面缓冲池峰值（字节）
		npro.PageFileUsage = sp.PageFileUsage; //页面文件
		npro.PeakPageFileUsage = sp.PeakPageFileUsage; //页面文件峰值
		npro.PageFaultCount = sp.PageFaultCount; //页面错误（个数）
		npro.ReadOperationCount = sp.ReadOperationCount.QuadPart; //I/O读取
		npro.WriteOperationCount = sp.WriteOperationCount.QuadPart; //I/O写入
		npro.OtherOperationCount = sp.OtherOperationCount.QuadPart; //I/O其他
		npro.NumberOfThreads = sp.NumberOfThreads; //线程数量
		npro.CreateTime = TimeOfDayInfo.CurrentTime.QuadPart-sp.CreateTime.QuadPart; //进程已运行时间
		npro.HandleCount = sp.HandleCount; //句柄数量
		npro.Priority = KernelPriorityClass[sp.BasePriority]; //进程优先级

		npro.isSuspended = sp.NumberOfThreads>0 && sp.Threads[0].ThreadState == 5 && sp.Threads[0].WaitReason == Suspended;

		npro.CPU=0.0f;//初始化CPU使用率
		npro.IOSpeed=0l;//初始化IO读写速率
		npro.IOReadSpeed=0l;
		npro.IOWriteSpeed=0l;

		if (lTimeSpan){
			if (npro.CPUTime!=0l){
				//计算CPU使用率
				npro.CPU =	(FLOAT)(sp.KernelTime.QuadPart+sp.UserTime.QuadPart-npro.CPUTime)*100.0f
							/SystemBasicInfo.NumberOfProcessors
							/(FLOAT)llTimeSpan;
				if (npro.CPU<0.0f)
					npro.CPU=0.0f;
				if (npro.CPU>100.0f)
					npro.CPU=100.0f;
			}
			//计算IO读取速率
			if (npro.ReadTransferCount!=0l){
				npro.IOReadSpeed =
					(sp.ReadTransferCount.QuadPart-npro.ReadTransferCount)*1000
					/lTimeSpan;
				if (npro.IOReadSpeed<0l)
					npro.IOReadSpeed=0l;
			}
			//计算IO写入速率
			if (npro.WriteTransferCount!=0l){
				npro.IOWriteSpeed =
					(sp.WriteTransferCount.QuadPart-npro.WriteTransferCount)*1000
					/lTimeSpan;
				if (npro.IOWriteSpeed<0l)
					npro.IOWriteSpeed=0l;
			}
			//计算IO总速率
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

		npro.CPUTime = sp.KernelTime.QuadPart+sp.UserTime.QuadPart; //CPU时间
		npro.ReadTransferCount = sp.ReadTransferCount.QuadPart; //I/O读取字节
		npro.WriteTransferCount = sp.WriteTransferCount.QuadPart; //I/O写入字节
		npro.OtherTransferCount = sp.OtherTransferCount.QuadPart; //I/O其他字节

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

	_stprintf_s(szBuffer,1024,_T(" 进程数: %d"),ProcessNumber);
	m_ProcessNumber.SetWindowText(szBuffer);

	_stprintf_s(szBuffer,1024,_T(" CPU: %.1f%%"),fCPUUsage);
	SetWidth(szBuffer,_T(' '),16);
	i=240-(int)(fCPUUsage*240)/100;
	if (i<0) i=0;
	if (i>240) i=240;
	crCpuTotal=RGB(240,i,i);
	m_CPU.SetWindowText(szBuffer);

	_tcscpy_s(szBuffer,1024,_T("下载: "));
	ConvSpeedStr(szBuffer+_tcslen(szBuffer),&qNetRecvSpeed,NULL);
	SetWidth(szBuffer,_T(' '),16);
	i=(int)((8.0f-log10((float)qNetRecvSpeed+1))*60.0f);
	if (i<0) i=0;
	if (i>240) i=240;
	crRecvTotal=RGB(240,i,240);
	m_RecvSpeed.SetWindowText(szBuffer);

	_tcscpy_s(szBuffer,1024,_T("上传: "));
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

	_stprintf_s(szBuffer,1024,_T(" 内存: %d%%"),stMem.dwMemoryLoad);
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

	//列表排序
	if (!SortList())
		return FALSE;
	//设置列表的大小（行数、列数）
	if (!
		m_ListInfo.SetListSize(setting.ColumnNumber,nListItemCount)
		)
		return FALSE;

	//设置列表中每一项的数据（文本）
	if (!
		m_ListInfo.SetAllCellsString(GetCellString)
		)
		return FALSE;

	//设置列表中每一项的样式（背景色、文字颜色、图标）
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
	//刷新列表信息
	VERIFY(RefreshListInfo());

	int i,j;

	//设置列表控件中的项数
	VERIFY(m_LstProc.SetItemCount(nListItemCount));


	//设置列表中的每一单元格
	for (i=0;i<nListItemCount;i++){
		CListDispInfo::_DISP_ITEM &stDispItem=
			*m_ListInfo.GetList()->pdiaItem[i];

		//设置该行的文本
		for (j=0;j<setting.ColumnNumber;j++){
			m_LstProc.SetItemText<32767>(i,j,stDispItem.dcaCell[j].szText);
		}

		LVITEM lvi;
		lvi.mask=LVIF_IMAGE | LVIF_STATE;
		lvi.stateMask=LVIS_STATEIMAGEMASK;
		lvi.iItem=i;
		lvi.iSubItem=0;

		//设置该行的图标
		lvi.iImage=(int)stDispItem.nImage;

		//设置该行的复选框
		lvi.state = INDEXTOSTATEIMAGEMASK((stDispItem.bChecked ? 2 : 1));

		//应用该行的设置
		VERIFY(m_LstProc.SetItem(&lvi));

		//设置该行的选中状态
		m_LstProc.SetItemState(i,(stDispItem.bSelected ? LVIS_SELECTED : 0),LVIS_SELECTED);
	}

	m_LstProc.Invalidate();

	return TRUE;
}


BOOL CSuperTaskmgrDlg::RefreshFloatingWindow(){

	//设置文字
	ConvLLNetSpeedStr(szUpSpeed,qNetSendSpeed);
	ConvLLNetSpeedStr(szDownSpeed,qNetRecvSpeed);

	//开始绘制悬浮窗
	m_FloatingWindow.Redraw(RGB(96,191,255),szUpSpeed,szDownSpeed);
	return TRUE;
}

//刷新
void CSuperTaskmgrDlg::Refresh(BOOL bForceFreshAll){

	//此函数不可重入，防止多线程重入
	static BOOL bRefreshSucceed=TRUE;
	if (!bRefreshSucceed) return;

	bRefreshSucceed=FALSE;

	//开始刷新
	if (bForceFreshAll || (!IsIconic() && IsWindowVisible())){
		VERIFY(LoadCFG(FALSE));
		VERIFY(RefreshProcessInfo());
		VERIFY(CalcGlobalPerformanceInfo());
		VERIFY(RefreshStatusBar(&m_wndStatusBar));
		VERIFY(RefreshList());
	} else {
		//如果窗口最小化时只刷新全局性能信息
		VERIFY(RefreshGlobalInfo());
	}

	//刷新通知栏图标
	RefreshNotifyIcon(fCPUUsage,qNetTotalSpeed,qIoSpeed,stMem.dwMemoryLoad);

	//刷新悬浮窗
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



