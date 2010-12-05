// SolidSBCSrvSvc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SolidSBCSrvSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object
//CWinApp theApp;

CSolidSBCServerService	g_cServerService;
SERVICE_STATUS			g_ServerServiceStatus;
SERVICE_STATUS_HANDLE	g_ServerServiceStatusHandle  = NULL;
HANDLE					g_hServerSvcStopEvent        = NULL;

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
void WINAPI ServiceCtrlHandler(DWORD Opcode);
void PrintVersion(void);
void PrintUsage(void);
CString GetImageBinaryPath(TCHAR* pszImageFileName);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		
		CString strBinaryPath = GetImageBinaryPath(argv[0]);
		g_cServerService.SetImagePath(strBinaryPath);

		for (int i=0;i<argc;i++){

			if ( !_tcscmp( argv[i], _T("/install") ) ){
				if ( g_cServerService.InstallService() )
					_tprintf(_T("SolidSBC Server Service installed successfully.\n"));
				else
					_tprintf(_T("An error occured while installing SolidSBC Server Service. Please check logfile for detailed information.\n"));
				system("pause");
				return nRetCode;
			} 
			else if ( !_tcscmp( argv[i], _T("/delete") ) ){
				if ( g_cServerService.DeleteService() )
					_tprintf(_T("SolidSBC Server Service deleted successfully.\n"));
				else
					_tprintf(_T("SolidSBC Server Service could not be deleted. Please check logfile for detailed information.\n"));
				system("pause");
				return nRetCode;
			} 
			else if ( !_tcscmp( argv[i], _T("/version") ) ){
				PrintVersion();
				system("pause");
				return nRetCode;
			} 
			else if ( !_tcscmp( argv[i], _T("/help") ) ){
				PrintUsage();
				system("pause");
				return nRetCode;
			}
		}

		_tprintf(_T("SolidSBC Server Service is starting.\n"));
		SERVICE_TABLE_ENTRY DispatchTable[]=
		{{SSBC_SRVSVC_NAME,ServiceMain},{NULL,NULL}};
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

void PrintVersion(void)
{
	CString IfDebug = _T("");
#ifdef _DEBUG
	IfDebug = _T(" DEBUG .\n !!! WARNING: running in debug version is not supported !!!"); //show debug version
#endif

	_tprintf(_T("SolidSBC Server Service Version %s.%s%s.\n"),SSBC_SRVSVC_VERSION_MAJOR,SSBC_SRVSVC_VERSION_MINOR,IfDebug);
}

void PrintUsage(void)
{
}

void WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2),&wsaData);

	g_ServerServiceStatus.dwServiceType				= SERVICE_WIN32;
	g_ServerServiceStatus.dwCurrentState			= SERVICE_START_PENDING;
	g_ServerServiceStatus.dwControlsAccepted		= SERVICE_ACCEPT_STOP;
	g_ServerServiceStatus.dwWin32ExitCode			= 0;
	g_ServerServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServerServiceStatus.dwCheckPoint				= 0;
	g_ServerServiceStatus.dwWaitHint				= 0;

	g_ServerServiceStatusHandle = RegisterServiceCtrlHandler(SSBC_SRVSVC_NAME,ServiceCtrlHandler); 
	if (g_ServerServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}

	// Create an event. The control handler function, SvcCtrlHandler,
    // signals this event when it receives the stop control code.
    g_hServerSvcStopEvent = CreateEvent(
                         NULL,    // default security attributes
                         TRUE,    // manual reset event
                         FALSE,   // not signaled
                         NULL);   // no name
	if ( g_hServerSvcStopEvent == NULL)
    {
        return;
    }

	/*
	//get service parameters from config file
	if ( !LoadParameters() ){
		SetEvent(g_hSvcStopEvent);
		return;}
	*/
	
	g_ServerServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServerServiceStatus.dwCheckPoint   = 0;
	g_ServerServiceStatus.dwWaitHint     = 0;

	if (!SetServiceStatus (g_ServerServiceStatusHandle, &g_ServerServiceStatus))
	{
		SetEvent(g_hServerSvcStopEvent);
		return;
	}

	{
		CString strMsg;
		strMsg.Format(_T("\n\n\t\t===========================================\n\t\tSolidSBC Server Service v%s.%s is starting.\n\t\t===========================================\n"),SSBC_SRVSVC_VERSION_MAJOR,SSBC_SRVSVC_VERSION_MINOR);
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
	}
	
	g_cServerService.StartServerService();

	//wait for stop event
	WaitForSingleObject(g_hServerSvcStopEvent,INFINITE);
	
	{
		CString strMsg;
		strMsg.Format(_T("\n\n\t\t===========================================\n\t\tSolidSBC Server Service v%s.%s is ending.\n\t\t===========================================\n"),SSBC_SRVSVC_VERSION_MAJOR,SSBC_SRVSVC_VERSION_MINOR);
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
	}
}

void WINAPI ServiceCtrlHandler(DWORD Opcode)
{
	switch(Opcode)
	{
	case SERVICE_CONTROL_STOP:
		SetEvent(g_hServerSvcStopEvent);

		//StopService();
		g_cClientList.Empty();

		g_ServerServiceStatus.dwWin32ExitCode = 0;
		g_ServerServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServerServiceStatus.dwCheckPoint = 0;
		g_ServerServiceStatus.dwWaitHint = 0;
		SetServiceStatus (g_ServerServiceStatusHandle,&g_ServerServiceStatus);
		break;

	case SERVICE_CONTROL_PAUSE: 
		g_ServerServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;

	case SERVICE_CONTROL_CONTINUE:
		g_ServerServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;

	case SERVICE_CONTROL_INTERROGATE:
		break; 
	}
}