
// SolidSBCCliCfgGUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SolidSBCCliCfgGUI.h"
#include "SolidSBCCliCfgGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolidSBCCliCfgGUIApp

BEGIN_MESSAGE_MAP(CSolidSBCCliCfgGUIApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSolidSBCCliCfgGUIApp construction

CSolidSBCCliCfgGUIApp::CSolidSBCCliCfgGUIApp()
{
}


// The one and only CSolidSBCCliCfgGUIApp object

CSolidSBCCliCfgGUIApp theApp;


// CSolidSBCCliCfgGUIApp initialization

BOOL CSolidSBCCliCfgGUIApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxInitRichEdit2( );

	CWinAppEx::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("SolidSBCCliCfgGUI"));

	CSolidSBCCliCfgGUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


int CSolidSBCCliCfgGUIApp::ExitInstance()
{
	CWinAppEx::ExitInstance();

	return 0;
}
