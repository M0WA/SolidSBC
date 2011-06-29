
// SolidSBCResultViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SolidSBCResultViewer.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "SolidSBCResultViewerDoc.h"
#include "SolidSBCResultViewerView.h"

#include "SolidSBCClientListDoc.h"
#include "SolidSBCClientListView.h"

#include "SolidSBCClientHistoryDoc.h"
#include "SolidSBCClientHistoryView.h"

#include "SolidSBCMemoryResultDoc.h"
#include "SolidSBCMemoryResultView.h"

#include "SolidSBCCPUMeasureResultDoc.h"
#include "SolidSBCCPUMeasureResultView.h"

#include "SolidSBCHDResultDoc.h"
#include "SolidSBCHDResultView.h"

#include "SolidSBCPingResultDoc.h"
#include "SolidSBCPingResultView.h"

#include "SolidSBCTCPConResultDoc.h"
#include "SolidSBCTCPConResultView.h"

#include "SolidSBCGraphPlotterView.h"
#include "SolidSBCGraphPlotterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolidSBCResultViewerApp

BEGIN_MESSAGE_MAP(CSolidSBCResultViewerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSolidSBCResultViewerApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CSolidSBCResultViewerApp::OnFileNew)
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CSolidSBCResultViewerApp construction

CSolidSBCResultViewerApp::CSolidSBCResultViewerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SolidSBCResultViewer.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSolidSBCResultViewerApp object

CSolidSBCResultViewerApp theApp;


// CSolidSBCResultViewerApp initialization

BOOL CSolidSBCResultViewerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	AfxInitRichEdit2();

	// Standard initialization
	SetRegistryKey(_T("SolidSBC Result Viewer"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	
	{
		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(IDR_SolidSBCResultVTYPE,
			RUNTIME_CLASS(CSolidSBCResultViewerDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CSolidSBCResultViewerView));
		if (!pDocTemplate)
			return FALSE;
		AddDocTemplate(pDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCCLIENTLISTVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCClientListDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCClientListView));
		AddDocTemplate(pNewDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCCLIENTHISTORYFORM_TMPL,
			RUNTIME_CLASS(CSolidSBCClientHistoryDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCClientHistoryView));
		AddDocTemplate(pNewDocTemplate);
	}
	
	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCMEMORYRESULTVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCMemoryResultDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCMemoryResultView));
		AddDocTemplate(pNewDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCCPUMEASURERESULTVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCCPUMeasureResultDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCCPUMeasureResultView));
		AddDocTemplate(pNewDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCHDRESULTVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCHDResultDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCHDResultView));
		AddDocTemplate(pNewDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCTCPCONRESULTVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCTCPConResultDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCTCPConResultView));
		AddDocTemplate(pNewDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCPINGRESULTVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCPingResultDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCPingResultView));
		AddDocTemplate(pNewDocTemplate);
	}

	{
		CMultiDocTemplate* pNewDocTemplate = new CMultiDocTemplate(
			IDR_SOLIDSBCGRAPHPLOTTERVIEW_TMPL,
			RUNTIME_CLASS(CSolidSBCGraphPlotterDoc),
			RUNTIME_CLASS(CChildFrame),
			RUNTIME_CLASS(CSolidSBCGraphPlotterView));
		AddDocTemplate(pNewDocTemplate);
	}

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	m_pMainFrame = pMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CSolidSBCResultViewerApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CSolidSBCResultViewerApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CSolidSBCResultViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSolidSBCResultViewerApp message handlers

void CSolidSBCResultViewerApp::OnFileNew()
{
	OpenView( _T("SolidSBCResultSettings") );
}

void CSolidSBCResultViewerApp::OnDatabaseConnected()
{
	OpenView( _T("SolidSBCClientListView") );
}

void CSolidSBCResultViewerApp::OnClientHistoryView(CString strClientUUID)
{
	CDocument* pDoc = OpenView( _T("SolidSBCClientHistoryForm") );
	
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = NULL;
	if ( (pView = pDoc->GetNextView(pos)) )
	{
		((CSolidSBCClientHistoryView*)pView)->SetClientUUID(strClientUUID);
	}
}

void CSolidSBCResultViewerApp::OnMemoryResultView(CString strClientUUID, int nIdentity)
{
	CDocument* pDoc = OpenView( _T("SolidSBCMemoryResultView") );
	
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = NULL;
	if ( (pView = pDoc->GetNextView(pos)) )
	{
		((CSolidSBCMemoryResultView*)pView)->SetClientUUID(strClientUUID,nIdentity);
	}
}

void CSolidSBCResultViewerApp::OnCPUMeasureResultView(CString strClientUUID, int nIdentity)
{
	CDocument* pDoc = OpenView( _T("SolidSBCCPUMeasureResultView") );
	
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = NULL;
	if ( (pView = pDoc->GetNextView(pos)) )
	{
		((CSolidSBCCPUMeasureResultView*)pView)->SetClientUUID(strClientUUID,nIdentity);
	}
}

void CSolidSBCResultViewerApp::OnHDResultView(CString strClientUUID, int nIdentity)
{
	CDocument* pDoc = OpenView( _T("SolidSBCHDResultView") );
	
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = NULL;
	if ( (pView = pDoc->GetNextView(pos)) )
	{
		((CSolidSBCHDResultView*)pView)->SetClientUUID(strClientUUID,nIdentity);
	}
}

void CSolidSBCResultViewerApp::OnPingResultView(CString strClientUUID, int nIdentity)
{
	CDocument* pDoc = OpenView( _T("SolidSBCPingResultView") );
	
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = NULL;
	if ( (pView = pDoc->GetNextView(pos)) )
	{
		((CSolidSBCPingResultView*)pView)->SetClientUUID(strClientUUID,nIdentity);
	}
}

void CSolidSBCResultViewerApp::OnTCPResultView(CString strClientUUID, int nIdentity)
{
	CDocument* pDoc = OpenView( _T("SolidSBCTCPConResultView") );
	
	if (!pDoc)
		return;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView* pView = NULL;
	if ( (pView = pDoc->GetNextView(pos)) )
	{
		((CSolidSBCTCPConResultView*)pView)->SetClientUUID(strClientUUID,nIdentity);
	}
	
}

CDocument* CSolidSBCResultViewerApp::OpenView(CString strViewName)
{
	POSITION DocTmplPos = GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = NULL;
	CString strName = _T("");
	CDocument* pDoc = NULL;

	while( DocTmplPos != NULL ){
		strName = "";
		pDocTemplate = GetNextDocTemplate(DocTmplPos);
		pDocTemplate->GetDocString(strName,CDocTemplate::docName);
		if ( strName == strViewName ){
			pDoc = pDocTemplate->OpenDocumentFile(NULL);
			break;}
		strName = _T("");
		pDocTemplate = NULL;
	}

	return pDoc;
}

CString CSolidSBCResultViewerApp::GetFilePath(void)
{
	TCHAR szFileNamePath[1024];
	GetModuleFileName(NULL,szFileNamePath,1024);
	CString strFileNamePath = szFileNamePath;
	int nPos = strFileNamePath.ReverseFind(_T('\\'));
	CString strPath = strFileNamePath.Left(nPos);
	return strPath;
}

CString CSolidSBCResultViewerApp::GetSettingsFileName(void)
{
	CString strPath = GetFilePath();
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),strPath,SSBC_SRV_CFGFILE);
	return strFileName;
}