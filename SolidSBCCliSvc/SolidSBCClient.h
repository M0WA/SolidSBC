#pragma once

#include "Resource.h"
#include "uuid_gen.h"
#include "SolidSBCTestNetwork.h"
#include "SolidSBCTestMemory.h"
#include "SolidSBCTestHarddrive.h"
#include "SolidSBCTestCPU.h"
#include "SolidSBCTestCPUMeasure.h"
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
	CVMPerfTestNetwork		m_cVMPerfTestNetwork;
	CVMPerfTestMemory		m_cVMPerfTestMemory;
	CVMPerfTestHarddrive	m_cVMPerfTestHarddrive;
	CVMPerfTestCPU			m_cVMPerfTestCPU;
	CVMPerfTestCPUMeasure   m_cVMPerfTestCPUMeasure;
};
