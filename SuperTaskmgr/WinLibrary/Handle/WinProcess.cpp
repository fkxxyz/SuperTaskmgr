
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

//ͨ����ȡ�����ڴ�ȡ�ý��
BOOL CProcess::GetPathStringByMemory(LPTSTR lpStr,SIZE_T nLength,PROCESSINFOCLASS ProcessInformationClass) const {
	int nLen; //����ַ�������

	//��ȡ���̵Ļ�����Ϣ�����еõ����� peb �ĵ�ַ��
	NTSTATUS ntstatus;
	PROCESS_BASIC_INFORMATION pbi;
	ntstatus=NtQueryBasicInformation(&pbi);
	if (!NT_SUCCESS(ntstatus)){
		SetLastError(RtlNtStatusToDosError(ntstatus));
		return FALSE;
	}

	//��ȡ���̵��ڴ棬�õ� ProcessParameters �ṹ��ĵ�ַ
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

	//��ȡ���̵��ڴ棬�õ��ַ������ȡ���������Ϣ
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
	//��鳤���Ƿ񳬹�����������
	if (nLen>=(int)nLength){
		assert(0);
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}
	//ֱ�Ӷ�ȡ������
	if (!
		ReadMemory(us.Buffer,lpStr,us.Length,&ck)
			||
		ck!=us.Length
		)
	{
		return FALSE;
	}
#else
	//ֱ�Ӷ�ȡ������
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
	int nLen; //����ַ�������

	if (!bExistQueryLimitedInformation)
		return GetPathStringByMemory(lpStr, nLength, ProcessInformationClass);

	// �� NtQueryInformationProcess ȡ�ý��

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

	//��ʼ����
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

	//�������������ķ�������
	HANDLE hToken;
	if (!OpenProcessToken(m_hProcess,TOKEN_QUERY,&hToken))
		return FALSE;

	//��ȡ���Ƶ��û���Ϣ
	BYTE lpBuffer[1024];
	DWORD dwBytesReturn;
	if (!GetTokenInformation(hToken,TokenUser,lpBuffer,1024,&dwBytesReturn)){
		CloseHandle(hToken);
		return FALSE;
	}
	PTOKEN_USER pstTokenUserInfo=(PTOKEN_USER)lpBuffer;
	CloseHandle(hToken);

	//ͨ�� SID ��ȡ�û���
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



