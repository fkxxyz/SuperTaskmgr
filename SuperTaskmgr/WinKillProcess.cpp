
#include "stdafx.h"

extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);
extern BOOL (WINAPI *EndTask)(HWND,BOOL,BOOL);

//以作业关联的方式结束进程
BOOL AsJobObjKillProcess(DWORD pID){
	BOOL ret=FALSE;
	DWORD dwLastError;

	//打开进程
	HANDLE hProcess;
	if (!(
		hProcess=OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE,0,pID)
		))
	{
		return FALSE;
	}

	//创建作业对象
	HANDLE hJob;
	if (!(
		hJob=CreateJobObject(NULL,_T("AsJobObjKillProcess"))
		))
	{
		dwLastError=GetLastError();
		goto err1;
	}

	//关联进程到作业对象
	if (!AssignProcessToJobObject(hJob,hProcess)){
		dwLastError=GetLastError();
		goto err2;
	}

	//结束作业
	if (!TerminateJobObject(hJob,1)){
		dwLastError=GetLastError();
		goto err2;
	}

	ret=TRUE;

err2:
	CloseHandle(hJob);
err1:
	CloseHandle(hProcess);

	if (!ret)
		SetLastError(dwLastError);
	return ret;
}

//结束进程中的所有线程
BOOL KillProcessAllThreads(DWORD pID){
	/*
	BOOL ret=FALSE;
	DWORD dwLastError;

	//获取进程信息
	SYSTEM_PROCESSES **pProcessInfo;
	int nProcessNumber;
	ULONG uRetLength;
	NTSTATUS ntstatus;
	if (!NT_SUCCESS(
		ntstatus=NtQueryProcessInfo(pProcessInfo,&nProcessNumber, &uRetLength)
		))
	{
		dwLastError = RtlNtStatusToDosError(ntstatus);
		goto err;
	}

	//寻找该进程
	int i;
	for (i=0;i<nProcessNumber;i++){
		if (pID == (DWORD)(DWORD_PTR)ProcessInfo[i]->ProcessId){
			break;
		}
	}

	//如果进程未找到
	if (i==nProcessNumber){
		dwLastError = ERROR_INVALID_PARAMETER;
		goto err;
	}

	SYSTEM_PROCESSES &Process=*ProcessInfo[i];
	ULONG j;
	for (j=0;j<Process.NumberOfThreads;j++){

		//打开进程的线程
		HANDLE hThread;
		if (!(
			hThread=OpenThread(THREAD_TERMINATE,FALSE,
				(DWORD)(DWORD_PTR)Process.Threads[j].ClientId.UniqueThread
				)
		))
		{
			dwLastError=GetLastError();
			continue;
		}

		//结束线程
		if (!TerminateThread(hThread,1)){
			dwLastError=GetLastError();
		}

		CloseHandle(hThread);
	}

	ret=dwLastError == ERROR_SUCCESS;

err:
	if (!ret)
		SetLastError(dwLastError);
	return ret;
	*/
	return 0;
}


#define PROCESS_CREATE_REMOTE_THREAD  (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)
typedef void (WINAPI *lpfnExitProcess)(UINT);
#define SizeOfRemoteFunc 64

DWORD RemoteFunc(lpfnExitProcess *lpExitProcess)
{
	(*lpExitProcess)(1);
	return 0;
}

//通过创建远程线程执行 ExitProcess(1) 来结束进程
BOOL WriteProcessMemoryKill(DWORD pID,DWORD dwMilliseconds){
	BOOL ret=FALSE;
	DWORD dwLastError;

	//打开进程
	HANDLE hProcess;
	if (!(
		hProcess=OpenProcess(PROCESS_CREATE_REMOTE_THREAD,FALSE,pID)
		) && !(
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pID)
		))
	{
		dwLastError=GetLastError();
		goto end;
	}

	//获取 ExitProcess() 函数地址
	lpfnExitProcess myfnExitProcess;
	if (!(
		myfnExitProcess=(lpfnExitProcess)GetProcAddress(LoadLibrary(_T("kernel32")), "ExitProcess")
		))
	{
		dwLastError=GetLastError();
		goto end1;
	}

	//远程分配函数空间
	LPVOID lpRemoteFunc;
	if (!(
		lpRemoteFunc=VirtualAllocEx(
		hProcess,
		NULL,
		SizeOfRemoteFunc+sizeof(void*),
		MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE
		)
		))
	{
		dwLastError=GetLastError();
		goto end1;
	}

	//远程写入函数
	if (!WriteProcessMemory(hProcess,lpRemoteFunc,&myfnExitProcess,sizeof(void*),NULL)){ //写入参数
		dwLastError=GetLastError();
		goto end1;
	}
	if (!WriteProcessMemory(hProcess,(LPVOID)((INT_PTR)lpRemoteFunc+sizeof(void*)),
		RemoteFunc,SizeOfRemoteFunc,NULL)){ //写入函数体
		dwLastError=GetLastError();
		goto end1;
	}

	//创建远程线程
	HANDLE hRemoteThread;
	if (!(
		hRemoteThread=CreateRemoteThread(
		hProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)((INT_PTR)lpRemoteFunc+sizeof(void*)),
		lpRemoteFunc,
		0,
		0
		)
		))
	{
		dwLastError=GetLastError();
		goto end2;
	}

	//等待远程线程的执行
	switch (WaitForSingleObject(hRemoteThread,dwMilliseconds)){
	case WAIT_ABANDONED:
		break;
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT: //等待超时
		dwLastError=ERROR_TIMEOUT;
		ASSERT(FALSE);
		goto end2;
	case WAIT_FAILED:
		dwLastError=GetLastError();
		goto end2;
	}

	ret=TRUE;

end2:
	CloseHandle(hRemoteThread);
end1:
	CloseHandle(hProcess);

end:
	if (!ret)
		SetLastError(dwLastError);
	return ret;
}


BOOL CALLBACK KillProcessWindowsProc(HWND hwnd,LPARAM lParam){
	DWORD pID;
	GetWindowThreadProcessId(hwnd,&pID);
	if (pID==(DWORD)lParam){
		EndTask(hwnd,FALSE,TRUE);
	}
	EnumChildWindows(hwnd,KillProcessWindowsProc,lParam);
	return TRUE;
}

//通过关闭此进程中所有的窗口来结束进程
BOOL KillProcessAllWindows(DWORD pID){
	BOOL ret=FALSE;
	DWORD dwLastError;

	if (!EnumWindows(KillProcessWindowsProc,(LPARAM)pID)){
		dwLastError=GetLastError();
		goto end;
	}

	//打开进程
	HANDLE hProcess;
	if (!(
		hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,pID)
		))
	{
		dwLastError=GetLastError();
		goto end;
	}

	//获取进程结束码
	DWORD dwExitCode;
	if (!GetExitCodeProcess(hProcess,&dwExitCode)){
		dwLastError=GetLastError();
		goto end1;
	}

	if (dwExitCode == STILL_ACTIVE){
		dwLastError=ERROR_ACCESS_DENIED;
		goto end1;
	}

	ret=TRUE;

end1:
	CloseHandle(hProcess);

end:
	if (!ret)
		SetLastError(dwLastError);
	return ret;
}


