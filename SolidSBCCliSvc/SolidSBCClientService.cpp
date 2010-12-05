#include "StdAfx.h"
#include "SolidSBCClientService.h"

CSolidSBCClientService g_cClientService;

CSolidSBCClientService::CSolidSBCClientService(void)
: m_nReconnectCounter(0)
{
}

CSolidSBCClientService::~CSolidSBCClientService(void)
{
}

int CSolidSBCClientService::InstallService(void)
{
	//already installed
	if ( IsServiceInstalled() ){
		CString strMsg;
		strMsg.Format(_T("InstallService: Deleting previously installed SolidSBC Client Service first"));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		DeleteService();}

	SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
    {
		CString strMsg;
		strMsg.Format(_T("InstallService: Cannot install SolidSBC Client Service (%d)\n"), GetLastError());
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
        return FALSE;
    }

	// Get a handle to the SCM database. 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
		CString strMsg;
		strMsg.Format(_T("InstallService: OpenSCManager failed (%d)\n"), GetLastError());
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
        return FALSE;
    }

	//fit start type of the service according its settings
	int nServiceStartType = SERVICE_DEMAND_START;
	if ( m_CliSvcParam.bAutoStart )
		nServiceStartType = SERVICE_AUTO_START;

    // Create the service
	//TODO: start with least privileges, installs as LocalSystem account
    schService = CreateService( 
        schSCManager,				// SCM database 
        SSBC_CLISVC_NAME,			// name of service 
        SSBC_CLISVC_DISPLAY,		// service name to display 
        SERVICE_ALL_ACCESS,			// desired access 
        SERVICE_WIN32_OWN_PROCESS,	// service type 
        nServiceStartType,			// start type 
        SERVICE_ERROR_NORMAL,		// error control type 
        szPath,						// path to service's binary 
        NULL,						// no load ordering group 
        NULL,						// no tag identifier 
        NULL,						// no dependencies 
        NULL,						// LocalSystem account 
        NULL);						// no password 
 
    if (schService == NULL) 
    {
        CloseServiceHandle(schSCManager);
		CString strMsg;
		strMsg.Format(_T("InstallService: CreateService failed (%d)"), GetLastError());
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
        return FALSE;
    }
	else {
		CString strMsg;
		strMsg.Format(_T("SolidSBC Client Service installed successfully"));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO); 
	}

	//change service description
	SERVICE_DESCRIPTION serviceDesc = {SSBC_CLISVC_DESCRIPTION};
	ChangeServiceConfig2((SC_HANDLE)schService,SERVICE_CONFIG_DESCRIPTION,&serviceDesc);
	CloseServiceHandle((SC_HANDLE)schService);

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);

	return TRUE;
}

int CSolidSBCClientService::DeleteService(void)
{
	HANDLE schSCManager;
	SC_HANDLE hService;
	schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (schSCManager == NULL)
		return false;

	hService=OpenService((SC_HANDLE)schSCManager,SSBC_CLISVC_NAME,SERVICE_ALL_ACCESS);
	if (hService == NULL)
		return false;

	::DeleteService(hService);
	_SERVICE_STATUS ss;
	ControlService(hService,SERVICE_CONTROL_STOP,&ss);

	if(CloseServiceHandle(hService)==0)
		return false;

	return true;
}

bool CSolidSBCClientService::IsServiceInstalled(void)
{ 
	SC_HANDLE schSCManager;
    SC_HANDLE schService;

    // Get a handle to the SCM database. 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
		CString strMsg;
		strMsg.Format(_T("IsServiceInstalled: OpenSCManager failed (%d)"), GetLastError());
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
        return FALSE;
    }

    // Get a handle to the service.
    schService = OpenService( 
        schSCManager,						// SCM database 
        SSBC_CLISVC_NAME,					// name of service 
        SERVICE_ALL_ACCESS);				// full access 
 
    if (schService == NULL)
    { 
        CloseServiceHandle(schSCManager);
        return FALSE;
    }    

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);

	return TRUE;
}

CString CSolidSBCClientService::GetSettingsFileName(void)
{
	CString strFileName = _T("");
	if ( g_strImagePath != _T("") ){
		strFileName.Format(_T("%s\\%s"),g_strImagePath,SSBC_CLISVC_SETTINGS_FILENAME);
	} else {			
		TCHAR szFileNamePath[1024];
		GetModuleFileName(NULL,szFileNamePath,1024);
		CString strFileNamePath = szFileNamePath;
		int nPos = strFileNamePath.ReverseFind(_T('\\'));
		CString strPath = strFileNamePath.Left(nPos);
		strFileName.Format(_T("%s\\%s"),strPath,SSBC_CLISVC_SETTINGS_FILENAME);
		g_strImagePath = szFileNamePath;
	}
	return strFileName;
}

void CSolidSBCClientService::SaveParameters(
	  BOOL bAutoStart
	, CString strDataSource
	, DWORD   dwSrvConfPort
	, DWORD   dwSrvDataPort
	, UINT nProfileID
	, BOOL bAutoReconnect)
{
	SSBC_CLISVC_PARAM tmpServiceParam;
	ZeroMemory(&tmpServiceParam,sizeof(tmpServiceParam));

	tmpServiceParam.bAutoStart      = bAutoStart;
	tmpServiceParam.dwSrvConfPort   = dwSrvConfPort;
	tmpServiceParam.dwSrvDataPort   = dwSrvDataPort;
	tmpServiceParam.nProfileID      = nProfileID;
	tmpServiceParam.bAutoReconnect  = bAutoReconnect;
	_stprintf_s( (TCHAR*)tmpServiceParam.szDataSource, MAX_SERVER_NAME, strDataSource );

	TCHAR szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szPath);
	CString strFileName = GetSettingsFileName();
	{
		CString strMsg;
		strMsg.Format(_T("Saving settings for SolidSBC Client Service to file %s, pwd = %s"),strFileName,szPath);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		_tprintf_s(_T("%s\r\n"),strMsg);
	}

	//save new parameters to file
	int nOpenFlags = ( CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive | CFile::typeBinary );
	TRY
	{
		CFile cServiceParameterFile( strFileName, nOpenFlags );
		cServiceParameterFile.Write( &tmpServiceParam, sizeof( tmpServiceParam ) );
		cServiceParameterFile.Close( );
	}
	CATCH(CException, pEx)
	{
		CString strMsg;
		strMsg.Format(_T("Error: Could not write SolidSBC Client Service parameters to file %s."),strFileName);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		_tprintf_s(_T("%s\r\n"),strMsg);
	}
	END_CATCH

	//refesh parameters in memory
	LoadParameters();
}

BOOL CSolidSBCClientService::LoadParameters(void)
{
	SSBC_CLISVC_PARAM tmpServiceParam;
	ZeroMemory( &tmpServiceParam, sizeof( tmpServiceParam ) );

	//read new parameters from file
	int nOpenFlags = ( CFile::modeRead | CFile::shareExclusive | CFile::typeBinary );
	TRY
	{
		CFile cServiceParameterFile( GetSettingsFileName(), nOpenFlags );
		cServiceParameterFile.Read( &tmpServiceParam, sizeof( tmpServiceParam ) );
		cServiceParameterFile.Close( );
	}
	CATCH(CException, pEx)
	{
		CString strMsg;
		strMsg.Format(_T("LoadParameters: Could not load SolidSBC Client Service parameters from file %s"),GetSettingsFileName());
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		return FALSE;
	}
	END_CATCH

	//refresh parameters in memory from file
	ZeroMemory( &m_CliSvcParam, sizeof( m_CliSvcParam ) );
	m_CliSvcParam = tmpServiceParam;

	return TRUE;
}

int CSolidSBCClientService::StopClientService(void)
{
	m_cSolidSBCClientMutex.Lock();
	int nReturn = m_cSolidSBCClient.Stop();
	m_cSolidSBCClientMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientService::StartClientService(void)
{
	//get service parameters from config file
	if ( !LoadParameters() ){
		return false;}

	CString strDataSource;
	strDataSource.Format(_T("%s"),m_CliSvcParam.szDataSource);

	m_cSolidSBCClientMutex.Lock();

	if ( m_cSolidSBCClient.Run( strDataSource, m_CliSvcParam.dwSrvConfPort, m_CliSvcParam.dwSrvDataPort, m_CliSvcParam.nProfileID ) ){

		//error handling
		CString strMsg;
		strMsg.Format(_T("StartClientService: Could not start SolidSBC Client Service."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);

	} else{

		CString strMsg;
		strMsg.Format(_T("\n\n\t\t===========================================\n\t\tSolidSBC Client Service v%s.%s has started.\n\t\t===========================================\n"),SSBC_CLISVC_VERSION_MAJOR,SSBC_CLISVC_VERSION_MINOR);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);

	}

	m_cSolidSBCClientMutex.Unlock();
	return 0;
}

int CSolidSBCClientService::StartResultConnection(void)
{
	USES_CONVERSION;
	
	CString strDataSource;
	strDataSource.Format(_T("%s"),m_CliSvcParam.szDataSource);

	SOCKADDR_IN target;
	target.sin_family = AF_INET;
	target.sin_port   = htons(static_cast<USHORT>(m_CliSvcParam.dwSrvDataPort));
	target.sin_addr.s_addr = inet_addr( T2A(strDataSource) );

	m_cSolidSBCClientMutex.Lock();
	m_nReconnectCounter = 0;
	int nReturn = m_cSolidSBCClient.StartResultConnection(target);
	m_cSolidSBCClientMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientService::StartTestFromProfilePacket(PSSBC_PROFILE_REPLY_PACKET pPacket)
{
	m_cSolidSBCClientMutex.Lock();
	int nReturn = m_cSolidSBCClient.StartTestFromProfilePacket(pPacket);
	m_cSolidSBCClientMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientService::ChangeProfile(PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET pPacket)
{
	m_cSolidSBCClientMutex.Lock();

	m_cSolidSBCClient.StopTests();
	m_cSolidSBCClient.Stop();

	m_CliSvcParam.nProfileID = pPacket->nChangeToProfileID;
	CString strDataSource;
	strDataSource.Format(_T("%s"),m_CliSvcParam.szDataSource);

	if ( m_cSolidSBCClient.Run( strDataSource, m_CliSvcParam.dwSrvConfPort, m_CliSvcParam.dwSrvDataPort, m_CliSvcParam.nProfileID ) ){
		//error handling
		CString strMsg;
		strMsg.Format(_T("ChangeProfile: Could not restart SolidSBC Client Service."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
	}else{
		CString strMsg;
		strMsg.Format(_T("SolidSBC Client Service has changed profile to id %d."),m_CliSvcParam.nProfileID);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
	}

	m_cSolidSBCClientMutex.Unlock();
	return 0;
}

void CSolidSBCClientService::SendTestResult( PSSBC_TEST_RESULT_PACKET pPacket )
{

#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCClientService::SendTestResult() pPacket->hdr.type = %d."),pPacket->hdr.type);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif

	//send test result packet
	m_cSolidSBCClientMutex.Lock();
	int nReturn = m_cSolidSBCClient.SendTestResult(pPacket);
	m_cSolidSBCClientMutex.Unlock();
	
	//error while sending, result connection closed...
	if ( nReturn == -1 ){
		g_cClientService.ConnectionClosed();}
}

void CSolidSBCClientService::ConnectionClosed( void )
{
	m_cSolidSBCClientMutex.Lock();
	
	{
		//log the error
			CString strMsg;
			strMsg.Format(_T("Reconnecting to server %s using profile %d."),m_CliSvcParam.szDataSource,m_CliSvcParam.nProfileID);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	//stopping all test
	m_cSolidSBCClient.StopTests();
	m_cSolidSBCClient.Stop(false);

	//try to reconnect if specified by settings
	if ( m_CliSvcParam.bAutoReconnect ){
				
		if ( m_nReconnectCounter == 3 )	{
		
			//wait 3s after 3 failed retries
			Sleep(3 * 1000);

			//reset counter
			m_nReconnectCounter = 0;

			//log the error
			CString strMsg;
			strMsg.Format(_T("Could not reconnect to server %s using profile %d."),m_CliSvcParam.szDataSource,m_CliSvcParam.nProfileID);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}

		CString strDataSource;
		strDataSource.Format(_T("%s"),m_CliSvcParam.szDataSource);
		if ( m_cSolidSBCClient.Run( strDataSource, m_CliSvcParam.dwSrvConfPort, m_CliSvcParam.dwSrvDataPort, m_CliSvcParam.nProfileID ) ){

			//log the error as debug, errors logged every 3 retries...
			CString strMsg;
			strMsg.Format(_T("Could not reconnect to server %s using profile %d (retry count: %d)."),m_CliSvcParam.szDataSource,m_CliSvcParam.nProfileID,m_nReconnectCounter);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);

		}

		m_nReconnectCounter++;
	}

	m_cSolidSBCClientMutex.Unlock();
}