
// WinProcess.h
#pragma once

#ifndef PROCESS_QUERY_LIMITED_INFORMATION
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#endif

extern NTSTATUS (NTAPI *NtSuspendProcess)(HANDLE);
extern NTSTATUS (NTAPI *NtResumeProcess)(HANDLE);
extern NTSTATUS (NTAPI *NtQueryInformationProcess)(HANDLE, DWORD, PVOID, ULONG, PULONG);
extern void (NTAPI *RtlGetNtVersionNumbers)(LPDWORD,LPDWORD,LPDWORD);
extern ULONG (NTAPI *RtlNtStatusToDosError)(NTSTATUS Status);
extern NTSTATUS (NTAPI *NtQueryInformationProcess)(HANDLE, DWORD, PVOID, ULONG, PULONG);

class CProcess : public CKernelObject {
private:
	static BOOL bExistQueryLimitedInformation;
	static BOOL ExistQueryLimitedInformation();
	static DWORD dwPageSize;
	static DWORD GetPageSize();

protected:
	HANDLE &m_hProcess;

#ifdef _DEBUG
	DWORD m_dwAccess;
	BOOL HaveAccess(DWORD dwDesiredAccess) const {
		assert(m_hProcess);
		return (m_dwAccess & dwDesiredAccess) == dwDesiredAccess;
	}
#else
	BOOL HaveAccess(DWORD dwDesiredAccess) const {return TRUE;}
#endif

public:
	CProcess():m_hProcess(m_Handle){}
	explicit CProcess(HANDLE handle):m_hProcess(m_Handle){ Attach(handle);}
	CProcess(CProcess &object):m_hProcess(m_Handle){ Attach((HANDLE)object); }

	void Attach(HANDLE hProcess){
		m_hProcess=hProcess;
#ifdef _DEBUG
		m_dwAccess=PROCESS_ALL_ACCESS;
#endif
	}

	//OpenProcess （如果系统不支持 PROCESS_QUERY_LIMITED_INFORMATION ，会自动转换成 PROCESS_QUERY_INFORMATION | PROCESS_VM_READ）
	BOOL Open(DWORD dwProcessId,DWORD dwDesiredAccess=PROCESS_ALL_ACCESS,BOOL bInheritHandle=FALSE){
		assert(m_hProcess==NULL);
		if ((dwDesiredAccess | PROCESS_QUERY_LIMITED_INFORMATION) && !bExistQueryLimitedInformation){
			dwDesiredAccess = dwDesiredAccess &
				(0xFFFFFFFF - PROCESS_QUERY_LIMITED_INFORMATION) |
				(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ) ;
		}
#ifdef _DEBUG
		if (!(m_hProcess=::OpenProcess(dwDesiredAccess,bInheritHandle,dwProcessId)))
			return FALSE;
		m_dwAccess=dwDesiredAccess;
		return TRUE;
#else
		return (m_hProcess=::OpenProcess(dwDesiredAccess,bInheritHandle,dwProcessId)) != NULL;
#endif
	}

	// CreateProcess
	BOOL Create(
		LPCTSTR               lpApplicationName=NULL,
		LPTSTR                lpCommandLine=NULL,
		LPSECURITY_ATTRIBUTES lpProcessAttributes=NULL,
		LPSECURITY_ATTRIBUTES lpThreadAttributes=NULL,
		BOOL                  bInheritHandles=FALSE,
		DWORD                 dwCreationFlags=0,
		LPVOID                lpEnvironment=NULL,
		LPCTSTR               lpCurrentDirectory=NULL,
		LPSTARTUPINFO         lpStartupInfo=NULL,
		LPPROCESS_INFORMATION lpProcessInformation=NULL
		)
	{
		assert(m_hProcess==NULL);

		STARTUPINFO stStartupInfo;
		if (lpStartupInfo==NULL){
			memset(&stStartupInfo,0,sizeof(stStartupInfo));
			stStartupInfo.cb=sizeof(stStartupInfo);
			stStartupInfo.dwFlags=0;
			lpStartupInfo=&stStartupInfo;
		}
		PROCESS_INFORMATION stProcessInfo;
		BOOL bAutoFreeHandle = lpProcessInformation==NULL;
		if (lpProcessInformation==NULL){
			lpProcessInformation=&stProcessInfo;
		}
		BOOL bResult=
			::CreateProcess(
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpProcessInformation
				);
		if (bResult){
			if (bAutoFreeHandle)
				CloseHandle(stProcessInfo.hThread);
			m_hProcess = stProcessInfo.hProcess;
#ifdef _DEBUG
			m_dwAccess = PROCESS_ALL_ACCESS;
#endif
		}
		return bResult;
	}

	//GetProcessId
	DWORD GetId() const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return GetProcessId(m_hProcess);
	}

	//TerminateProcess
	BOOL Terminate(UINT uExitCode=1){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_TERMINATE));
		return ::TerminateProcess(m_hProcess,uExitCode);
	}

	//GetExitCodeProcess
	BOOL GetExitCode(LPDWORD lpExitCode) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetExitCodeProcess(m_hProcess,lpExitCode);
	}

	//NtSuspendProcess
	NTSTATUS NtSuspend(){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_SUSPEND_RESUME));
		return ::NtSuspendProcess(m_hProcess);
	}

	//NtResumeProcess
	NTSTATUS NtResume(){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_SUSPEND_RESUME));
		return ::NtResumeProcess(m_hProcess);
	}

	//GetPriorityClass
	DWORD GetPriorityClass() const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetPriorityClass(m_hProcess);
	}

	//SetPriorityClass
	BOOL SetPriorityClass(DWORD dwPriorityClass){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_SET_INFORMATION));
		return ::SetPriorityClass(m_hProcess,dwPriorityClass);
	}

	//GetProcessPriorityBoost
	BOOL GetPriorityBoost(PBOOL pDisablePriorityBoost) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetProcessPriorityBoost(m_hProcess,pDisablePriorityBoost);
	}

	//SetProcessPriorityBoost 
	BOOL SetPriorityBoost(BOOL DisablePriorityBoost){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_SET_INFORMATION));
		return ::SetProcessPriorityBoost(m_hProcess,DisablePriorityBoost);
	}

#if _WIN32_WINNT >= 0x0501
	//GetProcessHandleCount
	BOOL GetHandleCount(PDWORD pdwHandleCount) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetProcessHandleCount(m_hProcess,pdwHandleCount);
	}
#endif // (_WIN32_WINNT >= 0x0501)

	//GetProcessIoCounters
	BOOL GetIoCounters(PIO_COUNTERS lpIoCounters) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetProcessIoCounters(m_hProcess,lpIoCounters);
	}

	//GetProcessTimes
	BOOL GetTimes(LPFILETIME lpCreationTime,LPFILETIME lpExitTime,LPFILETIME lpKernelTime,LPFILETIME lpUserTime) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetProcessTimes(m_hProcess,lpCreationTime,lpExitTime,lpKernelTime,lpUserTime);
	}

	BOOL GetUserName(LPTSTR szUserName,DWORD nSize);

	//QueryWorkingSet
	BOOL QueryWorkingSet(PVOID pv,DWORD cb) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::QueryWorkingSet(m_hProcess,pv,cb);
	}

	BOOL GetWorkingSetInfo(SIZE_T *pWSTotal,SIZE_T *pWSPrivate,SIZE_T *pWSShareable,SIZE_T *pWSShared) const ;
	BOOL GetPrivateWorkingSet(SIZE_T *pWSPrivate) const {
		return GetWorkingSetInfo(NULL,pWSPrivate,NULL,NULL);
	}

	//GetProcessWorkingSetSize
	BOOL GetWorkingSetSize(PSIZE_T lpMinimumWorkingSetSize,PSIZE_T lpMaximumWorkingSetSize) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::GetProcessWorkingSetSize(m_hProcess,lpMinimumWorkingSetSize,lpMaximumWorkingSetSize);
	}

	//SetProcessWorkingSetSize
	BOOL SetWorkingSetSize(SIZE_T dwMinimumWorkingSetSize,SIZE_T dwMaximumWorkingSetSize) {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_SET_QUOTA));
		return ::SetProcessWorkingSetSize(m_hProcess,dwMinimumWorkingSetSize,dwMaximumWorkingSetSize);
	}

	//EmptyWorkingSet
	BOOL EmptyWorkingSet(){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_SET_QUOTA) && HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		return ::EmptyWorkingSet(m_hProcess);
	}

	//ReadProcessMemory
	BOOL ReadMemory(LPCVOID lpBaseAddress,LPVOID lpBuffer,SIZE_T nSize,SIZE_T *lpNumberOfBytesRead=NULL) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_VM_READ));
		return ::ReadProcessMemory(m_hProcess,lpBaseAddress,lpBuffer,nSize,lpNumberOfBytesRead);
	}

	//WriteProcessMemory
	BOOL WriteMemory(LPVOID lpBaseAddress,LPCVOID lpBuffer,SIZE_T nSize,SIZE_T *lpNumberOfBytesWritten){
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION));
		return ::WriteProcessMemory(m_hProcess,lpBaseAddress,lpBuffer,nSize,lpNumberOfBytesWritten);
	}

	//CreateRemoteThread
	HANDLE CreateRemoteThread(
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID                 lpParameter = NULL,
		LPDWORD                lpThreadId = NULL,
		DWORD                  dwCreationFlags = 0,
		SIZE_T                 dwStackSize = 0,
		LPSECURITY_ATTRIBUTES  lpThreadAttributes = NULL)
	{
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ));
		return ::CreateRemoteThread(m_hProcess,lpThreadAttributes,dwStackSize,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);
	}

	//NtQueryInformationProcess
	NTSTATUS NtQueryInformation(
		PROCESSINFOCLASS ProcessInformationClass,
		PVOID ProcessInformation,
		ULONG ProcessInformationLength,
		PULONG ReturnLength) const 
	{
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		NTSTATUS ntstatus=
			::NtQueryInformationProcess(
				m_hProcess,ProcessInformationClass,ProcessInformation,ProcessInformationLength,ReturnLength
				);
		assert(ntstatus!=STATUS_INFO_LENGTH_MISMATCH);
		return ntstatus;
	}

	//NtQueryInformationProcess(ProcessBasicInformation)
	NTSTATUS NtQueryBasicInformation(PROCESS_BASIC_INFORMATION *lpPBI) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		ULONG dwBytesReturn;
		NTSTATUS ntstatus=NtQueryInformation(ProcessBasicInformation,lpPBI,sizeof(*lpPBI),&dwBytesReturn);
		if (NT_SUCCESS(ntstatus))
			assert(dwBytesReturn==sizeof(*lpPBI));
		return ntstatus;
	}

	//NtQueryInformationProcess(ProcessWow64Information)
	NTSTATUS NtQueryWow64Information(DWORD_PTR *pIsWow64Process) const {
		assert(m_hProcess);
		assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));
		ULONG dwBytesReturn;
		NTSTATUS ntstatus=NtQueryInformation(ProcessWow64Information,pIsWow64Process,sizeof(DWORD_PTR),&dwBytesReturn);
		if (NT_SUCCESS(ntstatus))
			assert(dwBytesReturn==sizeof(DWORD_PTR));
		return ntstatus;
	}

private:
	BOOL GetPathString(LPTSTR lpStr,SIZE_T nLength,PROCESSINFOCLASS ProcessInformationClass) const ;
	BOOL GetPathStringByMemory(LPTSTR lpStr,SIZE_T nLength,PROCESSINFOCLASS ProcessInformationClass) const ;

public:
	BOOL GetImagePath(LPTSTR lpImagePath,SIZE_T nLength) const {
		return GetPathString(lpImagePath,nLength,ProcessImagePath);
	}
	BOOL GetCmdLine(LPTSTR lpCommandLine,SIZE_T nLength) const {
		return GetPathString(lpCommandLine,nLength,ProcessCommandLine);
	}


	static BOOL AquireToken(HANDLE hProcess,LPCTSTR PrivilegeName);
};


