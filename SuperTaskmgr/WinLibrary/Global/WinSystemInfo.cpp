
// WinSystemInfo.cpp

#include "stdafx.h"
#include "WinSystemInfo.h"

//��ȡϵͳ�汾��Ϣ
BOOL WINAPI RealGetVersionEx(LPOSVERSIONINFO lpVersionInfo){
	if (!GetVersionEx(lpVersionInfo)) return FALSE;
	RtlGetNtVersionNumbers(&lpVersionInfo->dwMajorVersion,&lpVersionInfo->dwMinorVersion,&lpVersionInfo->dwBuildNumber);
	lpVersionInfo->dwBuildNumber&=0x0000FFFF;
	return TRUE;
}


#define SP_OFFERT (MaxProcessNumber*sizeof(SYSTEM_PROCESSES*))
NTSTATUS NtQueryProcessInfo(CDMAlloc<SYSTEM_PROCESSES**> &pProcess,int *pProcessNumber){

	//���Ե�һ�λ�ȡ
	SYSTEM_PROCESSES *pSystemProcess=(SYSTEM_PROCESSES *)(pProcess+MaxProcessNumber);
	ULONG nBytesReturn;
	NTSTATUS result;
	if ((ULONG)pProcess.GetSize()>SP_OFFERT)
		result=NtQueryProcessInfo(pSystemProcess,
			(ULONG)pProcess.GetSize()-SP_OFFERT,&nBytesReturn);
	else
		result=NtQueryProcessInfo(NULL,0,&nBytesReturn);
	if (NT_SUCCESS(result)) goto lBuildIndex;

	//�����һ�λ�ȡ���ɹ�����Ϊ���������Ȳ���ʱ������仺�������»�ȡ
	if (result!=STATUS_INFO_LENGTH_MISMATCH) return result;
	if (!pProcess.DemandSize(nBytesReturn+SP_OFFERT)) return STATUS_NO_MEMORY;
	pSystemProcess=(SYSTEM_PROCESSES *)(pProcess+MaxProcessNumber);
	result=NtQueryProcessInfo(pSystemProcess,
		(ULONG)pProcess.GetSize()-SP_OFFERT,&nBytesReturn);

	if (!NT_SUCCESS(result)) return result;

	//��������
lBuildIndex:
	register int nProcess=0;
	while (1){
		pProcess[nProcess++]=pSystemProcess;

		if (!pSystemProcess->NextEntryOffset)
			break;
		pSystemProcess=(SYSTEM_PROCESSES *)(
			(DWORD_PTR)pSystemProcess+pSystemProcess->NextEntryOffset
			);
	}
	*pProcessNumber=nProcess;
	return result;
}

