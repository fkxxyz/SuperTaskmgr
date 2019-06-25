
// NtKernelDefine.h"

#pragma once

#include "NtStatusDefine.h"

//
// Thread priority
//

typedef LONG KPRIORITY;



//
// Legacy thread affinity.
//

typedef ULONG_PTR KAFFINITY;
typedef KAFFINITY *PKAFFINITY;




#ifdef _NTDEF_

typedef UNICODE_STRING LSA_UNICODE_STRING, *PLSA_UNICODE_STRING;
typedef STRING LSA_STRING, *PLSA_STRING;
typedef OBJECT_ATTRIBUTES LSA_OBJECT_ATTRIBUTES, *PLSA_OBJECT_ATTRIBUTES;

#else // _NTDEF_

typedef struct _LSA_UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength/2), length_is(Length/2)]
#endif // MIDL_PASS
    PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING;

typedef struct _LSA_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} LSA_STRING, *PLSA_STRING;

typedef struct _LSA_OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PLSA_UNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} LSA_OBJECT_ATTRIBUTES, *PLSA_OBJECT_ATTRIBUTES;

#endif // _NTDEF_


//
// Pointer to an Asciiz string
//

typedef CHAR *PSZ;
typedef CONST char *PCSZ;

// begin_ntndis
//
// Counted String
//

typedef USHORT RTL_STRING_LENGTH_TYPE;

typedef struct _STRING {
    __maybevalid USHORT Length;
    __maybevalid USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;
typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;
typedef CONST STRING* PCOEM_STRING;

//
// CONSTCounted String
//

typedef struct _CSTRING {
    USHORT Length;
    USHORT MaximumLength;
    CONST char *Buffer;
} CSTRING;
typedef CSTRING *PCSTRING;
#define ANSI_NULL ((CHAR)0)     // winnt

typedef STRING CANSI_STRING;
typedef PSTRING PCANSI_STRING;

//
// Unicode strings are counted 16-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
//

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
	PWCH   Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#define UNICODE_NULL ((WCHAR)0) // winnt



typedef struct _RTL_USER_PROCESS_PARAMETERS {
  BYTE           Reserved1[16];
  PVOID          Reserved2[10];
  UNICODE_STRING ImagePathName;
  UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB { // Size: 0x1D8
/*000*/ UCHAR InheritedAddressSpace;
/*001*/ UCHAR ReadImageFileExecOptions;
/*002*/ UCHAR BeingDebugged;
/*003*/ UCHAR SpareBool; // Allocation size
/*004*/ HANDLE Mutant;
/*008*/ HINSTANCE ImageBaseAddress; // Instance
/*00C*/ VOID *DllList;
/*010*/ _RTL_USER_PROCESS_PARAMETERS *ProcessParameters;
/*014*/ ULONG SubSystemData;
/*018*/ HANDLE DefaultHeap;
/*01C*/ KSPIN_LOCK FastPebLock;
/*020*/ ULONG FastPebLockRoutine;
/*024*/ ULONG FastPebUnlockRoutine;
/*028*/ ULONG EnvironmentUpdateCount;
/*02C*/ ULONG KernelCallbackTable;
/*030*/ LARGE_INTEGER SystemReserved;
/*038*/ ULONG FreeList;
/*03C*/ ULONG TlsExpansionCounter;
/*040*/ ULONG TlsBitmap;
/*044*/ LARGE_INTEGER TlsBitmapBits;
/*04C*/ ULONG ReadOnlySharedMemoryBase;
/*050*/ ULONG ReadOnlySharedMemoryHeap;
/*054*/ ULONG ReadOnlyStaticServerData;
/*058*/ ULONG AnsiCodePageData;
/*05C*/ ULONG OemCodePageData;
/*060*/ ULONG UnicodeCaseTableData;
/*064*/ ULONG NumberOfProcessors;
/*068*/ LARGE_INTEGER NtGlobalFlag; // Address of a local copy
/*070*/ LARGE_INTEGER CriticalSectionTimeout;
/*078*/ ULONG HeapSegmentReserve;
/*07C*/ ULONG HeapSegmentCommit;
/*080*/ ULONG HeapDeCommitTotalFreeThreshold;
/*084*/ ULONG HeapDeCommitFreeBlockThreshold;
/*088*/ ULONG NumberOfHeaps;
/*08C*/ ULONG MaximumNumberOfHeaps;
/*090*/ ULONG ProcessHeaps;
/*094*/ ULONG GdiSharedHandleTable;
/*098*/ ULONG ProcessStarterHelper;
/*09C*/ ULONG GdiDCAttributeList;
/*0A0*/ KSPIN_LOCK LoaderLock;
/*0A4*/ ULONG OSMajorVersion;
/*0A8*/ ULONG OSMinorVersion;
/*0AC*/ USHORT OSBuildNumber;
/*0AE*/ USHORT OSCSDVersion;
/*0B0*/ ULONG OSPlatformId;
/*0B4*/ ULONG ImageSubsystem;
/*0B8*/ ULONG ImageSubsystemMajorVersion;
/*0BC*/ ULONG ImageSubsystemMinorVersion;
/*0C0*/ ULONG ImageProcessAffinityMask;
/*0C4*/ ULONG GdiHandleBuffer[0x22];
/*14C*/ ULONG PostProcessInitRoutine;
/*150*/ ULONG TlsExpansionBitmap;
/*154*/ UCHAR TlsExpansionBitmapBits[0x80];
/*1D4*/ ULONG SessionId;
} PEB, *PPEB;

typedef struct _RTL_USER_PROCESS_PARAMETERS_64 {
  BYTE           Reserved1[16];
  LONGLONG          Reserved2[10];
  UNICODE_STRING ImagePathName;
  UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS_64, *PRTL_USER_PROCESS_PARAMETERS_64;

typedef struct _PEB_64 { // Size: 0x1D8
/*000*/ UCHAR InheritedAddressSpace;
/*001*/ UCHAR ReadImageFileExecOptions;
/*002*/ UCHAR BeingDebugged;
/*003*/ UCHAR SpareBool; // Allocation size
/*004*/ LONGLONG Mutant;
/*008*/ LONGLONG ImageBaseAddress; // Instance
/*00C*/ LONGLONG DllList;
/*010*/ LONGLONG ProcessParameters;
/*014*/ ULONG SubSystemData;
/*018*/ LONGLONG DefaultHeap;
/*01C*/ LONGLONG FastPebLock;
/*020*/ ULONG FastPebLockRoutine;
/*024*/ ULONG FastPebUnlockRoutine;
/*028*/ ULONG EnvironmentUpdateCount;
/*02C*/ ULONG KernelCallbackTable;
/*030*/ LARGE_INTEGER SystemReserved;
/*038*/ ULONG FreeList;
/*03C*/ ULONG TlsExpansionCounter;
/*040*/ ULONG TlsBitmap;
/*044*/ LARGE_INTEGER TlsBitmapBits;
/*04C*/ ULONG ReadOnlySharedMemoryBase;
/*050*/ ULONG ReadOnlySharedMemoryHeap;
/*054*/ ULONG ReadOnlyStaticServerData;
/*058*/ ULONG AnsiCodePageData;
/*05C*/ ULONG OemCodePageData;
/*060*/ ULONG UnicodeCaseTableData;
/*064*/ ULONG NumberOfProcessors;
/*068*/ LARGE_INTEGER NtGlobalFlag; // Address of a local copy
/*070*/ LARGE_INTEGER CriticalSectionTimeout;
/*078*/ ULONG HeapSegmentReserve;
/*07C*/ ULONG HeapSegmentCommit;
/*080*/ ULONG HeapDeCommitTotalFreeThreshold;
/*084*/ ULONG HeapDeCommitFreeBlockThreshold;
/*088*/ ULONG NumberOfHeaps;
/*08C*/ ULONG MaximumNumberOfHeaps;
/*090*/ ULONG ProcessHeaps;
/*094*/ ULONG GdiSharedHandleTable;
/*098*/ ULONG ProcessStarterHelper;
/*09C*/ ULONG GdiDCAttributeList;
/*0A0*/ LONGLONG LoaderLock;
/*0A4*/ ULONG OSMajorVersion;
/*0A8*/ ULONG OSMinorVersion;
/*0AC*/ USHORT OSBuildNumber;
/*0AE*/ USHORT OSCSDVersion;
/*0B0*/ ULONG OSPlatformId;
/*0B4*/ ULONG ImageSubsystem;
/*0B8*/ ULONG ImageSubsystemMajorVersion;
/*0BC*/ ULONG ImageSubsystemMinorVersion;
/*0C0*/ ULONG ImageProcessAffinityMask;
/*0C4*/ ULONG GdiHandleBuffer[0x22];
/*14C*/ ULONG PostProcessInitRoutine;
/*150*/ ULONG TlsExpansionBitmap;
/*154*/ UCHAR TlsExpansionBitmapBits[0x80];
/*1D4*/ ULONG SessionId;
} PEB_64, *PPEB_64;


typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation, // 0
	ProcessQuotaLimits, // 1
	ProcessIoCounters, // 2
	ProcessVmCounters, // 3
	ProcessTimes, // 4
	ProcessBasePriority, // 5
	ProcessRaisePriority, // 6
	ProcessDebugPort, // 7
	ProcessExceptionPort, // 8
	ProcessAccessToken, // 9
	ProcessLdtInformation, // 10
	ProcessLdtSize, // 11
	ProcessDefaultHardErrorMode, // 12
	ProcessIoPortHandlers, // 13
	ProcessPooledUsageAndLimits, // 14
	ProcessWorkingSetWatch, // 15
	ProcessUserModeIOPL, // 16
	ProcessEnableAlignmentFaultFixup, // 17
	ProcessPriorityClass, // 18
	ProcessWx86Information, // 19
	ProcessHandleCount, // 20
	ProcessAffinityMask, // 21
	ProcessPriorityBoost, // 22
	ProcessDeviceMap, // 23
	ProcessSessionInformation, // 24
	ProcessForegroundInformation, // 25
	ProcessWow64Information, // 26
	ProcessImageFileName, // 27
	ProcessLUIDDeviceMapsEnabled, // 28
	ProcessBreakOnTermination, // 29
	ProcessDebugObjectHandle, // 30
	ProcessDebugFlags, // 31
	ProcessHandleTracing, // 32
	ProcessUnknown33, // 33
	ProcessUnknown34, // 34
	ProcessUnknown35, // 35
	ProcessCookie, // 36
	MaxProcessInfoClass, // 37
	ProcessImagePath=43, // 43
	ProcessCommandLine=60 // 60
} PROCESSINFOCLASS;


//
// Basic and Extended Basic Process Information
//  NtQueryInformationProcess using ProcessBasicInformation
//

typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION,*PPROCESS_BASIC_INFORMATION;

typedef struct _PROCESS_EXTENDED_BASIC_INFORMATION {
    SIZE_T Size;    // Ignored as input, written with structure size on output
    PROCESS_BASIC_INFORMATION BasicInfo;
    union {
        ULONG Flags;
        struct {
            ULONG IsProtectedProcess : 1;
            ULONG IsWow64Process : 1;
            ULONG IsProcessDeleting : 1;
            ULONG IsCrossSessionCreate : 1;
            ULONG IsFrozen : 1;
            ULONG IsBackground : 1;
            ULONG IsStronglyNamed : 1;
            ULONG IsSecureProcess : 1;
            ULONG SpareBits : 24;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
} PROCESS_EXTENDED_BASIC_INFORMATION, *PPROCESS_EXTENDED_BASIC_INFORMATION;


//
// ClientId
//

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID;
typedef CLIENT_ID *PCLIENT_ID;


typedef enum _KWAIT_REASON {
	Executive = 0,
	FreePage = 1,
	PageIn = 2,
	PoolAllocation = 3,
	DelayExecution = 4,
	Suspended = 5,
	UserRequest = 6,
	WrExecutive = 7,
	WrFreePage = 8,
	WrPageIn = 9,
	WrPoolAllocation = 10,
	WrDelayExecution = 11,
	WrSuspended = 12,
	WrUserRequest = 13,
	WrEventPair = 14,
	WrQueue = 15,
	WrLpcReceive = 16,
	WrLpcReply = 17,
	WrVirtualMemory = 18,
	WrPageOut = 19,
	WrRendezvous = 20,
	Spare2 = 21,
	Spare3 = 22,
	Spare4 = 23,
	Spare5 = 24,
	WrCalloutStack = 25,
	WrKernel = 26,
	WrResource = 27,
	WrPushLock = 28,
	WrMutex = 29,
	WrQuantumEnd = 30,
	WrDispatchInt = 31,
	WrPreempted = 32,
	WrYieldExecution = 33,
	WrFastMutex = 34,
	WrGuardedMutex = 35,
	WrRundown = 36,
	MaximumWaitReason = 37
} KWAIT_REASON;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,	// 0 Y N
	SystemProcessorInformation,	   // 1 Y N
	SystemPerformanceInformation,		// 2 Y N
	SystemTimeOfDayInformation,	   // 3 Y N
	SystemNotImplemented1,	// 4 Y N
	SystemProcessesAndThreadsInformation, // 5 Y N
	SystemCallCounts,		  // 6 Y N
	SystemConfigurationInformation,		// 7 Y N
	SystemProcessorTimes,	// 8 Y N
	SystemGlobalFlag,		  // 9 Y Y
	SystemNotImplemented2,	// 10 Y N
	SystemModuleInformation,	   // 11 Y N
	SystemLockInformation,	// 12 Y N
	SystemNotImplemented3,	// 13 Y N
	SystemNotImplemented4,	// 14 Y N
	SystemNotImplemented5,	// 15 Y N
	SystemHandleInformation,	   // 16 Y N
	SystemObjectInformation,	   // 17 Y N
	SystemPagefileInformation,	   // 18 Y N
	SystemInstructionEmulationCounts,	// 19 Y N
	SystemInvalidInfoClass1,	   // 20
	SystemCacheInformation,	// 21 Y Y
	SystemPoolTagInformation,	   // 22 Y N
	SystemProcessorStatistics,	   // 23 Y N
	SystemDpcInformation,	// 24 Y Y
	SystemNotImplemented6,	// 25 Y N
	SystemLoadImage,		  // 26 N Y
	SystemUnloadImage,		  // 27 N Y
	SystemTimeAdjustment,	// 28 Y Y
	SystemNotImplemented7,	// 29 Y N
	SystemNotImplemented8,	// 30 Y N
	SystemNotImplemented9,	// 31 Y N
	SystemCrashDumpInformation,	   // 32 Y N
	SystemExceptionInformation,	   // 33 Y N
	SystemCrashDumpStateInformation,	// 34 Y Y/N
	SystemKernelDebuggerInformation,	// 35 Y N
	SystemContextSwitchInformation,		// 36 Y N
	SystemRegistryQuotaInformation,		// 37 Y Y
	SystemLoadAndCallImage,	// 38 N Y
	SystemPrioritySeparation,	   // 39 N Y
	SystemNotImplemented10,	// 40 Y N
	SystemNotImplemented11,	// 41 Y N
	SystemInvalidInfoClass2,	   // 42
	SystemInvalidInfoClass3,	   // 43
	SystemTimeZoneInformation,	   // 44 Y N
	SystemLookasideInformation,	   // 45 Y N
	SystemSetTimeSlipEvent,	// 46 N Y
	SystemCreateSession,	// 47 N Y
	SystemDeleteSession,	// 48 N Y
	SystemInvalidInfoClass4,	   // 49
	SystemRangeStartInformation,		// 50 Y N
	SystemVerifierInformation,	   // 51 Y Y
	SystemAddVerifier,		  // 52 N Y
	SystemSessionProcessesInformation	// 53 Y N
} SYSTEM_INFORMATION_CLASS;