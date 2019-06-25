
#pragma once

typedef enum {
	cProcessName, //��������
	cDescription, //��������
	cImagePath, //�����ļ�·��
	cCommandLine, //����������
	cCPU, //CPUռ����
	cIOSpeed, //IO��д����
	cPrivateWorkingSet, //ר�ù�����

	cNetRecvSpeed, //������������
	cNetSendSpeed, //�����ϴ�����
	cNetTotalSpeed, //���紫������
	cNetRecvFlow, //������������
	cNetSendFlow, //�����ϴ�����
	cNetTotalFlow, //���紫��������

	cUserName, //�û���
	cIsWow64Process, //�Ƿ���Windows on Windows64 ���������� ���Ƿ���32λ������64λϵͳ�����У�
	cSuspended, //�߳��Ƿ��ѹ���
	cNumberOfThreads, //�߳�����
	cCreateTime,  //����ʱ��
	cCPUTime,  //CPUʱ��
	cPriority, //����Ȩ��
	cPID, //����PID
	cInheritedFromProcessId, //������PID
	cHandleCount, //�������
	cSessionId, //�ỰID

	cPeakVirtualSize, //�����ڴ��ֵ
	cVirtualSize, //�����ڴ�
	cPageFaultCount, //ҳ�����
	cPeakWorkingSetSize, //�ڴ��ֵ
	cWorkingSetSize, //�ڴ�
	cQuotaPeakPagedPoolUsage, //ҳ�滺��ط�ֵ
	cQuotaPagedPoolUsage, //ҳ�滺���
	cQuotaPeakNonPagedPoolUsage, //��ҳ�滺��ط�ֵ
	cQuotaNonPagedPoolUsage, //��ҳ�滺���
	cPageFileUsage, //ҳ���ļ�
	cPeakPageFileUsage, //ҳ���ļ���ֵ
	cPrivateUsage, //�����ڴ�(���������)

	cReadOperationCount, //I/O��ȡ
	cWriteOperationCount, //I/Oд��
	cOtherOperationCount, //I/O����
	cReadTransferCount, //I/O��ȡ�ֽ�
	cWriteTransferCount, //I/Oд���ֽ�
	cOtherTransferCount, //I/O�����ֽ�

	cENUM_COLUMN_END //ö�ٽ�����־
} ENUM_COLUMN;

typedef struct {
	BOOL isCurrentProcess; //�Ƿ��Ǳ����̱���
	BOOL inWhite;//�Ƿ�Ϊ����������
	BOOL inBlack;//�Ƿ�Ϊ����������
	BOOL inSystem;//�Ƿ�Ϊϵͳ����
	BOOL Selected;//�Ƿ����б��б�ѡ��
	BOOL Checked;//�Ƿ����б��б���ѡ
	CProcess Process; //�ܹ����ʽ�����Ϣ�Ľ���α���

	LPCTSTR szDispName; //��ʾ�Ľ�������
	LPCTSTR szPlatform; //��ʾ��ƽ̨
	LPCTSTR szPID; //��ʾ�Ľ��� ID
	LPCTSTR szParentID; //��ʾ�ĸ����� ID
	LPCTSTR szSessionId; //��ʾ�ĻỰ ID

	LPCTSTR name; //��������
	LPCTSTR path; //�����ļ�·��
	LPCTSTR cmdline; //����������
	HICON hIcon; //�����ļ�ͼ��
	int nImage; //ͼ���б��е����
	LPCTSTR FileDescription;//�����ļ�����
	FLOAT CPU; //CPUռ����
	LONGLONG NetTotalSpeed; //ռ������
	LONGLONG NetSendSpeed; //�ϴ��ٶ�
	LONGLONG NetRecvSpeed; //�����ٶ�
	LONGLONG IOSpeed; //IO��д�ٶ�
	LONGLONG IOReadSpeed; //IO��ȡ�ٶ�
	LONGLONG IOWriteSpeed; //IOд���ٶ�
	SIZE_T PrivateWorkingSet; //�ڴ棨ר�ù�������

	LONGLONG NetSendFlow; //�ѷ��͵�����
	LONGLONG NetRecvFlow; //�ѽ��ܵ�����
	LONGLONG NetTotalFlow; //��������

	BOOL bDisabledNetwork; //�Ƿ��ѽ�ֹ����

	BOOL isWow64Process; //�Ƿ���32λ��ϵͳ
	LPCTSTR szUserName; //�û���
	BOOL isSuspended; //�Ƿ��ѹ���
	ULONG NumberOfThreads; //�߳�����
	LONGLONG CreateTime;  //����ʱ��
	LONGLONG CPUTime;  //CPUʱ��
	DWORD Priority; //����Ȩ��
	DWORD PID; //����PID
	DWORD pPID; //������PID
	ULONG HandleCount; //�������
	ULONG SessionId; //�ỰID

	SIZE_T PeakVirtualSize; //�����ڴ��ֵ
	SIZE_T VirtualSize; //�����ڴ�
	ULONG PageFaultCount; //ҳ�����
	SIZE_T PeakWorkingSetSize; //�ڴ��ֵ
	SIZE_T WorkingSetSize; //�ڴ�
	SIZE_T QuotaPeakPagedPoolUsage; //ҳ�滺��ط�ֵ
	SIZE_T QuotaPagedPoolUsage; //ҳ�滺���
	SIZE_T QuotaPeakNonPagedPoolUsage; //��ҳ�滺��ط�ֵ
	SIZE_T QuotaNonPagedPoolUsage; //��ҳ�滺���
	SIZE_T PageFileUsage; //ҳ���ļ�
	SIZE_T PeakPageFileUsage; //ҳ���ļ���ֵ
	SIZE_T PrivateUsage; //�����ڴ�(���������)

	LONGLONG ReadOperationCount; //I/O��ȡ
	LONGLONG WriteOperationCount; //I/Oд��
	LONGLONG OtherOperationCount; //I/O����
	LONGLONG ReadTransferCount; //I/O��ȡ�ֽ�
	LONGLONG WriteTransferCount; //I/Oд���ֽ�
	LONGLONG OtherTransferCount; //I/O�����ֽ�
} PROCESS, *LPPROCESS;




extern LPCTSTR Pid0Name;
