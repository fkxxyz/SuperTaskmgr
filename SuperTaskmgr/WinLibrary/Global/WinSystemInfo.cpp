
// WinSystemInfo.cpp

#include "stdafx.h"
#include "WinSystemInfo.h"

//获取系统版本信息
BOOL WINAPI RealGetVersionEx(LPOSVERSIONINFO lpVersionInfo){
	if (!GetVersionEx(lpVersionInfo)) return FALSE;
	RtlGetNtVersionNumbers(&lpVersionInfo->dwMajorVersion,&lpVersionInfo->dwMinorVersion,&lpVersionInfo->dwBuildNumber);
	lpVersionInfo->dwBuildNumber&=0x0000FFFF;
	return TRUE;
}


#define SP_OFFERT (MaxProcessNumber*sizeof(SYSTEM_PROCESSES*))
NTSTATUS NtQueryProcessInfo(CDMAlloc<SYSTEM_PROCESSES**> &pProcess,int *pProcessNumber){

	//尝试第一次获取
	SYSTEM_PROCESSES *pSystemProcess=(SYSTEM_PROCESSES *)(pProcess+MaxProcessNumber);
	ULONG nBytesReturn;
	NTSTATUS result;
	if ((ULONG)pProcess.GetSize()>SP_OFFERT)
		result=NtQueryProcessInfo(pSystemProcess,
			(ULONG)pProcess.GetSize()-SP_OFFERT,&nBytesReturn);
	else
		result=NtQueryProcessInfo(NULL,0,&nBytesReturn);
	if (NT_SUCCESS(result)) goto lBuildIndex;

	//如果第一次获取不成功是因为缓冲区长度不足时，则分配缓冲区重新获取
	if (result!=STATUS_INFO_LENGTH_MISMATCH) return result;
	if (!pProcess.DemandSize(nBytesReturn+SP_OFFERT)) return STATUS_NO_MEMORY;
	pSystemProcess=(SYSTEM_PROCESSES *)(pProcess+MaxProcessNumber);
	result=NtQueryProcessInfo(pSystemProcess,
		(ULONG)pProcess.GetSize()-SP_OFFERT,&nBytesReturn);

	if (!NT_SUCCESS(result)) return result;

	//建立索引
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

