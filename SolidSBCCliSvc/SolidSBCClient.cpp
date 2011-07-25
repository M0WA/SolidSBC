#include "StdAfx.h"
#include "SolidSBCClient.h"

#include <string>

CSolidSBCClient::CSolidSBCClient(void)
: m_bIsInitialized(FALSE)
, m_strDataSource (_T(""))
, m_pszUUID(NULL)
, m_nProfileID(0)
{
}

CSolidSBCClient::~CSolidSBCClient(void)
{
	Stop(false);
	delete [] m_pszUUID;
	m_pszUUID = NULL;
}

int CSolidSBCClient::Run( CString strDataSource, DWORD dwSrvConfPort, DWORD dwSrvDataPort, UINT nProfileID )
{
	m_nProfileID  = nProfileID;
	m_dwSrvConfPort = dwSrvConfPort;
	m_dwSrvDataPort = dwSrvDataPort;

	if ( m_bIsInitialized )
		Stop();

	m_strDataSource = strDataSource;

	int nInit = Init();
	{
		CString strMsg;
		strMsg.Format(_T("DataSource = \"%s\", ProfileID = %d, SrvConfPort = %d, SrvDataPort = %d")
			,strDataSource,nProfileID,dwSrvConfPort,dwSrvDataPort);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
	}
	return nInit;
}

int CSolidSBCClient::Stop(bool bLog)
{
	m_bIsInitialized = FALSE;

	StopTests();
	m_cCliConfigSocket.Close();
	m_cCliResultSocket.Close();

	delete [] m_pszUUID;
	m_pszUUID = NULL;

	return 0;
}

int CSolidSBCClient::Init( void )
{
	m_bIsInitialized = FALSE;

	if ( !GetClientUUID() )
		return 1;

	int nResult = InitServerConfig();

	if ( !nResult ){ m_bIsInitialized = TRUE;   }
	else           { m_bIsInitialized = FALSE; 
					 m_strDataSource  = _T(""); }

	return nResult;
}

int CSolidSBCClient::InitServerConfig(void)
{
	USES_CONVERSION;
	SOCKADDR_IN target;
	target.sin_family = AF_INET;
	target.sin_port   = htons(static_cast<USHORT>(m_dwSrvConfPort));
	target.sin_addr.s_addr = inet_addr(T2A(m_strDataSource));

	m_cCliConfigSocket.SetProfileID(m_nProfileID);
	m_cCliConfigSocket.Connect( target );
	return true;
}

int CSolidSBCClient::StartResultConnection(SOCKADDR_IN target)
{
	{
		USES_CONVERSION;
		CString strMsg;
		strMsg.Format(_T("CSolidSBCClient::StartResultConnection() m_nProfileID = %d, m_pszUUID = %s"),m_nProfileID,A2T(m_pszUUID));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	m_cCliResultSocket.SetProfileID(m_nProfileID);
	m_cCliResultSocket.SetClientUUID(m_pszUUID);
	m_cCliResultSocket.Connect(target);

	return 0;
}

int CSolidSBCClient::SendTestResult(PSSBC_BASE_PACKET_HEADER pPacket)
{
	int nReturn = 0;
	if ( !m_bIsInitialized )
		nReturn = 1;
	
	nReturn = m_cCliResultSocket.SendPacket(pPacket);

	PBYTE orgPacket = (PBYTE)pPacket;
	delete [] orgPacket;
	pPacket = 0;
	return nReturn;
}

BOOL CSolidSBCClient::GetClientUUID(void)
{
	BOOL bReturn = TRUE;
	char** ppszUUID = &m_pszUUID;
	(*ppszUUID) = NULL;

	//try to open (ro) client uuid file
	FILE* pFile = NULL;
	errno_t err = fopen_s( &pFile, SSBC_CLIENT_UUID_FILENAMEPATH,"r");
	if ( !err && (pFile != NULL) ){
		// obtain file size:
		fseek (pFile , 0 , SEEK_END);
		long lSize = ftell (pFile);
		rewind (pFile);

		//read uuid from file
		int nBufferSize = lSize+1;
		(*ppszUUID) = new char[nBufferSize];
		memset((*ppszUUID),0x00,nBufferSize);
		if ( fread((*ppszUUID),1,lSize,pFile) < (size_t)lSize ){
			delete [] (*ppszUUID);
			(*ppszUUID) = NULL;
			bReturn = FALSE;
		}

		//close file
		fclose(pFile);
	} else if ( (!fopen_s(&pFile,SSBC_CLIENT_UUID_FILENAMEPATH,"w")) && pFile ){

		//create uuid and write it to file
		(*ppszUUID) = CUuidGen::CreateUUID();
		if ( fwrite ( (*ppszUUID), 1, strlen( (*ppszUUID) ), pFile ) != strlen( (*ppszUUID) ) ){
			delete [] (*ppszUUID);
			(*ppszUUID) = NULL;
			bReturn = FALSE;
		} 
		//close file
		fclose(pFile);
	} 
	
	if ( (*ppszUUID) == NULL) {
		//some error occured, maybe insufficiant rights
		{
			USES_CONVERSION;
			CString strMsg;
			strMsg.Format( _T("GetClientUUID: Error while generating/restoring Client UUID") );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		bReturn = FALSE;
	} else {
		{
			USES_CONVERSION;
			CString strMsg;
			strMsg.Format(_T("Client UUID is: %s ."),A2T(*ppszUUID) );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}
	}

	return bReturn;
}

void CSolidSBCClient::DumpProfileReplyPacket(PSSBC_PROFILE_REPLY_PACKET pPacket)
{
		{
			USES_CONVERSION;
			CString strMsg;
			strMsg.Format(
			_T( "Dumping profile reply packet:\n\n")
			_T( "\t\tnProfileID:    %d\n")
			_T( "\t\tszProfileName: %s\n")

			_T( "\t\tbHDCfgEnable:            %d\n")
			_T( "\t\tbHDCfgRandomRead:        %d\n")
			_T( "\t\tbHDCfgRandomWrite:       %d\n")
			_T( "\t\tnHDCfgMaxRead:           %d\n")
			_T( "\t\tnHDCfgMaxWrite:          %d\n")
			_T( "\t\tnHDCfgReadWriteDelay:    %d\n")
			_T( "\t\tbHDCfgTransmitData:      %d\n")

			_T( "\t\tbCpuCfgEnable:       %d\n")
			_T( "\t\tbCpuCfgRandom:       %d\n")
			_T( "\t\tnCpuCfgMaxRand:      %d\n")
			_T( "\t\tfCpuCfgSleepMS:      %f\n")
			_T( "\t\tnCpuCfgThreadCnt:    %d\n")

			_T( "\t\tbCpuMeasureCfgEnable:                     %d\n")
			_T( "\t\tnCpuMeasureCfgAdditionCount:              %d\n")
			_T( "\t\tnCpuMeasureCfgDivisionCount:              %d\n")
			_T( "\t\tnCpuMeasureCfgTransmitData:               %d\n")
			_T( "\t\tnCpuMeasureCfgAutoFixMultipliers:         %d\n")
			_T( "\t\tdCpuMeasureCfgFixMultipliersThreshold:    %f\n")

			_T( "\t\tbMemCfgEnable:          %d\n")
			_T( "\t\tbMemCfgRandomize:       %d\n")
			_T( "\t\tnMemCfgMinMem:          %d\n")
			_T( "\t\tnMemCfgMaxMem:          %d\n")
			_T( "\t\tbMemCfgTransmitData:    %d\n")

			_T( "\t\tbNetCfgEnablePingTest:       %d\n")
			_T( "\t\tbNetCfgEnableTcpConnTest:    %d\n")
			_T( "\t\tnNetCfgPingInterval:         %d\n")
			_T( "\t\tszNetCfgPingHost:            %s\n")
			_T( "\t\tnNetCfgPingTTL:              %d\n")
			_T( "\t\tnNetCfgPingPayloadSize:      %d\n")
			_T( "\t\tbNetCfgPingTransmitData:     %d\n")

			_T( "\t\tnNetCfgTcpConnInterval:        %d\n")
			_T( "\t\tszNetCfgTcpConnHost:           %s\n")
			_T( "\t\tnNetCfgTcpConnPort:            %d\n")
			_T( "\t\tnNetCfgTcpConnTTL:             %d\n")
			_T( "\t\tbNetCfgTcpConnTransmitData:    %d\n")

			, pPacket->nProfileID
			, pPacket->szProfileName

			, pPacket->bHDCfgEnable
			, pPacket->bHDCfgRandomRead
			, pPacket->bHDCfgRandomWrite
			, pPacket->nHDCfgMaxRead
			, pPacket->nHDCfgMaxWrite
			, pPacket->nHDCfgReadWriteDelay
			, pPacket->bHDCfgTransmitData

			, pPacket->bCpuCfgEnable
			, pPacket->bCpuCfgRandom
			, pPacket->nCpuCfgMaxRand
			, pPacket->fCpuCfgSleepMS
			, pPacket->nCpuCfgThreadCnt

			, pPacket->bCpuMeasureCfgEnable
			, pPacket->nCpuMeasureCfgAdditionCount
			, pPacket->nCpuMeasureCfgDivisionCount
			, pPacket->nCpuMeasureCfgTransmitData
			, pPacket->nCpuMeasureCfgAutoFixMultipliers
			, pPacket->dCpuMeasureCfgFixMultipliersThreshold

			, pPacket->bMemCfgEnable
			, pPacket->bMemCfgRandomize
			, pPacket->nMemCfgMinMem
			, pPacket->nMemCfgMaxMem
			, pPacket->bMemCfgTransmitData

			, pPacket->bNetCfgEnablePingTest
			, pPacket->bNetCfgEnableTcpConnTest
			, pPacket->nNetCfgPingInterval
			, pPacket->szNetCfgPingHost
			, pPacket->nNetCfgPingTTL
			, pPacket->nNetCfgPingPayloadSize
			, pPacket->bNetCfgPingTransmitData

			, pPacket->nNetCfgTcpConnInterval
			, pPacket->szNetCfgTcpConnHost
			, pPacket->nNetCfgTcpConnPort
			, pPacket->nNetCfgTcpConnTTL
			, pPacket->bNetCfgTcpConnTransmitData
			);

			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}
}

int CSolidSBCClient::StartTestFromProfilePacket(PSSBC_PROFILE_REPLY_PACKET pPacket)
{
	DumpProfileReplyPacket(pPacket);
	/*
	HWND hMsgWnd = NULL;
	if ( pPacket->bHDCfgEnable  ){
		
		{
			CString strMsg;
			strMsg.Format(_T("Starting Harddrive Tests...") );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}

		m_cVMPerfTestHarddrive.StartHarddrive(
			hMsgWnd,
			pPacket->bHDCfgRandomRead,
			pPacket->bHDCfgRandomWrite,
			pPacket->nHDCfgMaxRead,
			pPacket->nHDCfgMaxWrite,
			pPacket->nHDCfgReadWriteDelay,
			pPacket->bHDCfgTransmitData
		);
	}

	if ( pPacket->bCpuCfgEnable ){
		
		{
			CString strMsg;
			strMsg.Format(_T("Starting Cpu Tests...") );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}

		m_cVMPerfTestCPU.StartCPU(
			hMsgWnd,
			pPacket->bCpuCfgRandom,
			pPacket->nCpuCfgMaxRand,
			pPacket->fCpuCfgSleepMS,
			pPacket->nCpuCfgThreadCnt
		);
	}

	if ( pPacket->bCpuMeasureCfgEnable ){
		
		{
			CString strMsg;
			strMsg.Format(_T("Starting CpuMeasure Tests...") );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}

		m_cVMPerfTestCPUMeasure.StartCPUMeasure(
			hMsgWnd,
			pPacket->nCpuMeasureCfgAdditionCount,
			pPacket->nCpuMeasureCfgDivisionCount,
			pPacket->nCpuMeasureCfgTransmitData,
			pPacket->nCpuMeasureCfgAutoFixMultipliers,
			pPacket->dCpuMeasureCfgFixMultipliersThreshold
		);
	}

	if ( pPacket->bMemCfgEnable ){
		
		{
			CString strMsg;
			strMsg.Format(_T("Starting Memory Tests..."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}

		m_cVMPerfTestMemory.StartMemory(
			hMsgWnd,
			pPacket->bMemCfgRandomize,
			pPacket->nMemCfgMinMem,
			pPacket->nMemCfgMaxMem,
			pPacket->bMemCfgTransmitData
		);
	}

	if ( pPacket->bNetCfgEnablePingTest || pPacket->bNetCfgEnableTcpConnTest ){

		VMPERF_TEST_NETWORK_PING_PARAM pingParam;
		_stprintf_s(pingParam.szPingHost,SSBC_PROFILE_MAX_SERVER_NAME,pPacket->szNetCfgPingHost);
		pingParam.nPingInterval		= pPacket->nNetCfgPingInterval;
		pingParam.nPingTTL			= pPacket->nNetCfgPingTTL;
		pingParam.nPingPayloadSize	= pPacket->nNetCfgPingPayloadSize;
		pingParam.bPingTransmitData = pPacket->bNetCfgPingTransmitData;

		VMPERF_TEST_NETWORK_TCPCONN_PARAM tcpconParam;
		_stprintf_s(tcpconParam.szTcpConnHost,SSBC_PROFILE_MAX_SERVER_NAME,pPacket->szNetCfgTcpConnHost);
		tcpconParam.nTcpConnInterval		= pPacket->nNetCfgTcpConnInterval;
		tcpconParam.nTcpConnPort			= pPacket->nNetCfgTcpConnPort;
		tcpconParam.nTcpConnTTL				= pPacket->nNetCfgTcpConnTTL;
		tcpconParam.bTcpConnTransmitData	= pPacket->bNetCfgTcpConnTransmitData;
	
		{
			CString strMsg;
			strMsg.Format(_T("Starting Network Tests...") );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}

		m_cVMPerfTestNetwork.StartNetwork(
			hMsgWnd,
			pPacket->bNetCfgEnablePingTest,
			pPacket->bNetCfgEnableTcpConnTest,
			pingParam,
			tcpconParam
		);
	}
	*/

	return 0;
}

void CSolidSBCClient::StopTests(bool bUnloadLibraries)
{
	std::vector<std::pair<HMODULE,CSolidSBCTestManager*>>::iterator iter = m_vecTestLibs.begin();

	for(; iter != m_vecTestLibs.end(); iter++)
	{
		std::vector<std::string> vecTestNames;
		(*iter).second->GetTestNames(vecTestNames);

		std::vector<std::string>::iterator iterName = vecTestNames.begin();
		for(; iterName != vecTestNames.end(); iterName++)
			(*iter).second->StopTest( (*iterName) );

		if ( bUnloadLibraries )
			FreeLibrary((*iter).first);
	}
}

void CSolidSBCClient::InitTests(void)
{
	TCHAR szPath[1025];
	memset(szPath,0,1025 * sizeof(TCHAR));
	GetModuleFileName(NULL,szPath,1024);

	CString fileName = szPath;
	CString dllDir   = fileName.Left( fileName.ReverseFind(_T('\\')) ).TrimRight('\\') + CString(_T("\\tests\\*.dll"));
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	if( (hFind = FindFirstFile(dllDir,&ffd)) == INVALID_HANDLE_VALUE)
		return;
	
	CSolidSBCTestManager* pTestManager = 0;
	std::pair<HMODULE,CSolidSBCTestManager*> pairLibManager;

	StopTests();
	m_vecTestLibs.clear();

	do
	{
		HMODULE hLib = LoadLibrary(ffd.cFileName);
		if( hLib == NULL ) continue;
		
		typedef const void* (*PINSTANCE_GETTER_FUNC) (void);
		PINSTANCE_GETTER_FUNC GetSolidSBCTestInstanceFunc = 0;
		GetSolidSBCTestInstanceFunc = (PINSTANCE_GETTER_FUNC)GetProcAddress(hLib,"GetSolidSBCTestInstance");

		if(GetSolidSBCTestInstanceFunc)
			pTestManager = (CSolidSBCTestManager*)GetSolidSBCTestInstanceFunc();
		else {FreeLibrary(hLib); continue;}
		
		if (!pTestManager) {FreeLibrary(hLib); continue;}

		pairLibManager.first  = hLib;
		pairLibManager.second = pTestManager;
		m_vecTestLibs.push_back(pairLibManager);
	}
	while(FindNextFile(hFind,&ffd) != 0);

	FindClose(hFind);
}