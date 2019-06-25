
// WinSystemInfo.h

#pragma once


#define MaxProcessNumber 4096


extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);
extern void (NTAPI *RtlGetNtVersionNumbers)(LPDWORD,LPDWORD,LPDWORD);
extern NTSTATUS (NTAPI *NtQuerySystemInformation)(UINT, PVOID, ULONG, PULONG);

// 0 Y N
typedef struct _SYSTEM_BASIC_INFORMATION { 
	ULONG Unknow1;
	ULONG TimerResolution;
	ULONG PageSize;
	ULONG NumberOfPhysicalPages;
	ULONG LowestPhysicalPageNumber;
	ULONG HighestPhysicalPageNumber;
	ULONG AllocationGranularity;
	ULONG_PTR MinimumUserModeAddress;
	ULONG_PTR MaximumUserModeAddress;
	ULONG_PTR ActiveProcessorsAffinityMask;
	CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

// 2 Y N
typedef struct _SYSTEM_PERFORMANCE_INFORMATION { 
	LARGE_INTEGER IdleProcessTime; 
	LARGE_INTEGER IoReadTransferCount; 
	LARGE_INTEGER IoWriteTransferCount; 
	LARGE_INTEGER IoOtherTransferCount; 
	ULONG IoReadOperationCount; 
	ULONG IoWriteOperationCount; 
	ULONG IoOtherOperationCount; 
	ULONG AvailablePages; 
	ULONG CommittedPages; 
	ULONG CommitLimit; 
	ULONG PeakCommitment; 
	ULONG PageFaultCount; 
	ULONG CopyOnWriteCount; 
	ULONG TransitionCount; 
	ULONG CacheTransitionCount; 
	ULONG DemandZeroCount; 
	ULONG PageReadCount; 
	ULONG PageReadIoCount; 
	ULONG CacheReadCount; 
	ULONG CacheIoCount; 
	ULONG DirtyPagesWriteCount; 
	ULONG DirtyWriteIoCount; 
	ULONG MappedPagesWriteCount; 
	ULONG MappedWriteIoCount; 
	ULONG PagedPoolPages; 
	ULONG NonPagedPoolPages; 
	ULONG PagedPoolAllocs; 
	ULONG PagedPoolFrees; 
	ULONG NonPagedPoolAllocs; 
	ULONG NonPagedPoolFrees; 
	ULONG FreeSystemPtes; 
	ULONG ResidentSystemCodePage; 
	ULONG TotalSystemDriverPages; 
	ULONG TotalSystemCodePages; 
	ULONG NonPagedPoolLookasideHits; 
	ULONG PagedPoolLookasideHits; 
	ULONG Spare3Count; 
	ULONG ResidentSystemCachePage; 
	ULONG ResidentPagedPoolPage; 
	ULONG ResidentSystemDriverPage; 
	ULONG CcFastReadNoWait; 
	ULONG CcFastReadWait; 
	ULONG CcFastReadResourceMiss; 
	ULONG CcFastReadNotPossible; 
	ULONG CcFastMdlReadNoWait; 
	ULONG CcFastMdlReadWait; 
	ULONG CcFastMdlReadResourceMiss; 
	ULONG CcFastMdlReadNotPossible; 
	ULONG CcMapDataNoWait; 
	ULONG CcMapDataWait; 
	ULONG CcMapDataNoWaitMiss; 
	ULONG CcMapDataWaitMiss; 
	ULONG CcPinMappedDataCount; 
	ULONG CcPinReadNoWait; 
	ULONG CcPinReadWait; 
	ULONG CcPinReadNoWaitMiss; 
	ULONG CcPinReadWaitMiss; 
	ULONG CcCopyReadNoWait; 
	ULONG CcCopyReadWait; 
	ULONG CcCopyReadNoWaitMiss; 
	ULONG CcCopyReadWaitMiss; 
	ULONG CcMdlReadNoWait; 
	ULONG CcMdlReadWait; 
	ULONG CcMdlReadNoWaitMiss; 
	ULONG CcMdlReadWaitMiss; 
	ULONG CcReadAheadIos; 
	ULONG CcLazyWriteIos; 
	ULONG CcLazyWritePages; 
	ULONG CcDataFlushes; 
	ULONG CcDataPages; 
	ULONG ContextSwitches; 
	ULONG FirstLevelTbFills; 
	ULONG SecondLevelTbFills; 
	ULONG SystemCalls; 
	ULONG Unknow[8];
} SYSTEM_PERFORMANCE_INFORMATION,*PSYSTEM_PERFORMANCE_INFORMATION;

// 3 Y N
typedef struct _SYSTEM_TIMEOFDAY_INFORMATION { 
	LARGE_INTEGER BootTime; 
	LARGE_INTEGER CurrentTime; 
	LARGE_INTEGER TimeZoneBias; 
	ULONG TimeZoneId; 
	ULONG Reserved; 
} SYSTEM_TIMEOFDAY_INFORMATION, *PSYSTEM_TIMEOFDAY_INFORMATION;

// 5 Y N
typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER KernelTime; //CPU�ں�ģʽʹ��ʱ��
	LARGE_INTEGER UserTime; //CPU�û�ģʽʹ��ʱ��
	LARGE_INTEGER CreateTime; //�̴߳���ʱ��
	ULONG WaitTime; //�ȴ�ʱ��
	PVOID StartAddress; //�߳̿�ʼ�������ַ
	CLIENT_ID ClientId; //�̺߳ͽ���ID
	KPRIORITY Priority; //�߳����ȼ�
	KPRIORITY BasePriority; //�������ȼ�
	ULONG ContextSwitchCount; //�����л���Ŀ
	ULONG ThreadState; //��ǰ״̬
	KWAIT_REASON WaitReason; //�ȴ�ԭ��
	ULONG Unknow;
} SYSTEM_THREADS, *PSYSTEM_THREADS;
typedef struct _SYSTEM_PROCESSES {
	ULONG NextEntryOffset; //��һ�����̽ṹ��ƫ����
	ULONG NumberOfThreads; //�߳�����
	LONGLONG PrivateWorkingSetSize; //ר�ù����� ��Win7 ���ϵİ汾��Ч��
	DWORD Unknown[4];
	LARGE_INTEGER CreateTime;  //����ʱ��
	LARGE_INTEGER UserTime;  //�û�ģʽ(Ring3)��CPUʱ��
	LARGE_INTEGER KernelTime;  //�ں�ģʽ(Ring0)��CPUʱ��
	UNICODE_STRING ProcessName; //��������
	KPRIORITY BasePriority; //����Ȩ��
	HANDLE ProcessId; //����PID
	HANDLE InheritedFromProcessId; //������PID
	ULONG HandleCount; //�������
	ULONG SessionId; //�ỰID
	ULONG_PTR PageDirectoryBase;

	// �ڴ���ز���
	// This part corresponds to VM_COUNTERS_EX. 
	// NOTE: *NOT* THE SAME AS VM_COUNTERS! 
	// 
	SIZE_T PeakVirtualSize; //�����ڴ��ֵ
	SIZE_T VirtualSize; //�����ڴ�
	ULONG PageFaultCount; //ҳ�����
	SIZE_T PeakWorkingSetSize; //��������ֵ
	SIZE_T WorkingSetSize; //������
	SIZE_T QuotaPeakPagedPoolUsage; //ҳ�滺��ط�ֵ
	SIZE_T QuotaPagedPoolUsage; //ҳ�滺���
	SIZE_T QuotaPeakNonPagedPoolUsage; //��ҳ�滺��ط�ֵ
	SIZE_T QuotaNonPagedPoolUsage; //��ҳ�滺���
	SIZE_T PageFileUsage; //ҳ���ļ�
	SIZE_T PeakPageFileUsage; //ҳ���ļ���ֵ
	SIZE_T PrivateUsage; //�ύ��С

	// IO ��ز���
	// This part corresponds to IO_COUNTERS 
	// 
	LARGE_INTEGER ReadOperationCount; //I/O��ȡ
	LARGE_INTEGER WriteOperationCount; //I/Oд��
	LARGE_INTEGER OtherOperationCount; //I/O����
	LARGE_INTEGER ReadTransferCount; //I/O��ȡ�ֽ�
	LARGE_INTEGER WriteTransferCount; //I/Oд���ֽ�
	LARGE_INTEGER OtherTransferCount; //I/O�����ֽ�

	struct _SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

// 8 Y N
typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION { 
	LARGE_INTEGER IdleTime; //����ʱ��
	LARGE_INTEGER KernelTime; //�ں�ģʽʱ��
	LARGE_INTEGER UserTime; //�û�ģʽʱ��
	LARGE_INTEGER DpcTime; //�ӳٹ��̵���ʱ��
	LARGE_INTEGER InterruptTime; //�ж�ʱ��
	ULONG InterruptCount; //�жϴ���
	BYTE Unknow[148];
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

// 16 Y N
typedef struct _SYSTEM_HANDLE_INFORMATION { 
	ULONG ProcessID; //���̵ı�ʶID 
	UCHAR ObjectTypeNumber; //�������� 
	UCHAR Flags; //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT 
	USHORT Handle; //����������ֵ 
	PVOID Object; //��������ָ���ں˶����ַ 
	ACCESS_MASK GrantedAccess; //�������ʱ��׼��Ķ���ķ���Ȩ 
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
typedef struct _SYSTEM_ALL_HANDLE_INFORMATION {
	ULONG NumberOfHandle;
	_SYSTEM_HANDLE_INFORMATION stHandle[1];
} SYSTEM_ALL_HANDLE_INFORMATION;



//��ȡϵͳ�İ汾��Ϣ
BOOL WINAPI RealGetVersionEx(LPOSVERSIONINFO lpVersionInfo);

// 0 - ��ȡϵͳ������Ϣ
NTSTATUS NtQuerySystemBasicInfo(SYSTEM_BASIC_INFORMATION *pSystemBasicInfo);

// 2 - ��ȡ������Ϣ
NTSTATUS NtQueryPerformanceInfo(SYSTEM_PERFORMANCE_INFORMATION *pPerformanceInfo);

// 3 - ��ȡʱ��������Ϣ
NTSTATUS NtQueryTimeOfDayInfo(SYSTEM_TIMEOFDAY_INFORMATION *pTimeOfDayInfo);

// 5 - ��ȡ������Ϣ
NTSTATUS NtQueryProcessInfo(SYSTEM_PROCESSES *pSystemProcess,ULONG nBufferSize,PULONG nReturnLength);
NTSTATUS NtQueryProcessInfo(CDMAlloc<SYSTEM_PROCESSES**> &pProcess,int *pProcessNumber);

// 8 - ��ȡ������ʱ����Ϣ
NTSTATUS NtQueryProcessorTimeInfo(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION *pProcessorTimeInfo);

// 16 - ��ȡϵͳ�����Ϣ
NTSTATUS NtQuerySystemHandleInfo(SYSTEM_ALL_HANDLE_INFORMATION *pSystemHandleInfo,ULONG nBufferSize,PULONG nReturnLength);
NTSTATUS NtQuerySystemHandleInfo(CDMAlloc<SYSTEM_ALL_HANDLE_INFORMATION*> &pSystemHandleInfo);






// inline functions

inline NTSTATUS NtQuerySystemBasicInfo(SYSTEM_BASIC_INFORMATION *pSystemBasicInfo) {
	return NtQuerySystemInformation(SystemBasicInformation,pSystemBasicInfo,
		sizeof(SYSTEM_BASIC_INFORMATION),NULL);
}
inline NTSTATUS NtQueryPerformanceInfo(SYSTEM_PERFORMANCE_INFORMATION *pPerformanceInfo){
	return NtQuerySystemInformation(SystemPerformanceInformation,pPerformanceInfo,
		sizeof(SYSTEM_PERFORMANCE_INFORMATION),NULL);
}
inline NTSTATUS NtQueryTimeOfDayInfo(SYSTEM_TIMEOFDAY_INFORMATION *pTimeOfDayInfo){
	return NtQuerySystemInformation(SystemTimeOfDayInformation,pTimeOfDayInfo,
		sizeof(SYSTEM_TIMEOFDAY_INFORMATION),NULL);
}
inline NTSTATUS NtQueryProcessInfo(SYSTEM_PROCESSES *pSystemProcess,ULONG nBufferSize,PULONG nReturnLength){
	return NtQuerySystemInformation(SystemProcessesAndThreadsInformation,pSystemProcess,
		nBufferSize,nReturnLength);
}
inline NTSTATUS NtQueryProcessorTimeInfo(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION *pProcessorTimeInfo){
	return NtQuerySystemInformation(SystemProcessorTimes,pProcessorTimeInfo,
		sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION),NULL);
}

inline NTSTATUS NtQuerySystemHandleInfo(SYSTEM_ALL_HANDLE_INFORMATION *pSystemHandleInfo,ULONG nBufferSize,PULONG nReturnLength){
	return NtQuerySystemInformation(SystemHandleInformation,pSystemHandleInfo,
		nBufferSize,nReturnLength);
}
inline NTSTATUS NtQuerySystemHandleInfo(CDMAlloc<SYSTEM_ALL_HANDLE_INFORMATION*> &pSystemHandleInfo){
	pSystemHandleInfo.DemandSize(0x1000);

	ULONG nBytesReturn;
	NTSTATUS result=NtQuerySystemHandleInfo(pSystemHandleInfo,(ULONG)pSystemHandleInfo.GetSize(),&nBytesReturn);
	if (result!=STATUS_INFO_LENGTH_MISMATCH) return result;
	if (!pSystemHandleInfo.DemandSize(nBytesReturn)) return STATUS_NO_MEMORY;

	return NtQuerySystemHandleInfo(pSystemHandleInfo,(ULONG)pSystemHandleInfo.GetSize(),&nBytesReturn);
}



