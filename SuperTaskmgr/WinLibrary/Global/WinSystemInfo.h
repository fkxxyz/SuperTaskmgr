
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
	LARGE_INTEGER KernelTime; //CPU内核模式使用时间
	LARGE_INTEGER UserTime; //CPU用户模式使用时间
	LARGE_INTEGER CreateTime; //线程创建时间
	ULONG WaitTime; //等待时间
	PVOID StartAddress; //线程开始的虚拟地址
	CLIENT_ID ClientId; //线程和进程ID
	KPRIORITY Priority; //线程优先级
	KPRIORITY BasePriority; //基本优先级
	ULONG ContextSwitchCount; //环境切换数目
	ULONG ThreadState; //当前状态
	KWAIT_REASON WaitReason; //等待原因
	ULONG Unknow;
} SYSTEM_THREADS, *PSYSTEM_THREADS;
typedef struct _SYSTEM_PROCESSES {
	ULONG NextEntryOffset; //下一个进程结构体偏移量
	ULONG NumberOfThreads; //线程数量
	LONGLONG PrivateWorkingSetSize; //专用工作集 （Win7 以上的版本有效）
	DWORD Unknown[4];
	LARGE_INTEGER CreateTime;  //创建时间
	LARGE_INTEGER UserTime;  //用户模式(Ring3)的CPU时间
	LARGE_INTEGER KernelTime;  //内核模式(Ring0)的CPU时间
	UNICODE_STRING ProcessName; //进程名称
	KPRIORITY BasePriority; //优先权限
	HANDLE ProcessId; //进程PID
	HANDLE InheritedFromProcessId; //父进程PID
	ULONG HandleCount; //句柄数量
	ULONG SessionId; //会话ID
	ULONG_PTR PageDirectoryBase;

	// 内存相关参数
	// This part corresponds to VM_COUNTERS_EX. 
	// NOTE: *NOT* THE SAME AS VM_COUNTERS! 
	// 
	SIZE_T PeakVirtualSize; //虚拟内存峰值
	SIZE_T VirtualSize; //虚拟内存
	ULONG PageFaultCount; //页面错误
	SIZE_T PeakWorkingSetSize; //工作集峰值
	SIZE_T WorkingSetSize; //工作集
	SIZE_T QuotaPeakPagedPoolUsage; //页面缓冲池峰值
	SIZE_T QuotaPagedPoolUsage; //页面缓冲池
	SIZE_T QuotaPeakNonPagedPoolUsage; //非页面缓冲池峰值
	SIZE_T QuotaNonPagedPoolUsage; //非页面缓冲池
	SIZE_T PageFileUsage; //页面文件
	SIZE_T PeakPageFileUsage; //页面文件峰值
	SIZE_T PrivateUsage; //提交大小

	// IO 相关参数
	// This part corresponds to IO_COUNTERS 
	// 
	LARGE_INTEGER ReadOperationCount; //I/O读取
	LARGE_INTEGER WriteOperationCount; //I/O写入
	LARGE_INTEGER OtherOperationCount; //I/O其他
	LARGE_INTEGER ReadTransferCount; //I/O读取字节
	LARGE_INTEGER WriteTransferCount; //I/O写入字节
	LARGE_INTEGER OtherTransferCount; //I/O其他字节

	struct _SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

// 8 Y N
typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION { 
	LARGE_INTEGER IdleTime; //空闲时间
	LARGE_INTEGER KernelTime; //内核模式时间
	LARGE_INTEGER UserTime; //用户模式时间
	LARGE_INTEGER DpcTime; //延迟过程调用时间
	LARGE_INTEGER InterruptTime; //中断时间
	ULONG InterruptCount; //中断次数
	BYTE Unknow[148];
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

// 16 Y N
typedef struct _SYSTEM_HANDLE_INFORMATION { 
	ULONG ProcessID; //进程的标识ID 
	UCHAR ObjectTypeNumber; //对象类型 
	UCHAR Flags; //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT 
	USHORT Handle; //对象句柄的数值 
	PVOID Object; //对象句柄所指的内核对象地址 
	ACCESS_MASK GrantedAccess; //创建句柄时所准许的对象的访问权 
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
typedef struct _SYSTEM_ALL_HANDLE_INFORMATION {
	ULONG NumberOfHandle;
	_SYSTEM_HANDLE_INFORMATION stHandle[1];
} SYSTEM_ALL_HANDLE_INFORMATION;



//获取系统的版本信息
BOOL WINAPI RealGetVersionEx(LPOSVERSIONINFO lpVersionInfo);

// 0 - 获取系统基本信息
NTSTATUS NtQuerySystemBasicInfo(SYSTEM_BASIC_INFORMATION *pSystemBasicInfo);

// 2 - 获取性能信息
NTSTATUS NtQueryPerformanceInfo(SYSTEM_PERFORMANCE_INFORMATION *pPerformanceInfo);

// 3 - 获取时间日期信息
NTSTATUS NtQueryTimeOfDayInfo(SYSTEM_TIMEOFDAY_INFORMATION *pTimeOfDayInfo);

// 5 - 获取进程信息
NTSTATUS NtQueryProcessInfo(SYSTEM_PROCESSES *pSystemProcess,ULONG nBufferSize,PULONG nReturnLength);
NTSTATUS NtQueryProcessInfo(CDMAlloc<SYSTEM_PROCESSES**> &pProcess,int *pProcessNumber);

// 8 - 获取处理器时间信息
NTSTATUS NtQueryProcessorTimeInfo(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION *pProcessorTimeInfo);

// 16 - 获取系统句柄信息
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



