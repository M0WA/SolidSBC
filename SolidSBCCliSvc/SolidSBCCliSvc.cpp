// SolidSBCCliSvc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SolidSBCCliSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SERVICE_STATUS        g_ClientServiceStatus;
SERVICE_STATUS_HANDLE g_ClientServiceStatusHandle  = NULL;
HANDLE				  g_hClientSvcStopEvent        = NULL;

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
void ParseClientServiceCommandLine(int argc, TCHAR* argv[]);
void PrintVersion(void);
void PrintUsage(void);
CString GetImageBinaryPath(TCHAR* pszImageFileName);

// The one and only application object
//CWinApp theApp;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit( ::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		CString strBinaryPath = GetImageBinaryPath(argv[0]);
		g_cClientService.SetImagePath(strBinaryPath);

		for (int i=0;i<argc;i++){

			if ( !_tcscmp( argv[i], _T("/install") ) ){
				ParseClientServiceCommandLine(argc,argv);
				if ( g_cClientService.InstallService() )
					_tprintf(_T("SolidSBC Client Service installed successfully.\n"));
				else
					_tprintf(_T("An error occured while installing SolidSBC Client Service. Please check logfile for detailed information.\n"));
				system("pause");
				return nRetCode;
			} 
			else if ( !_tcscmp( argv[i], _T("/delete") ) ){
				if ( g_cClientService.DeleteService() )
					_tprintf(_T("SolidSBC Client Service deleted successfully.\n"));
				else
					_tprintf(_T("SolidSBC Client Service could not be deleted. Please check logfile for detailed information.\n"));
				system("pause");
				return nRetCode;
			} 
			else if ( !_tcscmp( argv[i], _T("/version") ) ){
				PrintVersion();
				system("pause");
				return nRetCode;
			} 
			else if ( !_tcscmp( argv[i], _T("/help") ) || !_tcscmp( argv[i], _T("/?") ) ){
				PrintUsage();
				system("pause");
				return nRetCode;
			}
		}

		_tprintf(_T("SolidSBC Client Service is starting.\n"));
		SERVICE_TABLE_ENTRY DispatchTable[]=
		{{SSBC_CLISVC_NAME,ServiceMain},{NULL,NULL}};
		StartServiceCtrlDispatcher(DispatchTable);
	}

	return nRetCode;
}

CString GetImageBinaryPath(TCHAR* pszImageFileName)
{
	CString strImagePathName = pszImageFileName;
	int nLastBackslash = strImagePathName.ReverseFind( _T('\\') );
	return strImagePathName.Left(nLastBackslash);
}

void PrintUsage(void)
{
	_tprintf(_T("SolidSBC Client Service\n"));
	_tprintf(_T("Usage:\n\n"));
	_tprintf(_T("/install [opts]: installs service (need administrator privileges)\n"));
	_tprintf(_T("\t/autoreconnect   : reconnect on connection loss\n"));
	_tprintf(_T("\t/remote   [ip]   : ip of solidsbc server\n"));
	_tprintf(_T("\t/dataport [port] : data port of solidsbc server\n"));
	_tprintf(_T("\t/confport [port] : conf port of solidsbc server\n"));
	_tprintf(_T("/delete : deletes service  (need administrator privileges)\n"));
	_tprintf(_T("/version: prints version\n"));
	_tprintf(_T("/help   : prints this help\n"));
	_tprintf(_T("\n"));
}

void PrintVersion(void)
{
	CString IfDebug = _T("");
#ifdef _DEBUG
	IfDebug = _T(" DEBUG .\n !!! WARNING: running in debug version is not supported !!!"); //show debug version
#endif

	_tprintf(_T("SolidSBC Client Service Version %s.%s%s.\n"),SSBC_CLISVC_VERSION_MAJOR,SSBC_CLISVC_VERSION_MINOR,IfDebug);
}

void ParseClientServiceCommandLine(int argc, TCHAR* argv[])
{	
	SSBC_CLISVC_PARAM cliParam;
	ZeroMemory(&cliParam,sizeof(SSBC_CLISVC_PARAM));

	cliParam.dwSrvDataPort = 52411;
	cliParam.dwSrvConfPort = 52410;

	for ( int i = 0; i < argc; i++ ){
		if ( !_tcscmp( argv[i], _T("/autostart") ) ){
			cliParam.bAutoStart = TRUE;
		} else if ( !_tcscmp( argv[i], _T("/autoreconnect") ) ){
			cliParam.bAutoReconnect = TRUE;
		} else if ( !_tcscmp( argv[i], _T("/remote") ) ){
			i++;
			_stprintf_s((TCHAR*)cliParam.szDataSource,MAX_SERVER_NAME,argv[i]);
		} else if ( !_tcscmp( argv[i], _T("/dataport") ) ){
			i++;
			cliParam.dwSrvDataPort   = _ttoi(argv[i]);
		} else if ( !_tcscmp( argv[i], _T("/confport") ) ){
			i++;
			cliParam.dwSrvConfPort   = _ttoi(argv[i]);
		}
	}
	
	CString strDatasource;
	strDatasource.Format(_T("%s"),cliParam.szDataSource);

	if ( strDatasource == _T("") ){
			//handle missing cmdline arguments...
	}

	g_cClientService.SaveParameters(cliParam.bAutoStart,strDatasource,cliParam.dwSrvConfPort,cliParam.dwSrvDataPort,cliParam.bAutoReconnect);
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{	
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2),&wsaData);

	g_ClientServiceStatus.dwServiceType = SERVICE_WIN32;
	g_ClientServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ClientServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ClientServiceStatus.dwWin32ExitCode = 0;
	g_ClientServiceStatus.dwServiceSpecificExitCode = 0;
	g_ClientServiceStatus.dwCheckPoint = 0;
	g_ClientServiceStatus.dwWaitHint = 0;

	g_ClientServiceStatusHandle = RegisterServiceCtrlHandler(SSBC_CLISVC_NAME,ServiceCtrlHandler); 
	if (g_ClientServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}

	// Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.
    g_hClientSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name
	if ( g_hClientSvcStopEvent == NULL)
    {
        return;
    }

	g_ClientServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ClientServiceStatus.dwCheckPoint   = 0;
	g_ClientServiceStatus.dwWaitHint     = 0;

	if (!SetServiceStatus (g_ClientServiceStatusHandle, &g_ClientServiceStatus))
	{
		SetEvent(g_hClientSvcStopEvent);
		return;
	}

	g_cClientService.StartClientService();

	//wait for stop event
	WaitForSingleObject(g_hClientSvcStopEvent,INFINITE);
}

void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	switch(Opcode)
	{
	case SERVICE_CONTROL_STOP:
		{
			CString strMsg;
			strMsg.Format(_T("\n\n\t\t===========================================\n\t\tSolidSBC Client Service v%s.%s is ending.\n\t\t===========================================\n"),SSBC_CLISVC_VERSION_MAJOR,SSBC_CLISVC_VERSION_MINOR);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}
		g_cClientService.StopClientService();

		SetEvent(g_hClientSvcStopEvent);

		g_ClientServiceStatus.dwWin32ExitCode = 0;
		g_ClientServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ClientServiceStatus.dwCheckPoint = 0;
		g_ClientServiceStatus.dwWaitHint = 0;
		SetServiceStatus (g_ClientServiceStatusHandle,&g_ClientServiceStatus);
		break;

	case SERVICE_CONTROL_PAUSE: 
		g_ClientServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;

	case SERVICE_CONTROL_CONTINUE:
		g_ClientServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		break; 

	default:
#ifdef _DEBUG
		{
			CString strMsg;
			strMsg.Format(_T("ServiceCtrlHandler(DWORD Opcode = %d): Unknown OPCODE received."),Opcode);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
#endif
		break;
	}
}