
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrProcess.h"
#include "WinKillProcess.h"

LPCTSTR Pid0Name=_T("系统空闲");

extern NTSTATUS (NTAPI *NtTerminateProcess)(HANDLE,UINT);

//优先级常量
const DWORD CSuperTaskmgrDlg::PriorityClass[7]={
	0,
	IDLE_PRIORITY_CLASS,
	BELOW_NORMAL_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS,
	ABOVE_NORMAL_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS
};
const DWORD CSuperTaskmgrDlg::KernelPriorityClass[32]={
	0,
	IDLE_PRIORITY_CLASS,
	IDLE_PRIORITY_CLASS,
	IDLE_PRIORITY_CLASS,
	IDLE_PRIORITY_CLASS,
	IDLE_PRIORITY_CLASS, // <6
	BELOW_NORMAL_PRIORITY_CLASS, // ==6
	NORMAL_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS,
	NORMAL_PRIORITY_CLASS, // <10
	ABOVE_NORMAL_PRIORITY_CLASS, // ==10
	HIGH_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS,
	HIGH_PRIORITY_CLASS, // <16
	REALTIME_PRIORITY_CLASS, //==16
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS,
	REALTIME_PRIORITY_CLASS
};


//获取 csrss.exe 的进程ID
DWORD CSuperTaskmgrDlg::GetCsrssPID(){
	int i,n;
	if (!NT_SUCCESS(NtQueryProcessInfo(pProcessInfo,&ProcessNumber))){
		ASSERT(0);
		return 0;
	}
	n=ProcessNumber;
	for (i=0;i<n;i++){
		SYSTEM_PROCESSES &Process=*pProcessInfo[i];
		if (Process.ProcessName.Length<9) continue;
		if (_wcsnicmp(Process.ProcessName.Buffer,L"csrss.exe",9)==0)
			return (DWORD)(DWORD_PTR)Process.ProcessId;
	}
	return 0;
}

//获取进程句柄（绕过 NtOpenProcess 函数）
HANDLE CSuperTaskmgrDlg::GetProcessHandle(DWORD ProcessID){
	HANDLE ret;

	if (!NT_SUCCESS(NtQuerySystemHandleInfo(pSystemHandleInfo))){
		ASSERT(0);
		return NULL;
	}

	INT_PTR n=pSystemHandleInfo.GetData()->NumberOfHandle;
	for (INT_PTR i=0;i<n;i++){//枚举所有的句柄
		SYSTEM_HANDLE_INFORMATION &HandleInfo=pSystemHandleInfo.GetData()->stHandle[i];
		if (HandleInfo.ObjectTypeNumber==5){ //如果句柄对应的内核对象类型是进程
			CProcess Process;
			if (!Process.Open(HandleInfo.ProcessID)) continue;
			if (DuplicateHandle(Process,(PHANDLE)HandleInfo.Handle,GetCurrentProcess(),&ret,0,TRUE,DUPLICATE_SAME_ACCESS)){
				CProcess DestProcess(ret);
				if (DestProcess.GetId()==ProcessID){ //如果句柄对应的进程句柄类型是进程句柄
					DestProcess.Detach();
					return ret;
				}
			}
		}
	}
	return NULL;
}

//结束进程
BOOL CSuperTaskmgrDlg::KillProcess(DWORD PID){
	DWORD dwLastError;

	CProcess Process;
	if (!Process.Open(PID,PROCESS_TERMINATE)){
		dwLastError=GetLastError();
		Process.Attach(GetProcessHandle(PID));
	}
	if (Process){
		NTSTATUS ntstatus=NtTerminateProcess(Process,1);
		if (NT_SUCCESS(ntstatus))
			return TRUE;
		else
			dwLastError=RtlNtStatusToDosError(ntstatus);
	}

	if (
		AsJobObjKillProcess(PID)
			||
		KillProcessAllThreads(PID)
			||
		WriteProcessMemoryKill(PID)
			||
		KillProcessAllWindows(PID)
		)
		return TRUE;

	SetLastError(dwLastError);
	return FALSE;
}



