#pragma once

#include <Winsvc.h>
#include "SolidSBCClient.h"

class CSolidSBCClientService
{
public:
	CSolidSBCClientService(void);
	~CSolidSBCClientService(void);
	
	int  InstallService(void);
	int  DeleteService(void);
	bool IsServiceInstalled(void);

	int StartClientService(void);
	int StopClientService(void);

	int StartResultConnection(void);
	int StartTestFromProfilePacket(PSSBC_PROFILE_REPLY_PACKET pPacket);
	int ChangeProfile(PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET pPacket);
	void SendTestResult( PSSBC_BASE_PACKET_HEADER pPacket );
	void ConnectionClosed(void);

	void SetImagePath(CString strImagePath){g_strImagePath = strImagePath;};
	BOOL LoadParameters(void);
	void SaveParameters(
	  BOOL bAutoStart
	, CString strDataSource
	, DWORD   dwSrvConfPort
	, DWORD   dwSrvDataPort
	, UINT nProfileID
	, BOOL bAutoReconnect);

private:
	CString GetSettingsFileName(void);

	SSBC_CLISVC_PARAM       m_CliSvcParam;	
	CSolidSBCClient			m_cSolidSBCClient;
	CMutex					m_cSolidSBCClientMutex;
	int                     m_nReconnectCounter;
};

extern CSolidSBCClientService g_cClientService;

