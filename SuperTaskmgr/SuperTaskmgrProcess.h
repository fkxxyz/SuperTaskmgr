
#pragma once

typedef enum {
	cProcessName, //进程名称
	cDescription, //进程描述
	cImagePath, //进程文件路径
	cCommandLine, //进程命令行
	cCPU, //CPU占用率
	cIOSpeed, //IO读写速率
	cPrivateWorkingSet, //专用工作集

	cNetRecvSpeed, //网络下载速率
	cNetSendSpeed, //网络上传速率
	cNetTotalSpeed, //网络传输速率
	cNetRecvFlow, //网络下载流量
	cNetSendFlow, //网络上传流量
	cNetTotalFlow, //网络传输总流量

	cUserName, //用户名
	cIsWow64Process, //是否在Windows on Windows64 环境下运行 （是否是32位进程在64位系统下运行）
	cSuspended, //线程是否已挂起
	cNumberOfThreads, //线程数量
	cCreateTime,  //创建时间
	cCPUTime,  //CPU时间
	cPriority, //优先权限
	cPID, //进程PID
	cInheritedFromProcessId, //父进程PID
	cHandleCount, //句柄数量
	cSessionId, //会话ID

	cPeakVirtualSize, //虚拟内存峰值
	cVirtualSize, //虚拟内存
	cPageFaultCount, //页面错误
	cPeakWorkingSetSize, //内存峰值
	cWorkingSetSize, //内存
	cQuotaPeakPagedPoolUsage, //页面缓冲池峰值
	cQuotaPagedPoolUsage, //页面缓冲池
	cQuotaPeakNonPagedPoolUsage, //非页面缓冲池峰值
	cQuotaNonPagedPoolUsage, //非页面缓冲池
	cPageFileUsage, //页面文件
	cPeakPageFileUsage, //页面文件峰值
	cPrivateUsage, //虚拟内存(任务管理器)

	cReadOperationCount, //I/O读取
	cWriteOperationCount, //I/O写入
	cOtherOperationCount, //I/O其他
	cReadTransferCount, //I/O读取字节
	cWriteTransferCount, //I/O写入字节
	cOtherTransferCount, //I/O其他字节

	cENUM_COLUMN_END //枚举结束标志
} ENUM_COLUMN;

typedef struct {
	BOOL isCurrentProcess; //是否是本进程本身
	BOOL inWhite;//是否为白名单进程
	BOOL inBlack;//是否为黑名单进程
	BOOL inSystem;//是否为系统进程
	BOOL Selected;//是否在列表中被选中
	BOOL Checked;//是否在列表中被复选
	CProcess Process; //能够访问进程信息的进程伪句柄

	LPCTSTR szDispName; //显示的进程名称
	LPCTSTR szPlatform; //显示的平台
	LPCTSTR szPID; //显示的进程 ID
	LPCTSTR szParentID; //显示的父进程 ID
	LPCTSTR szSessionId; //显示的会话 ID

	LPCTSTR name; //进程名称
	LPCTSTR path; //进程文件路径
	LPCTSTR cmdline; //进程命令行
	HICON hIcon; //进程文件图标
	int nImage; //图标列表中的序号
	LPCTSTR FileDescription;//进程文件描述
	FLOAT CPU; //CPU占用率
	LONGLONG NetTotalSpeed; //占用网速
	LONGLONG NetSendSpeed; //上传速度
	LONGLONG NetRecvSpeed; //下载速度
	LONGLONG IOSpeed; //IO读写速度
	LONGLONG IOReadSpeed; //IO读取速度
	LONGLONG IOWriteSpeed; //IO写入速度
	SIZE_T PrivateWorkingSet; //内存（专用工作集）

	LONGLONG NetSendFlow; //已发送的流量
	LONGLONG NetRecvFlow; //已接受的流量
	LONGLONG NetTotalFlow; //已用流量

	BOOL bDisabledNetwork; //是否已禁止联网

	BOOL isWow64Process; //是否是32位子系统
	LPCTSTR szUserName; //用户名
	BOOL isSuspended; //是否已挂起
	ULONG NumberOfThreads; //线程数量
	LONGLONG CreateTime;  //创建时间
	LONGLONG CPUTime;  //CPU时间
	DWORD Priority; //优先权限
	DWORD PID; //进程PID
	DWORD pPID; //父进程PID
	ULONG HandleCount; //句柄数量
	ULONG SessionId; //会话ID

	SIZE_T PeakVirtualSize; //虚拟内存峰值
	SIZE_T VirtualSize; //虚拟内存
	ULONG PageFaultCount; //页面错误
	SIZE_T PeakWorkingSetSize; //内存峰值
	SIZE_T WorkingSetSize; //内存
	SIZE_T QuotaPeakPagedPoolUsage; //页面缓冲池峰值
	SIZE_T QuotaPagedPoolUsage; //页面缓冲池
	SIZE_T QuotaPeakNonPagedPoolUsage; //非页面缓冲池峰值
	SIZE_T QuotaNonPagedPoolUsage; //非页面缓冲池
	SIZE_T PageFileUsage; //页面文件
	SIZE_T PeakPageFileUsage; //页面文件峰值
	SIZE_T PrivateUsage; //虚拟内存(任务管理器)

	LONGLONG ReadOperationCount; //I/O读取
	LONGLONG WriteOperationCount; //I/O写入
	LONGLONG OtherOperationCount; //I/O其他
	LONGLONG ReadTransferCount; //I/O读取字节
	LONGLONG WriteTransferCount; //I/O写入字节
	LONGLONG OtherTransferCount; //I/O其他字节
} PROCESS, *LPPROCESS;




extern LPCTSTR Pid0Name;
