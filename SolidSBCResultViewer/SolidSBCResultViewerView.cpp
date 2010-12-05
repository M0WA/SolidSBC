
// SolidSBCResultViewerView.cpp : implementation of the CSolidSBCResultViewerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SolidSBCResultViewer.h"
#endif

#include "SolidSBCResultViewerDoc.h"
#include "SolidSBCResultViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolidSBCResultViewerView

IMPLEMENT_DYNCREATE(CSolidSBCResultViewerView, CFormView)

BEGIN_MESSAGE_MAP(CSolidSBCResultViewerView, CFormView)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CSolidSBCResultViewerView::OnBnClickedConnectButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CSolidSBCResultViewerView::OnBnClickedSaveButton)
END_MESSAGE_MAP()

// CSolidSBCResultViewerView construction/destruction

CSolidSBCResultViewerView::CSolidSBCResultViewerView()
	: CFormView(CSolidSBCResultViewerView::IDD)
{
	// TODO: add construction code here

}

CSolidSBCResultViewerView::~CSolidSBCResultViewerView()
{
}

void CSolidSBCResultViewerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DB_IPADDRESS, m_ctlDbIP);
	DDX_Control(pDX, IDC_DBPORT_EDIT, m_ctlPortEdit);
	DDX_Control(pDX, IDC_DBNAME_EDIT, m_ctlDbNameEdit);
	DDX_Control(pDX, IDC_DB_USER_EDIT, m_ctlDbUserEdit);
	DDX_Control(pDX, IDC_DBPASS_EDIT, m_ctlDbPassEdit);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_ctlConnectButton);
}

BOOL CSolidSBCResultViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSolidSBCResultViewerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
			
	CSolidSBCConfigFile cConfigFile;
	if ( !cConfigFile.Init(theApp.GetSettingsFileName()) ){
		m_ctlDbIP.SetWindowText(_T("127.0.0.1"));
		m_ctlPortEdit.SetWindowText(_T("3306"));
		m_ctlDbNameEdit.SetWindowText(_T("db name"));
		m_ctlDbUserEdit.SetWindowText(_T("db user"));
		m_ctlDbPassEdit.SetWindowText(_T("db pass"));
	}

	CString strType = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_TYPE, &strType );

	CString strHost = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_HOST, &strHost );
	m_ctlDbIP.SetWindowText(strHost);

	CString strPort = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_PORT, &strPort );
	m_ctlPortEdit.SetWindowText(strPort);

	CString strName = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_NAME, &strName );
	m_ctlDbNameEdit.SetWindowText(strName);

	CString strUser = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_USER, &strUser );
	m_ctlDbUserEdit.SetWindowText(strUser);
	
	CString strPass = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_PASS, &strPass );
	m_ctlDbPassEdit.SetWindowText(strPass);
}


// CSolidSBCResultViewerView diagnostics

#ifdef _DEBUG
void CSolidSBCResultViewerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSolidSBCResultViewerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSolidSBCResultViewerDoc* CSolidSBCResultViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSolidSBCResultViewerDoc)));
	return (CSolidSBCResultViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSolidSBCResultViewerView message handlers

void CSolidSBCResultViewerView::OnBnClickedConnectButton()
{
	CString strButtonText = _T("");
	m_ctlConnectButton.GetWindowText(strButtonText);

	if ( strButtonText == _T("Connect") ){
		
		CString strServer = _T(""), strPort = _T(""), 
			strDbName = _T(""), strDbUser = _T(""),	strDbPass = _T("");

		m_ctlDbIP.      GetWindowText(strServer);
		m_ctlPortEdit.  GetWindowText(strPort);
		m_ctlDbNameEdit.GetWindowText(strDbName);
		m_ctlDbUserEdit.GetWindowText(strDbUser);
		m_ctlDbPassEdit.GetWindowText(strDbPass);

		theApp.m_pMainFrame->SetStatusBar(_T("connecting to database..."));

		if ( g_cDatabaseConnection.Connect(strServer,strDbUser,strDbPass,strDbName,_ttoi(strPort)) ) {

			theApp.m_pMainFrame->SetStatusBar(_T("connected to database. initialising client list..."));

			m_ctlConnectButton.SetWindowText( _T("Disconnect") );
			CMDIChildWnd * pChild = ((CMDIFrameWnd*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
			pChild->ShowWindow(SW_MINIMIZE);
			
			theApp.OnDatabaseConnected();

			theApp.m_pMainFrame->SetStatusBar(_T("Ready"));
			
		} else {

			theApp.m_pMainFrame->SetStatusBar(_T("could not connect to database"));

		}

	} else if ( strButtonText == _T("Disconnect") ){

		g_cDatabaseConnection.Disconnect();
		m_ctlConnectButton.SetWindowText( _T("Connect") );

	}
}


void CSolidSBCResultViewerView::OnBnClickedSaveButton()
{
	// TODO: Add your control notification handler code here
}
