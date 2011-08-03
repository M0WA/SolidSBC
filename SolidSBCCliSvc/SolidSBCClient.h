#pragma once

#include "Resource.h"
#include "uuid_gen.h"

#include "SolidSBCResultManager.h"

#include "SolidSBCCliConfigSocket.h"
#include "SolidSBCCliResultSocket.h"

class CSolidSBCClient
{
public:
	CSolidSBCClient(void);
	~CSolidSBCClient(void);

	int StartTests(void);
	void StopTests(bool bUnloadLibraries = false);

	//used internally
public:	
	int Run( CString strDataSource, DWORD dwSrvConfPort, DWORD dwSrvDataPort );
	int Stop(bool bLog = true);

	int StartResultConnection(SOCKADDR_IN target);
	int SendTestResult(CSolidSBCTestResult* pResult);

private:
	int  Init( void );
	bool InitTests(void);
	BOOL GetClientUUID(void);

	CMutex                                                   m_lockTestLibs;
	std::vector< std::pair<HMODULE, CSolidSBCTestManager*> > m_vecTestLibs;
	CSolidSBCResultManager                                   m_ResultManager;

	BOOL					m_bIsInitialized;
	CString					m_strDataSource;
	DWORD					m_dwSrvConfPort;
	DWORD					m_dwSrvDataPort;
	char*					m_pszUUID;
	CSolidSBCCliConfigSocket m_cCliConfigSocket;
	CSolidSBCCliResultSocket m_cCliResultSocket;
};
