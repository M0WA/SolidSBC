#include "StdAfx.h"
#include "SolidSBCServerService.h"

CSolidSBCServerService::CSolidSBCServerService(void)
{
}

CSolidSBCServerService::~CSolidSBCServerService(void)
{
}

int CSolidSBCServerService::InstallService(void)
{
	//already installed
	if ( IsServiceInstalled() ){
		{
			CString strMsg;
			strMsg.Format(_T("InstallService: Deleting previously installed SolidSBC Server Service first"));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
		DeleteService();}

	SC_HANDLE schSCManager;
    SC_HANDLE schService;
    TCHAR szPath[MAX_PATH];

    if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
    {
		{
			CString strMsg;
			strMsg.Format(_T("InstallService: Cannot install SolidSBC Server Service (%d)\n"), GetLastError());
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
		}
        return FALSE;
    }
 // Get a handle to the SCM database. 
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
		{
			CString strMsg;
			strMsg.Format(_T("InstallService: OpenSCManager failed (%d)\n"), GetLastError());
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
		}
        return FALSE;
    }

	//fit start type of the service according its settings
	int nServiceStartType = SERVICE_DEMAND_START;

    // Create the service
	//TODO: start with least privileges, installs as LocalSystem account
    schService = CreateService( 
        schSCManager,              // SCM database 
        SSBC_SRVSVC_NAME,    // name of service 
        SSBC_SRVSVC_DISPLAY, // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        nServiceStartType,         // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if (schService == NULL) 
    {
        CloseServiceHandle(schSCManager);
		{
			CString strMsg;
			strMsg.Format(_T("InstallService: CreateService failed (%d)"), GetLastError());
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
		}
        return FALSE;
    }
	else {
		{
			CString strMsg;
			strMsg.Format(_T("SolidSBC Server Service installed successfully"));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO); 
		}
	}

	//change service description
	SERVICE_DESCRIPTION serviceDesc = {SSBC_SRVSVC_DESCRIPTION};
	ChangeServiceConfig2((SC_HANDLE)schService,SERVICE_CONFIG_DESCRIPTION,&serviceDesc);
	CloseServiceHandle((SC_HANDLE)schService);

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);

	return TRUE;
}

int CSolidSBCServerService::DeleteService(void)
{
	HANDLE schSCManager;
	SC_HANDLE hService;
	schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (schSCManager == NULL)
		return false;

	hService=OpenService((SC_HANDLE)schSCManager,SSBC_SRVSVC_NAME,SERVICE_ALL_ACCESS);
	if (hService == NULL)
		return false;

	::DeleteService(hService);
	_SERVICE_STATUS ss;
	ControlService(hService,SERVICE_CONTROL_STOP,&ss);

	if(CloseServiceHandle(hService)==0)
		return false;

	return true;
}

bool CSolidSBCServerService::IsServiceInstalled(void)
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
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
        return FALSE;
    }

    // Get a handle to the service.
    schService = OpenService( 
        schSCManager,						// SCM database 
        SSBC_SRVSVC_NAME,					// name of service 
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

int CSolidSBCServerService::StartServerService(void)
{
	g_cClientList.Init();

	m_cConfigClientSocket.StartListen(SSBC_CFG_SERVER_PORT);
	m_cResultClientSocket.StartListen(SSBC_DTA_SERVER_PORT);

	return 0;
}

void CSolidSBCServerService::SetImagePath(CString strImagePath)
{
	g_strImagePath = strImagePath;
}
