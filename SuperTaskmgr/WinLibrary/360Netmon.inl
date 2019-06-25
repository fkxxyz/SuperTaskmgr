
// 360Netmon.inl


inline BOOL C360NetmonInstaller::FindSupportDriver(const DWORD *&drv,size_t &size){
	BOOL &bIs64BitSystem=CNtDriverService::bIs64BitSystem;
	DWORD &dwVersionValue=CNtDriverService::dwVersionValue;

	if (bIs64BitSystem){
		if (dwVersionValue>=0x00060002){  //  64 位至少 Win8
			drv=drv_x64_wfp;
			size=sizeof(drv_x64_wfp);
		} else if (dwVersionValue==0x00060001){  // 64 位的 Win7
			drv=drv_x64;
			size=sizeof(drv_x64);
		} else {  // 64 位的 Win7 以下系统不支持
			return FALSE;
		}
	} else {
#ifndef _WIN64
		if (dwVersionValue>=0x00060002){  //  32 位至少 Win8
			drv=drv_wfp;
			size=sizeof(drv_wfp);
		} else if (dwVersionValue==0x00060001){  // 32 位的 Win7
			drv=drv_60;
			size=sizeof(drv_60);
		} else {  // 32 位的 Win7 以下系统
			drv=drv_50;
			size=sizeof(drv_50);
		}
#endif
	}
	return TRUE;
}

inline BOOL C360NetmonInstaller::Install(){
	return InstallDriverImage(
		_T("360netmon.sys"),
		_T("360netmon"),
		_T("360netmon"),
		SERVICE_SYSTEM_START,
		SERVICE_ERROR_NORMAL,
		_T("PNP_TDI"),
		1,
		dwDrvBuildNumber
		);
}

inline BOOL C360NetmonInstaller::Uninstall(){
	{
		C360TdiFilter TdiFilter;
		if (TdiFilter.Open()){
			TdiFilter.Stop();
		}
	}
	return DriverService.Uninstall(_T("360netmon"));
}


inline BOOL C360NetmonInstaller::StartNetmon(){
	return CNtDriverService::Start(_T("360netmon"));
}

inline BOOL C360NetmonInstaller::StopNetmon(){
	return CNtDriverService::Stop(_T("360netmon"));
}



inline BOOL C360TdiFilter::Open(){
	return CDevice::Open(_T("\\\\.\\360TdiFilter"));
}
inline BOOL C360TdiFilter::GetVersion(PDEVICE_VERSION pVersion){
	return IoRecv(IOCTL_360TDIFILTER_GET_VERSION,pVersion,sizeof(*pVersion));
}
inline BOOL C360TdiFilter::GetProcessInitFlow(PPROCESS_INIT_FLOW pInitProcessFlow){
	DWORD nByteReturned;
	if (!IoControl(
		IOCTL_360TDIFILTER_GET_PROCESS_INIT_FLOW,
		pInitProcessFlow->flow,
		sizeof(pInitProcessFlow->flow),
		pInitProcessFlow->flow,
		sizeof(pInitProcessFlow->flow),
		&nByteReturned
		))
		return FALSE;
	assert(nByteReturned % sizeof(pInitProcessFlow->flow[0]) == 0);
	pInitProcessFlow->nProcess=nByteReturned/sizeof(pInitProcessFlow->flow[0]);
	return TRUE;
}
inline BOOL C360TdiFilter::Start(){
	return IoCode(IOCTL_360TDIFILTER_START);
}
inline BOOL C360TdiFilter::Stop(){
	return IoCode(IOCTL_360TDIFILTER_STOP);
}
inline BOOL C360TdiFilter::GetStatus(LPBOOL pRunning){
	return IoRecvDWORD(IOCTL_360TDIFILTER_GET_STATUS,(LPDWORD)pRunning);
}
inline BOOL C360TdiFilter::GetGlobalFlow(PGLOBAL_FLOW pGlobalFlow){
	return IoRecv(IOCTL_360TDIFILTER_GET_GLOBAL_FLOW,pGlobalFlow,sizeof(*pGlobalFlow));
}
inline BOOL C360TdiFilter::GetProcessFlow(PPROCESS_FLOW pProcessFlow){
	DWORD nByteReturned;
	if (!IoControl(
		IOCTL_360TDIFILTER_GET_PROCESS_FLOW,
		pProcessFlow->flow,
		sizeof(pProcessFlow->flow),
		pProcessFlow->flow,
		sizeof(pProcessFlow->flow),
		&nByteReturned
		))
		return FALSE;
	assert(nByteReturned % sizeof(pProcessFlow->flow[0]) == 0);
	pProcessFlow->nProcess=nByteReturned/sizeof(pProcessFlow->flow[0]);
	return TRUE;
}

inline BOOL C360TdiFilter::LimitProcessSpeed(LPCTSTR szImagePath,LONGLONG nLimitSend,LONGLONG nLimitRecv){
	//检查文件路径合理性
	if (!ExistFile(szImagePath)){
		SetLastError(ERROR_FILE_NOT_FOUND);
		return FALSE;
	}
	assert(szImagePath[1]==_T(':') && szImagePath[2]==_T('\\'));

	//初始化结构体
	LIMIT_SPEED ls;
	memset(&ls,0,sizeof(ls));

	//复制文件的路径
#ifdef UNICODE
	_tcscpy_s<MAX_PATH>(ls.szPath,szImagePath);
#else
	if (MultiByteToWideChar(
		CP_ACP,0,
		szImagePath,(int)strlen(szImagePath)+1,
		ls.szPath,sizeof(ls.szPath)
		) == 0)
		return FALSE;
#endif

	//计算出文件Nt路径
	WCHAR *pPath, *pNtPath;
	pPath=ls.szPath+2; pNtPath=ls.szNtPath;
	while (*pPath){
		if (*pPath>=L'a' && *pPath<=L'z')
			*pNtPath=*pPath-(L'a'-L'A');
		else
			*pNtPath=*pPath;
		pPath++;pNtPath++;
	}
	*pNtPath=L'\0';

	//设置限制字节信息
	if (nLimitSend==LLONG_MAX && nLimitRecv==LLONG_MAX){
		ls.bCancelFlag=TRUE;
	}
	if (nLimitSend==0l && nLimitRecv==0l){
		ls.qwBlockCnnt=LLONG_MAX;
		nLimitSend=LLONG_MAX;
		nLimitRecv=LLONG_MAX;
	}
	ls.nLimitSend = nLimitSend;
	ls.nLimitRecv = nLimitRecv;

	return IoSend(IOCTL_360TDIFILTER_LIMIT_PROCESS_SPEED,&ls,sizeof(ls));
}
inline BOOL C360TdiFilter::DisableProcessNetwork(LPCTSTR szImagePath){
	return LimitProcessSpeed(szImagePath,0l,0l);
}
inline BOOL C360TdiFilter::EnableProcessNetwork(LPCTSTR szImagePath){
	return LimitProcessSpeed(szImagePath,LLONG_MAX,LLONG_MAX);
}

inline BOOL C360TdiFilter::GetProcessLimited(PPROCESS_LIMITED pProcessLimited){
	DWORD nByteReturned;
	if (!IoRecv(
		IOCTL_360TDIFILTER_GET_PROCESS_LIMITED,
		pProcessLimited->limit,
		sizeof(pProcessLimited->limit),
		&nByteReturned
		))
		return FALSE;
	assert(nByteReturned % sizeof(pProcessLimited->limit[0]) == 0);
	pProcessLimited->nProcess=nByteReturned/sizeof(pProcessLimited->limit[0]);
	return TRUE;
}


