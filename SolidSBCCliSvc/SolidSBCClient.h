#pragma once

#include "Resource.h"
#include "uuid_gen.h"
#include "SolidSBCCliConfigSocket.h"
#include "SolidSBCCliResultSocket.h"

class CSolidSBCClient
{
public:
	CSolidSBCClient(void);
	~CSolidSBCClient(void);
	int Run( CString strDataSource, DWORD dwSrvConfPort, DWORD dwSrvDataPort, UINT nProfileID );
	int Stop(bool bLog = true);
	int StartResultConnection(SOCKADDR_IN target);
	int SendTestResult(PSSBC_BASE_PACKET_HEADER pPacket);
	int StartTestFromProfilePacket(PSSBC_PROFILE_REPLY_PACKET pPacket);
	void StopTests(void);

private:
	int Init( void );
	int InitServerConfig(void);
	BOOL GetClientUUID(void);
	void DumpProfileReplyPacket(PSSBC_PROFILE_REPLY_PACKET pPacket);

	BOOL					m_bIsInitialized;
	CString					m_strDataSource;
	UINT					m_nProfileID;
	DWORD					m_dwSrvConfPort;
	DWORD					m_dwSrvDataPort;
	char*					m_pszUUID;
	CSolidSBCCliConfigSocket m_cCliConfigSocket;
	CSolidSBCCliResultSocket m_cCliResultSocket;
};
