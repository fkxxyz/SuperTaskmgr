
// WinProcess.cpp

#include "stdafx.h"

#include "WinProcess.h"
#pragma comment (lib,"Psapi.lib")



BOOL CProcess::bExistQueryLimitedInformation=CProcess::ExistQueryLimitedInformation();
BOOL CProcess::ExistQueryLimitedInformation(){
    DWORD dwMajorVersion,dwMinorVersion,dwBuildNumber;
	RtlGetNtVersionNumbers(&dwMajorVersion,&dwMinorVersion,&dwBuildNumber);
	return dwMajorVersion>=6;
}

DWORD CProcess::dwPageSize=CProcess::GetPageSize();
DWORD CProcess::GetPageSize(){
	SYSTEM_BASIC_INFORMATION sbi;
	NtQuerySystemBasicInfo(&sbi);
	return sbi.PageSize;
}

//通过读取进程内存取得结果
BOOL CProcess::GetPathStringByMemory(LPTSTR lpStr,SIZE_T nLength,PROCESSINFOCLASS ProcessInformationClass) const {
	int nLen; //结果字符串长度

	//读取进程的基本信息（从中得到进程 peb 的地址）
	NTSTATUS ntstatus;
	PROCESS_BASIC_INFORMATION pbi;
	ntstatus=NtQueryBasicInformation(&pbi);
	if (!NT_SUCCESS(ntstatus)){
		SetLastError(RtlNtStatusToDosError(ntstatus));
		return FALSE;
	}

	//读取进程的内存，得到 ProcessParameters 结构体的地址
	SIZE_T ck;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	if (!
		ReadMemory(&(pbi.PebBaseAddress)->ProcessParameters,&ProcessParameters,sizeof(ProcessParameters),&ck)
			||
		ck!=sizeof(ProcessParameters)
		)
	{
		return FALSE;
	}

	UNICODE_STRING *pus;
	if (ProcessInformationClass==ProcessImagePath)
		pus=&ProcessParameters->ImagePathName;
	else
		pus=&ProcessParameters->CommandLine;

	//读取进程的内存，得到字符串长度、缓冲区信息
	UNICODE_STRING us;
	if (!
		ReadMemory(pus,&us,sizeof(us),&ck)
			||
		ck!=sizeof(us)
		)
	{
		return FALSE;
	}
	nLen=us.Length/2;

#ifdef UNICODE
	//检查长度是否超过缓冲区长度
	if (nLen>=(int)nLength){
		assert(0);
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}
	//直接读取缓冲区
	if (!
		ReadMemory(us.Buffer,lpStr,us.Length,&ck)
			||
		ck!=us.Length
		)
	{
		return FALSE;
	}
#else
	//直接读取缓冲区
	if (!
		ReadMemory(us.Buffer,lpBuffer,us.Length,&ck)
			||
		ck!=us.Length
		)
	{
		return FALSE;
	}
	if (WideCharToMultiByte(CP_ACP,0,(LPCWSTR)lpBuffer,nLen,lpStr,(int)nLength,"?",NULL)==0)
		return FALSE;
#endif
	lpStr[nLen]=L'\0';
	return TRUE;
}

BOOL CProcess::GetPathString(LPTSTR lpStr,SIZE_T nLength,PROCESSINFOCLASS ProcessInformationClass) const {
	assert(ProcessInformationClass==ProcessImagePath || ProcessInformationClass==ProcessCommandLine);

	BYTE lpBuffer[32767];ULONG uReturnLength;
	int nLen; //结果字符串长度

	if (!bExistQueryLimitedInformation)
		return GetPathStringByMemory(lpStr, nLength, ProcessInformationClass);

	// 用 NtQueryInformationProcess 取得结果

	NTSTATUS ntstatus=NtQueryInformation(ProcessInformationClass,lpBuffer,32767,&uReturnLength);
	if (!NT_SUCCESS(ntstatus)){
		CProcess Process;
		if (!Process.Open(GetId(), PROCESS_QUERY_INFORMATION | PROCESS_VM_READ))
			return FALSE;
		return Process.GetPathStringByMemory(lpStr, nLength, ProcessInformationClass);
	}

	UNICODE_STRING &us=*(UNICODE_STRING*)lpBuffer;
	nLen=us.Length/2;

#ifdef UNICODE
	if (nLen>=(int)nLength){
		assert(0);
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}
	wcsncpy_s(lpStr,nLength,us.Buffer,nLen);
#else
	if (WideCharToMultiByte(CP_ACP,0,us.Buffer,nLen,lpStr,(int)nLength,"?",NULL)==0)
		return FALSE;
#endif

	lpStr[nLen]=L'\0';
	return TRUE;
}



typedef union _PSAPI_WORKING_SET_BLOCK {
	ULONG_PTR Flags;
	struct {
		ULONG_PTR Protection  :5;
		ULONG_PTR ShareCount  :3;
		ULONG_PTR Shared  :1;
		ULONG_PTR Reserved  :3;
#if defined(_WIN64)
		ULONG_PTR VirtualPage  :52;
#else
		ULONG_PTR VirtualPage  :20;
#endif
	};
} PSAPI_WORKING_SET_BLOCK, *PPSAPI_WORKING_SET_BLOCK;

typedef struct _PSAPI_WORKING_SET_INFORMATION {
	ULONG_PTR               NumberOfEntries;
	PSAPI_WORKING_SET_BLOCK WorkingSetInfo[1];
} PSAPI_WORKING_SET_INFORMATION, *PPSAPI_WORKING_SET_INFORMATION;


BOOL CProcess::GetWorkingSetInfo(SIZE_T *pWSTotal,SIZE_T *pWSPrivate,SIZE_T *pWSShareable,SIZE_T *pWSShared) const {
	BYTE lpBuffer[256*1024];
	PSAPI_WORKING_SET_INFORMATION *pstWSInfo=(PSAPI_WORKING_SET_INFORMATION *)lpBuffer;

	if (!QueryWorkingSet(pstWSInfo,256*1024)){
		DWORD dwLastError=GetLastError();
		SetLastError(dwLastError);
		return FALSE;
	}

	//开始计算
	int n=(int)pstWSInfo->NumberOfEntries;
	int WSPrivate=0,WSShareable=0,WSShared=0;
	for (int i=0;i<n;i++){
		if (!pstWSInfo->WorkingSetInfo[i].Shared){
			WSPrivate++;
			continue;
		}
		WSShareable++;
		if (pstWSInfo->WorkingSetInfo[i].ShareCount>1)
			WSShared++;
	}

	if (pWSTotal) *pWSTotal = n * dwPageSize;
	if (pWSPrivate) *pWSPrivate = WSPrivate * dwPageSize;
	if (pWSShareable) *pWSShareable = WSShareable * dwPageSize;
	if (pWSShared) *pWSShared = WSShared * dwPageSize;
	return TRUE;
}

BOOL CProcess::GetUserName(LPTSTR szUserName,DWORD nSize){
	assert(HaveAccess(PROCESS_QUERY_INFORMATION) || HaveAccess(PROCESS_QUERY_LIMITED_INFORMATION));

	//打开与进程相关联的访问令牌
	HANDLE hToken;
	if (!OpenProcessToken(m_hProcess,TOKEN_QUERY,&hToken))
		return FALSE;

	//获取令牌的用户信息
	BYTE lpBuffer[1024];
	DWORD dwBytesReturn;
	if (!GetTokenInformation(hToken,TokenUser,lpBuffer,1024,&dwBytesReturn)){
		CloseHandle(hToken);
		return FALSE;
	}
	PTOKEN_USER pstTokenUserInfo=(PTOKEN_USER)lpBuffer;
	CloseHandle(hToken);

	//通过 SID 获取用户名
	TCHAR szDomainName[1024];DWORD cchDomainName=1024;SID_NAME_USE stSNU;
	return LookupAccountSid(NULL,pstTokenUserInfo->User.Sid,szUserName,&nSize,szDomainName,&cchDomainName,&stSNU);
}

BOOL CProcess::AquireToken(HANDLE hProcess,LPCTSTR PrivilegeName){
	HANDLE hToken;
	TOKEN_PRIVILEGES stNewToken;
	LUID luidDebugLUID;
	if (!OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken)) return FALSE;
	if (!LookupPrivilegeValue(NULL,PrivilegeName,&luidDebugLUID)) return FALSE;
	stNewToken.PrivilegeCount=1;
	stNewToken.Privileges[0].Luid=luidDebugLUID;
	stNewToken.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken,FALSE,&stNewToken,sizeof(stNewToken),NULL,NULL)){
		CloseHandle(hToken);
		return FALSE;
	}
	CloseHandle(hToken);
	return TRUE;
}



