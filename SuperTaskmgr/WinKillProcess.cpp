
#include "stdafx.h"

extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);
extern BOOL (WINAPI *EndTask)(HWND,BOOL,BOOL);

//����ҵ�����ķ�ʽ��������
BOOL AsJobObjKillProcess(DWORD pID){
	BOOL ret=FALSE;
	DWORD dwLastError;

	//�򿪽���
	HANDLE hProcess;
	if (!(
		hProcess=OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE,0,pID)
		))
	{
		return FALSE;
	}

	//������ҵ����
	HANDLE hJob;
	if (!(
		hJob=CreateJobObject(NULL,_T("AsJobObjKillProcess"))
		))
	{
		dwLastError=GetLastError();
		goto err1;
	}

	//�������̵���ҵ����
	if (!AssignProcessToJobObject(hJob,hProcess)){
		dwLastError=GetLastError();
		goto err2;
	}

	//������ҵ
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

//���������е������߳�
BOOL KillProcessAllThreads(DWORD pID){
	/*
	BOOL ret=FALSE;
	DWORD dwLastError;

	//��ȡ������Ϣ
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

	//Ѱ�Ҹý���
	int i;
	for (i=0;i<nProcessNumber;i++){
		if (pID == (DWORD)(DWORD_PTR)ProcessInfo[i]->ProcessId){
			break;
		}
	}

	//�������δ�ҵ�
	if (i==nProcessNumber){
		dwLastError = ERROR_INVALID_PARAMETER;
		goto err;
	}

	SYSTEM_PROCESSES &Process=*ProcessInfo[i];
	ULONG j;
	for (j=0;j<Process.NumberOfThreads;j++){

		//�򿪽��̵��߳�
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

		//�����߳�
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

//ͨ������Զ���߳�ִ�� ExitProcess(1) ����������
BOOL WriteProcessMemoryKill(DWORD pID,DWORD dwMilliseconds){
	BOOL ret=FALSE;
	DWORD dwLastError;

	//�򿪽���
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

	//��ȡ ExitProcess() ������ַ
	lpfnExitProcess myfnExitProcess;
	if (!(
		myfnExitProcess=(lpfnExitProcess)GetProcAddress(LoadLibrary(_T("kernel32")), "ExitProcess")
		))
	{
		dwLastError=GetLastError();
		goto end1;
	}

	//Զ�̷��亯���ռ�
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

	//Զ��д�뺯��
	if (!WriteProcessMemory(hProcess,lpRemoteFunc,&myfnExitProcess,sizeof(void*),NULL)){ //д�����
		dwLastError=GetLastError();
		goto end1;
	}
	if (!WriteProcessMemory(hProcess,(LPVOID)((INT_PTR)lpRemoteFunc+sizeof(void*)),
		RemoteFunc,SizeOfRemoteFunc,NULL)){ //д�뺯����
		dwLastError=GetLastError();
		goto end1;
	}

	//����Զ���߳�
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

	//�ȴ�Զ���̵߳�ִ��
	switch (WaitForSingleObject(hRemoteThread,dwMilliseconds)){
	case WAIT_ABANDONED:
		break;
	case WAIT_OBJECT_0:
		break;
	case WAIT_TIMEOUT: //�ȴ���ʱ
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

//ͨ���رմ˽��������еĴ�������������
BOOL KillProcessAllWindows(DWORD pID){
	BOOL ret=FALSE;
	DWORD dwLastError;

	if (!EnumWindows(KillProcessWindowsProc,(LPARAM)pID)){
		dwLastError=GetLastError();
		goto end;
	}

	//�򿪽���
	HANDLE hProcess;
	if (!(
		hProcess=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,pID)
		))
	{
		dwLastError=GetLastError();
		goto end;
	}

	//��ȡ���̽�����
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


