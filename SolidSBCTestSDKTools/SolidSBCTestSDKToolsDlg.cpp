
// SolidSBCTestSDKToolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCTestSDKTools.h"
#include "SolidSBCTestSDKToolsDlg.h"
#include "afxdialogex.h"
#include "XMLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CSolidSBCTestSDKToolsDlg dialog




CSolidSBCTestSDKToolsDlg::CSolidSBCTestSDKToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSolidSBCTestSDKToolsDlg::IDD, pParent)
	, m_hTestLibrary(NULL)
	, m_pDatabase(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSolidSBCTestSDKToolsDlg::~CSolidSBCTestSDKToolsDlg()
{
	UnloadTestLibrary();

	if(m_pDatabase)
	{
		m_pDatabase->Disconnect();
		delete m_pDatabase;
		m_pDatabase = NULL;
	}
}

void CSolidSBCTestSDKToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLLFILE_EDIT, m_cDllFileName);
	DDX_Control(pDX, IDC_TEST_LIST, m_ctlTestList);
	DDX_Control(pDX, IDC_GENERATE_EMPTY_CONFIG_XML_BUTTON, m_ctlGenerateConfigButton);
	DDX_Control(pDX, IDC_START_STOP_BUTTON, m_ctlStartStopButton);
	DDX_Control(pDX, IDC_GENERATE_STUCTURE_BUTTON, m_ctlGenerateDBStructure);
	DDX_Control(pDX, IDC_HOST_IPADDRESS, m_ctlDbHostIp);
	DDX_Control(pDX, IDC_PORT_EDIT, m_ctlDbPort);
	DDX_Control(pDX, IDC_DATABASE_EDIT, m_ctlDbDatabase);
	DDX_Control(pDX, IDC_USER_EDIT, m_ctlDbUser);
	DDX_Control(pDX, IDC_PASS_EDIT, m_ctlDbPass);
	DDX_Control(pDX, IDC_CONNECT_DATABASE_BUTTON, m_ctlConnectButton);
	DDX_Control(pDX, IDC_CREATE_DATABASE_BUTTON, m_ctlCreateDatabaseButton);
}

BEGIN_MESSAGE_MAP(CSolidSBCTestSDKToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE_DLLFILE_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedBrowseDllfileButton)
	ON_BN_CLICKED(IDC_LOAD_DLLFILE_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedLoadDllfileButton)
	ON_BN_CLICKED(IDC_GENERATE_EMPTY_CONFIG_XML_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedGenerateEmptyConfigXmlButton)
	ON_BN_CLICKED(IDC_START_STOP_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedStartStopButton)
	ON_LBN_SELCHANGE(IDC_TEST_LIST, &CSolidSBCTestSDKToolsDlg::OnLbnSelchangeTestList)
	ON_BN_CLICKED(IDC_GENERATE_STUCTURE_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedGenerateStuctureButton)
	ON_BN_CLICKED(IDC_CREATE_DATABASE_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedCreateDatabaseButton)
	ON_BN_CLICKED(IDC_CONNECT_DATABASE_BUTTON, &CSolidSBCTestSDKToolsDlg::OnBnClickedConnectDatabaseButton)
END_MESSAGE_MAP()


// CSolidSBCTestSDKToolsDlg message handlers

BOOL CSolidSBCTestSDKToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctlGenerateConfigButton.EnableWindow(FALSE);
	m_ctlStartStopButton.EnableWindow(FALSE);
	m_ctlGenerateDBStructure.EnableWindow(FALSE);

	InitDatabase(SSBC_DB_TYPE_MYSQL);
	
	m_ctlDbHostIp.SetWindowText(_T("127.0.0.1"));
	m_ctlDbPort.SetWindowText(_T("3306"));
	m_ctlDbDatabase.SetWindowText(_T("SolidSBC"));
	m_ctlDbUser.SetWindowText(_T("root"));
	m_ctlDbPass.SetWindowText(_T(""));

	m_ctlCreateDatabaseButton.EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSolidSBCTestSDKToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSolidSBCTestSDKToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSolidSBCTestSDKToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedBrowseDllfileButton()
{
	CFileDialog cOpenFileDlg( TRUE, _T("dll"), _T( "*.dll"), OFN_FILEMUSTEXIST);
	if ( cOpenFileDlg.DoModal() == IDOK )
	{
		UpdateData(false);
		m_cDllFileName.SetWindowText(cOpenFileDlg.GetPathName());
		UpdateData();
		OnBnClickedLoadDllfileButton();
	}
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedLoadDllfileButton()
{
	UnloadTestLibrary();

	std::vector<std::string> vecTestNames;
	CSolidSBCTestManager* pManager = GetTestManager();
	if ( pManager )
		pManager->GetTestNames(vecTestNames);
	
	//refill list
	m_ctlTestList.ResetContent();
	std::vector<std::string>::iterator iIter = vecTestNames.begin();
	for(; iIter != vecTestNames.end(); iIter++)
		m_ctlTestList.AddString(CString((*iIter).c_str()));

	if(m_pDatabase && m_ctlTestList.GetCount())
		m_ctlGenerateDBStructure.EnableWindow(TRUE);
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedGenerateEmptyConfigXmlButton()
{
	USES_CONVERSION;
	CString strTestName = _T("");
	m_ctlTestList.GetText(m_ctlTestList.GetCurSel(),strTestName);

	//get the xml
	CSolidSBCTestManager* pManager = GetTestManager();
	if (!pManager)
		return;

	CSolidSBCTestConfig* pConfig = pManager->GetTestConfigByName(T2A(strTestName));
	if ( !pConfig )
		return;

	CString strXML = pConfig->GenerateXML();
	CXMLDlg xmlDlg(strXML);
	xmlDlg.DoModal();
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedStartStopButton()
{
	USES_CONVERSION;
	CString strTestName = _T("");
	m_ctlTestList.GetText(m_ctlTestList.GetCurSel(),strTestName);

	CSolidSBCTestManager* pManager = GetTestManager();
	if ( !pManager )
		return;

	CSolidSBCTestConfig*   pConfig = pConfig=pManager->GetTestConfigByName(T2A(strTestName));
	if ( !pConfig )
		return;

	std::string sXML = T2A(pConfig->GenerateXML());
	pManager->StartTest(sXML);
}

void CSolidSBCTestSDKToolsDlg::OnLbnSelchangeTestList()
{
	m_ctlGenerateConfigButton.EnableWindow(TRUE);
	m_ctlStartStopButton.EnableWindow(TRUE);
	m_ctlGenerateDBStructure.EnableWindow(TRUE);
}

bool CSolidSBCTestSDKToolsDlg::LoadTestLibrary()
{
	CString sDllFilename;
	m_cDllFileName.GetWindowText(sDllFilename);
	m_hTestLibrary = LoadLibrary(sDllFilename);

	if(!m_hTestLibrary)
		return false;
	return true;
}

CSolidSBCTestManager* CSolidSBCTestSDKToolsDlg::GetTestManager()
{
	if ( !m_hTestLibrary && !LoadTestLibrary() )
		return NULL;

	typedef const void* (*PINSTANCE_GETTER_FUNC) (void);
	PINSTANCE_GETTER_FUNC GetSolidSBCTestInstanceFunc = (PINSTANCE_GETTER_FUNC)
		GetProcAddress(m_hTestLibrary,"GetSolidSBCTestInstance");

	if ( !GetSolidSBCTestInstanceFunc )
	{
		CString strMessage;
		strMessage.Format(_T("Could not load test library: %d"),GetLastError());
		AfxMessageBox(strMessage);
		
		UnloadTestLibrary();
		return NULL;
	}

	try
	{
		return (CSolidSBCTestManager*)GetSolidSBCTestInstanceFunc();
	}
	catch(...)
	{
		AfxMessageBox(_T("Test library does not export a valid CSolidSBCTestInstance"));
		UnloadTestLibrary();
		return NULL;
	}
}

bool CSolidSBCTestSDKToolsDlg::GetTestNames(std::vector<CString>& vecNames)
{
	if(!m_hTestLibrary)
		return false;

	std::vector<std::string> vecTestNames;
	CSolidSBCTestManager* pManager = GetTestManager();
	if( !pManager )
		return false;

	pManager->GetTestNames(vecTestNames);

	if( !vecTestNames.size() ) {
		AfxMessageBox(_T("Test library does not export tests"));
		return false; }
	return true;
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedGenerateStuctureButton()
{
	USES_CONVERSION;
	CSolidSBCTestManager* pManager = GetTestManager();
	if( !pManager )
		return;
	
	std::string sCreateTableStatements = "";
	pManager->GetCreateTableStatements(sCreateTableStatements);
	if(m_pDatabase)
	{
		if ( AfxMessageBox(_T("!!! This will delete all test-data in your database !!!\nContinue ?"), MB_YESNO) != IDYES )
			return;
		m_pDatabase->ExecStmts(sCreateTableStatements,true);
	}
	else
	{
#ifdef _DEBUG
		AfxMessageBox(_T("Please connect to database first..."));
#endif
	}
}

bool CSolidSBCTestSDKToolsDlg::UnloadTestLibrary()
{
	if(!m_hTestLibrary)
		return false;
	FreeLibrary(m_hTestLibrary);
	m_hTestLibrary = NULL;
	return true;
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedCreateDatabaseButton()
{
	if(m_pDatabase)
	{	
		if ( AfxMessageBox(_T("!!! This will delete all data in your database !!!\nContinue ?"), MB_YESNO) != IDYES )
			return;

		CreateDatabase();
		CreateGetClientNameProcedure();
		CreateViews();
	}
	else
	{
#ifdef _DEBUG
		AfxMessageBox(_T("Please connect to database first...\n This message should not be shown, because button should be disabled when not connected..."));
#endif
	}
}

void CSolidSBCTestSDKToolsDlg::CreateViews(void)
{
	HRSRC hres = FindResource(NULL, MAKEINTRESOURCE(IDR_CREATE_VIEWS_SQL), _T("SQL_FILES"));
	if(!hres) //does not happen normally ;)
		return;

	HGLOBAL hbytes = LoadResource(NULL, hres);
	if(!hbytes)
		return;

	LPVOID pdata = LockResource(hbytes);
	if(!pdata)
		return;

	LPCSTR sXml = (LPCSTR)pdata;
	std::string sGetCreateViewsSql = sXml;

	m_pDatabase->ExecStmts(sGetCreateViewsSql,true);

	UnlockResource(hbytes);
	FreeResource(hres);
}

void CSolidSBCTestSDKToolsDlg::CreateGetClientNameProcedure(void)
{
	HRSRC hres = FindResource(NULL, MAKEINTRESOURCE(IDR_CREATE_GETCLIENTNAME_PROCEDURE_SQL), _T("SQL_FILES"));
	if(!hres) //does not happen normally ;)
		return;

	HGLOBAL hbytes = LoadResource(NULL, hres);
	if(!hbytes)
		return;

	LPVOID pdata = LockResource(hbytes);
	if(!pdata)
		return;

	LPCSTR sXml = (LPCSTR)pdata;
	std::string sGetClientNameSql = sXml;

	m_pDatabase->ExecStmts(sGetClientNameSql,false);

	UnlockResource(hbytes);
	FreeResource(hres);
}

void CSolidSBCTestSDKToolsDlg::CreateDatabase(void)
{
	HRSRC hres = FindResource(NULL, MAKEINTRESOURCE(IDR_CREATE_DATABASE_SQL), _T("SQL_FILES"));
	if(!hres) //does not happen normally ;)
		return;

	HGLOBAL hbytes = LoadResource(NULL, hres);
	if(!hbytes)
		return;

	LPVOID pdata = LockResource(hbytes);
	if(!pdata)
		return;

	LPCSTR sXml = (LPCSTR)pdata;
	std::string sCreateDatabaseSql = sXml;

	m_pDatabase->ExecStmts(sCreateDatabaseSql,true);

	UnlockResource(hbytes);
	FreeResource(hres);
}

void CSolidSBCTestSDKToolsDlg::OnBnClickedConnectDatabaseButton()
{
	CString strHost = _T("");
	m_ctlDbHostIp.GetWindowText(strHost);
	
	CString strPort = _T("");
	m_ctlDbPort.GetWindowText(strPort);
	unsigned short nPort = _ttoi(strPort);

	CString strDatabaseName = _T("");
	m_ctlDbDatabase.GetWindowText(strDatabaseName);

	CString strUser = _T("");
	m_ctlDbUser.GetWindowText(strUser);

	CString strPass = _T("");
	m_ctlDbPass.GetWindowText(strPass);

	CString strButtonText = _T("");
	m_ctlConnectButton.GetWindowText(strButtonText);
	
	m_pDatabase = GetDatabaseConnection();

	if(strButtonText == _T("Connect"))
	{
		m_pDatabase->Disconnect();
		m_pDatabase->SetConfig(strHost, nPort, strDatabaseName, strUser, strPass);
		if(!m_pDatabase->Connect())
		{
			AfxMessageBox(_T("error while connecting to database"));
			m_pDatabase->Disconnect();
			m_pDatabase = 0;
			m_ctlCreateDatabaseButton.EnableWindow(FALSE);
			m_ctlGenerateDBStructure.EnableWindow(FALSE);
		}
		else
		{
			m_ctlConnectButton.SetWindowText(_T("Disconnect"));
			m_ctlCreateDatabaseButton.EnableWindow(TRUE);
			if(m_ctlTestList.GetCount())
				m_ctlGenerateDBStructure.EnableWindow(TRUE);
		}
	}
	else
	{
			m_pDatabase->Disconnect();
			m_pDatabase = 0;
			m_ctlCreateDatabaseButton.EnableWindow(FALSE);
			m_ctlGenerateDBStructure.EnableWindow(FALSE);
			m_ctlConnectButton.SetWindowText(_T("Connect"));
	}
}
