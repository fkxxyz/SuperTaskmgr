
// WinSCManager.h
#pragma once

#include <Winsvc.h>

class CSCManager : public CHandleObject<SC_HANDLE> {
private:
	SC_HANDLE &m_hSCM;
	friend class CService;

public:
	CSCManager():m_hSCM(m_Handle){}
	~CSCManager(){if (m_hSCM) Close();}

	BOOL Open(DWORD dwDesiredAccess=SC_MANAGER_ALL_ACCESS, LPCTSTR lpMachineName=NULL, LPCTSTR lpDatabaseName=NULL){
		assert(m_hSCM==NULL);
		m_hSCM=OpenSCManager(lpMachineName,lpDatabaseName,dwDesiredAccess);
		return m_hSCM != NULL;
	}
	BOOL Close(){
		assert(m_hSCM!=NULL);
		if (!CloseServiceHandle(m_hSCM)) return FALSE;
		m_hSCM=NULL;
		return TRUE;
	}

	BOOL GetDisplayName(LPCTSTR lpServiceName,LPTSTR lpDisplayName,LPDWORD lpcchBuffer){
		assert(m_hSCM!=NULL);
		return GetServiceDisplayName(m_hSCM,lpServiceName,lpDisplayName,lpcchBuffer);
	}
	BOOL GetKeyName(LPCTSTR lpDisplayName,LPTSTR lpServiceName,LPDWORD lpcchBuffer){
		assert(m_hSCM!=NULL);
		return GetServiceKeyName(m_hSCM,lpDisplayName,lpServiceName,lpcchBuffer);
	}

	BOOL EnumStatus(
		DWORD                 dwServiceType,
		DWORD                 dwServiceState,
		LPENUM_SERVICE_STATUS lpServices,
		DWORD                 cbBufSize,
		LPDWORD               pcbBytesNeeded,
		LPDWORD               lpServicesReturned,
		LPDWORD               lpResumeHandle
		)
	{
		assert(m_hSCM!=NULL);
		return EnumServicesStatus(
			m_hSCM,
			dwServiceType,
			dwServiceState,
			lpServices,
			cbBufSize,
			pcbBytesNeeded,
			lpServicesReturned,
			lpResumeHandle
			);
	}
};

class CService : public CHandleObject<SC_HANDLE>{
	SC_HANDLE &m_hSCM;
	SC_HANDLE &m_hService;

public:
	CService(CSCManager &SCM):m_hSCM(SCM.m_hSCM),m_hService(m_Handle){}
	~CService(){if (m_hService) Close();}

	BOOL Open(LPCTSTR lpServiceName,DWORD dwDesiredAccess=SC_MANAGER_ALL_ACCESS){
		assert(m_hSCM!=NULL);
		assert(m_hService==NULL);
		m_hService=OpenService(m_hSCM,lpServiceName,dwDesiredAccess);
		return m_hService != NULL;
	}
	BOOL Close(){
		assert(m_hService!=NULL);
		if (!CloseServiceHandle(m_hService)) return FALSE;
		m_hService=NULL;
		return TRUE;
	}

	BOOL Create(
		LPCTSTR   lpServiceName,
		LPCTSTR   lpDisplayName,
		LPCTSTR   lpBinaryPathName,
		DWORD     dwServiceType,
		DWORD     dwStartType=SERVICE_DEMAND_START,
		DWORD     dwDesiredAccess=SC_MANAGER_ALL_ACCESS,
		DWORD     dwErrorControl=SERVICE_ERROR_IGNORE,
		LPCTSTR   lpLoadOrderGroup=NULL,
		LPDWORD   lpdwTagId=NULL,
		LPCTSTR   lpDependencies=NULL,
		LPCTSTR   lpServiceStartName=NULL,
		LPCTSTR   lpPassword=NULL
		)
	{
		assert(m_hSCM!=NULL);
		assert(m_hService==NULL);
		m_hService=CreateService(
			m_hSCM,
			lpServiceName,
			lpDisplayName,
			dwDesiredAccess,
			dwServiceType,
			dwStartType,
			dwErrorControl,
			lpBinaryPathName,
			lpLoadOrderGroup,
			lpdwTagId,
			lpDependencies,
			lpServiceStartName,
			lpPassword
			);
		return m_hService != NULL;
	}
	BOOL Delete(){
		assert(m_hService!=NULL);
		return DeleteService(m_hService);
	}

	BOOL Control(DWORD dwControl,LPSERVICE_STATUS lpServiceStatus){
		assert(m_hService!=NULL);
		return ControlService(m_hService,dwControl,lpServiceStatus);
	}

	BOOL Start(DWORD dwNumServiceArgs=0, LPCTSTR *lpServiceArgVectors=NULL){
		assert(m_hService!=NULL);
		return StartService(m_hService,dwNumServiceArgs,lpServiceArgVectors);
	}
	BOOL Stop(){
		SERVICE_STATUS ss;
		return Control(SERVICE_CONTROL_STOP,&ss);
	}

	BOOL Pause(){
		SERVICE_STATUS ss;
		return Control(SERVICE_CONTROL_PAUSE,&ss);
	}
	BOOL Continue(){
		SERVICE_STATUS ss;
		return Control(SERVICE_CONTROL_CONTINUE,&ss);
	}

	BOOL QueryStatus(LPSERVICE_STATUS lpServiceStatus){
		assert(m_hService!=NULL);
		return QueryServiceStatus(m_hService,lpServiceStatus);
	}
	DWORD QueryCurrentStatus(){
		SERVICE_STATUS ss;
		if (!QueryStatus(&ss)) return 0;
		return ss.dwCurrentState;
	}

	BOOL QueryConfig(LPQUERY_SERVICE_CONFIG lpServiceConfig,DWORD cbBufSize,LPDWORD pcbBytesNeeded){
		assert(m_hService!=NULL);
		return QueryServiceConfig(m_hService,lpServiceConfig,cbBufSize,pcbBytesNeeded);
	}
	BOOL ChangeConfig(
		LPCTSTR   lpDisplayName=NULL,
		DWORD     dwServiceType=SERVICE_NO_CHANGE,
		LPCTSTR   lpBinaryPathName=NULL,
		DWORD     dwStartType=SERVICE_NO_CHANGE,
		DWORD     dwErrorControl=SERVICE_NO_CHANGE,
		LPCTSTR   lpLoadOrderGroup=NULL,
		LPDWORD   lpdwTagId=NULL,
		LPCTSTR   lpDependencies=NULL,
		LPCTSTR   lpServiceStartName=NULL,
		LPCTSTR   lpPassword=NULL
		)
	{
		assert(m_hService!=NULL);
		return ChangeServiceConfig(
			m_hService,
			dwServiceType,
			dwStartType,
			dwErrorControl,
			lpBinaryPathName,
			lpLoadOrderGroup,
			lpdwTagId,
			lpDependencies,
			lpServiceStartName,
			lpPassword,
			lpDisplayName
			);
	}
};


