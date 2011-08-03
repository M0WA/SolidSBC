#include "StdAfx.h"
#include "SolidSBCClient.h"

#include <string>

CSolidSBCClient::CSolidSBCClient(void)
: m_bIsInitialized(FALSE)
, m_strDataSource (_T(""))
, m_pszUUID(NULL)
{
}

CSolidSBCClient::~CSolidSBCClient(void)
{
	Stop(false);
	delete [] m_pszUUID;
	m_pszUUID = NULL;
}

int CSolidSBCClient::Run( CString strDataSource, DWORD dwSrvConfPort, DWORD dwSrvDataPort )
{
	m_dwSrvConfPort = dwSrvConfPort;
	m_dwSrvDataPort = dwSrvDataPort;

	if ( m_bIsInitialized )
		Stop();

	m_strDataSource = strDataSource;

	int nInit = Init();
	{
		CString strMsg;
		strMsg.Format(_T("DataSource = \"%s\", SrvConfPort = %d, SrvDataPort = %d")
			,strDataSource,dwSrvConfPort,dwSrvDataPort);
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

	if ( !InitTests() )
		return 1;

	if ( !GetClientUUID() )
		return 2;

	USES_CONVERSION;
	SOCKADDR_IN target;
	target.sin_family      = AF_INET;
	target.sin_port        = htons(static_cast<USHORT>(m_dwSrvConfPort));
	target.sin_addr.s_addr = inet_addr(T2A(m_strDataSource));
	
	m_cCliConfigSocket.SetClientUUID(m_pszUUID);
	m_cCliConfigSocket.Connect( target );
	m_bIsInitialized = TRUE;

	return 0;
}

int CSolidSBCClient::StartResultConnection(SOCKADDR_IN target)
{
	{
		USES_CONVERSION;
		CString strMsg;
		strMsg.Format(_T("CSolidSBCClient::StartResultConnection(), m_pszUUID = %s"),A2T(m_pszUUID));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	m_cCliResultSocket.SetClientUUID(m_pszUUID);
	m_cCliResultSocket.Connect(target);

	return 0;
}

int CSolidSBCClient::SendTestResult(CSolidSBCTestResult* pResult)
{
	if ( !m_bIsInitialized )
		return 1;
	
	return m_cCliResultSocket.SendTestResultPacket(pResult);
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

int CSolidSBCClient::StartTests(void)
{
	m_ResultManager.StopResultManager();

	USES_CONVERSION;
	std::vector<CString> vecTestXmls;
	if ( !m_cCliConfigSocket.GetTestConfigXMLs(vecTestXmls) )
		return 0;
	
	bool bFound = false;
	int  nStartedTests = 0;

	std::vector<CString>::iterator iXml;
	std::vector< std::pair<HMODULE,CSolidSBCTestManager*> >::iterator iTest;

	for ( iXml = vecTestXmls.begin(); iXml != vecTestXmls.end(); iXml++)
	{
		CString sTestName       = CSolidSBCTestConfig::GetTestNameFromXML(*iXml);
		std::string strTestName = T2A(sTestName);

		CString sXmlConfig       = *iXml;
		std::string strXmlConfig = T2A(sXmlConfig);

		m_lockTestLibs.Lock();
		for ( iTest = m_vecTestLibs.begin(); iTest != m_vecTestLibs.end(); iTest++)
		{
			CSolidSBCTestManager* pTestManager = (*iTest).second;
			if ( pTestManager && pTestManager->HasTestName(strTestName) )
			{
				pTestManager->StartTest(strXmlConfig);
				bFound = true;
				nStartedTests++;
				break;
			}	
		}
		m_lockTestLibs.Unlock();
	}

	if ( nStartedTests )
		m_ResultManager.StartResultManager(&m_lockTestLibs,&m_vecTestLibs);

	return 0;
}

void CSolidSBCClient::StopTests(bool bUnloadLibraries)
{
	m_lockTestLibs.Lock();
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
	m_lockTestLibs.Unlock();
}

bool CSolidSBCClient::InitTests(void)
{
	TCHAR szPath[1025];
	memset(szPath,0,1025 * sizeof(TCHAR));
	GetModuleFileName(NULL,szPath,1024);

	CString fileName = szPath;
	CString dllDir   = fileName.Left( fileName.ReverseFind(_T('\\')) ).TrimRight('\\') + CString(_T("\\tests\\*.dll"));
	WIN32_FIND_DATA ffd;
	HANDLE hFind;

	if( (hFind = FindFirstFile(dllDir,&ffd)) == INVALID_HANDLE_VALUE)
	{
		CString strMsg;
		strMsg.Format(_T("Could not find any test-dlls in %s."), dllDir );
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		return false;
	}
	
	CSolidSBCTestManager* pTestManager = 0;
	std::pair<HMODULE,CSolidSBCTestManager*> pairLibManager;

	StopTests();

	m_lockTestLibs.Lock();
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
		
		if (!pTestManager) 
		{
			{
				CString strMsg;
				strMsg.Format(_T("Could not find any valid testmanager in %s."), ffd.cFileName );
				CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_WARN);
			}
			FreeLibrary(hLib); 
			continue;
		}

		pairLibManager.first  = hLib;
		pairLibManager.second = pTestManager;
		m_vecTestLibs.push_back(pairLibManager);
		
		{
			CString strMsg;
			strMsg.Format(_T("Loaded testmanager from %s."), ffd.cFileName );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
	}
	while(FindNextFile(hFind,&ffd) != 0);
	FindClose(hFind);

	int nLibs = (int)m_vecTestLibs.size();
	m_lockTestLibs.Unlock();

	if( !nLibs)
	{
		CString strMsg;
		strMsg.Format(_T("Could not find any valid testmanager in %s."), dllDir );
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		return false;
	}
	else
	{
		CString strMsg;
		strMsg.Format(_T("Loaded %d valid test-dll(-s)."), nLibs );
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		return true;
	}
}