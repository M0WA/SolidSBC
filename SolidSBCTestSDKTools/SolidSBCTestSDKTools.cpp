
// SolidSBCTestSDKTools.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SolidSBCTestSDKTools.h"
#include "SolidSBCTestSDKToolsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolidSBCTestSDKToolsApp

BEGIN_MESSAGE_MAP(CSolidSBCTestSDKToolsApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSolidSBCTestSDKToolsApp construction

CSolidSBCTestSDKToolsApp::CSolidSBCTestSDKToolsApp()
{
	::AfxInitRichEdit2();
}


// The one and only CSolidSBCTestSDKToolsApp object

CSolidSBCTestSDKToolsApp theApp;


// CSolidSBCTestSDKToolsApp initialization

BOOL CSolidSBCTestSDKToolsApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("mo-sys-sdktools"));

	CSolidSBCTestSDKToolsDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

