
// SolidSBCSrvCfgGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCSrvCfgGUI.h"
#include "SolidSBCSrvCfgGUIDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSolidSBCSrvCfgGUIDlg dialog




CSolidSBCSrvCfgGUIDlg::CSolidSBCSrvCfgGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSolidSBCSrvCfgGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSolidSBCSrvCfgGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_RICHEDIT2, m_ctlLogEdit);
	DDX_Control(pDX, IDC_DB_TYPE_COMBO, m_ctlDbTypeCombo);
	DDX_Control(pDX, IDC_DB_IPADDRESS, m_ctlDBIp);
	DDX_Control(pDX, IDC_DB_PORT_EDIT, m_ctlDBPortEdit);
	DDX_Control(pDX, IDC_DB_NAME_EDIT, m_ctlDBNameEdit);
	DDX_Control(pDX, IDC_DB_USER_EDIT, m_ctlDBUserEdit);
	DDX_Control(pDX, IDC_DB_PASS_EDIT, m_ctlDBPassEdit);
	DDX_Control(pDX, IDC_INSTALL_BUTTON, m_ctlInstallServiceButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_ctlDeleteServiceButton);
	DDX_Control(pDX, IDC_START_BUTTON, m_ctlStartServiceButton);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_ctlStopServiceButton);
	DDX_Control(pDX, IDC_OPEN_SERVER_DIR_BUTTON, m_ctlOpenDirButton);
	DDX_Control(pDX, IDC_EDIT_CONFIG_BUTTON, m_ctlEditConfigButton);
	DDX_Control(pDX, IDC_LOG_STATIC, m_ctlLogStatic);
}

BEGIN_MESSAGE_MAP(CSolidSBCSrvCfgGUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_INSTALL_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedInstallButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedDeleteButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPEN_SERVER_DIR_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedOpenServerDirButton)
	ON_BN_CLICKED(IDC_APPLY_SETTINGS_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedApplySettingsButton)
	ON_BN_CLICKED(IDC_EDIT_CONFIG_BUTTON, &CSolidSBCSrvCfgGUIDlg::OnBnClickedEditConfigButton)
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSolidSBCSrvCfgGUIDlg message handlers

BOOL CSolidSBCSrvCfgGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	
	m_ctlDbTypeCombo.InsertString(0,_T("MySQL"));
	m_ctlDbTypeCombo.SetCurSel(0);

	if ( !LoadParameters() )
	{
		m_ctlDBIp.SetWindowText(_T("127.0.0.1"));
		m_ctlDBPortEdit.SetWindowText(_T("3306"));
		m_ctlDBNameEdit.SetWindowText(_T("ssbc"));
		m_ctlDBUserEdit.SetWindowText(_T("ssbc_user"));
		m_ctlDBPassEdit.SetWindowText(_T("ssbc_pass"));
	} else {
		CString strOldTitle;
		GetWindowText(strOldTitle);
		CString strNewTitle;
		strNewTitle.Format(_T("%s - %s"),strOldTitle,GetSettingsFileName());
		SetWindowText(strNewTitle);
	}
	
	RECT wndRect;
	GetClientRect(&wndRect);
	m_nOldX = wndRect.right  - wndRect.left;
	m_nOldY = wndRect.bottom - wndRect.top;

	m_ctlLogStatic.GetWindowRect(&m_rectLogStatic);
	ScreenToClient(&m_rectLogStatic);
	
	m_ctlLogEdit.GetWindowRect(&m_rectLogEdit);
	ScreenToClient(&m_rectLogEdit);

	MakeButtonStates();
	m_nTimer = SetTimer(0xFEFE, 1000, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

CString CSolidSBCSrvCfgGUIDlg::GetSettingsFileName(void)
{
	CString strPath = GetFilePath();
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),strPath,SSBC_SRV_CFGFILE);
	return strFileName;
}

void CSolidSBCSrvCfgGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSolidSBCSrvCfgGUIDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSolidSBCSrvCfgGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedStartButton()
{
	StartStopClientService(true);
	MakeButtonStates();
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedStopButton()
{
	StartStopClientService(false);
	MakeButtonStates();
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedInstallButton()
{
	/*
	CString sBinary;
	sBinary.Format(_T("%s\\SolidSBCSrvSvc.exe"),GetFilePath());
	ExecuteCmd(sBinary,_T("/install"));
	*/

	USES_CONVERSION;
	CString strCmd;
	strCmd.Format(_T("\"%s\\SolidSBCSrvSvc.exe\" /install"),GetFilePath());
	system(T2A(strCmd));
	MakeButtonStates();
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedDeleteButton()
{
	USES_CONVERSION;
	CString strCmd;
	strCmd.Format(_T("\"%s\\SolidSBCSrvSvc.exe\" /delete"),GetFilePath());
	system(T2A(strCmd));
	
	MakeButtonStates();
}

void CSolidSBCSrvCfgGUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent == 0xFEFE ){
		
		USES_CONVERSION;
		int nOpenFlags = ( CFile::modeRead );
		int nBufferSize = 512;
		char* pszBuffer = new char[nBufferSize];
		ZeroMemory(pszBuffer,nBufferSize);

		CString strFileContent;

		m_cTimerMutex.Lock();
		TRY
		{
			CFile cServiceParameterFile( GetLogFileName(), nOpenFlags );
			int nRead = 0;
			while ( (nRead = cServiceParameterFile.Read( pszBuffer, nBufferSize - 1 ) ) ){
				pszBuffer[nRead] = 0;
				strFileContent += A2T(pszBuffer);
				ZeroMemory(pszBuffer,nBufferSize);
			}
			cServiceParameterFile.Close( );
		}
		CATCH(CException, pEx)
		{
		}
		END_CATCH
		m_cTimerMutex.Unlock();

		delete [] pszBuffer;
		pszBuffer = NULL;

		if ( m_strLogFileContent != strFileContent){
			
			//max length of displayed log
			if ( strFileContent.GetLength() > 350000)
				m_strLogFileContent = m_strLogFileContent.Right(350000);
			else
				m_strLogFileContent = strFileContent;

			m_ctlLogEdit.SetWindowText(m_strLogFileContent);

			int nBegin = m_ctlLogEdit.GetTextLength();
			m_ctlLogEdit.SetSel(nBegin, nBegin);   // Select last character
			m_ctlLogEdit.SetFocus();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

CString CSolidSBCSrvCfgGUIDlg::GetLogFileName(void)
{
	CString strPath = GetFilePath();
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),strPath,SSBC_SRVSVC_LOG_FILENAME);
	return strFileName;
}

BOOL CSolidSBCSrvCfgGUIDlg::DestroyWindow()
{
	KillTimer(m_nTimer);
	return CDialog::DestroyWindow();
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedOpenServerDirButton()
{
	CString strPath = GetFilePath();

	CString strCmd;
	strCmd.Format(_T("explorer \"%s\""),strPath);
	USES_CONVERSION;
	system(T2A(strCmd));
}

BOOL CSolidSBCSrvCfgGUIDlg::LoadParameters(void)
{	
	CSolidSBCConfigFile cConfigFile;
	if ( !cConfigFile.Init(GetSettingsFileName()) )
		return FALSE;

	CString strType = _T("");
	cConfigFile.GetParamStr( SSBC_DATABASE_TYPE, &strType );

	CString strHost = _T("");
	if ( cConfigFile.GetParamStr( SSBC_DATABASE_HOST, &strHost ) )
		return FALSE;
	m_ctlDBIp.SetWindowText(strHost);

	CString strPort = _T("");
	if ( cConfigFile.GetParamStr( SSBC_DATABASE_PORT, &strPort ) )
		return FALSE;
	m_ctlDBPortEdit.SetWindowText(strPort);

	CString strName = _T("");
	if ( cConfigFile.GetParamStr( SSBC_DATABASE_NAME, &strName ) )
		return FALSE;
	m_ctlDBNameEdit.SetWindowText(strName);

	CString strUser = _T("");
	if ( cConfigFile.GetParamStr( SSBC_DATABASE_USER, &strUser ) )
		return FALSE;
	m_ctlDBUserEdit.SetWindowText(strUser);
	
	CString strPass = _T("");
	if ( cConfigFile.GetParamStr( SSBC_DATABASE_PASS, &strPass ) )
		return FALSE;
	m_ctlDBPassEdit.SetWindowText(strPass);

	return TRUE;
}

CString CSolidSBCSrvCfgGUIDlg::GetFilePath(void)
{
	TCHAR szFileNamePath[1024];
	GetModuleFileName(NULL,szFileNamePath,1024);
	CString strFileNamePath = szFileNamePath;
	int nPos = strFileNamePath.ReverseFind(_T('\\'));
	CString strPath = strFileNamePath.Left(nPos);
	return strPath;
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedEditConfigButton()
{
	CString strCmd;
	strCmd.Format(_T("notepad \"%s\""),GetSettingsFileName());
	USES_CONVERSION;
	system(T2A(strCmd));
}

bool CSolidSBCSrvCfgGUIDlg::IsServiceInstalled(void)
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

void CSolidSBCSrvCfgGUIDlg::MakeButtonStates(void)
{
	if ( IsServiceInstalled() ){

		m_ctlInstallServiceButton.EnableWindow(FALSE);
		m_ctlDeleteServiceButton.EnableWindow(TRUE);
		
		if ( IsServiceRunning() ){

			m_ctlStartServiceButton.EnableWindow(FALSE);
			m_ctlStopServiceButton.EnableWindow(TRUE);

		} else {

			m_ctlStartServiceButton.EnableWindow(TRUE);
			m_ctlStopServiceButton.EnableWindow(FALSE);

		}

	} else {

		m_ctlInstallServiceButton.EnableWindow(TRUE);
		m_ctlDeleteServiceButton.EnableWindow(FALSE);
		
		m_ctlStartServiceButton.EnableWindow(FALSE);
		m_ctlStopServiceButton.EnableWindow(FALSE);

	}
}

bool CSolidSBCSrvCfgGUIDlg::IsServiceRunning(void)
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

	SERVICE_STATUS svcstat;
	ZeroMemory(&svcstat,sizeof(SERVICE_STATUS));
	if ( !QueryServiceStatus(schService,&svcstat) ){
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

    CloseServiceHandle(schService); 
    CloseServiceHandle(schSCManager);

	if ( ( svcstat.dwCurrentState == SERVICE_RUNNING ) || ( svcstat.dwCurrentState == SERVICE_START_PENDING ) ){
		return TRUE;
	} else {
		return FALSE;
	}
}

void CSolidSBCSrvCfgGUIDlg::OnCancel()
{
	OnOK();
}

void CSolidSBCSrvCfgGUIDlg::OnClose()
{
	OnOK();
}

void CSolidSBCSrvCfgGUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_ctlLogEdit.m_hWnd && (nType == SIZE_RESTORED) ){

		int nNewWidth = 0, nNewHeight = 0;

		int nXDiff = cx - m_nOldX; //positive if bigger
		int nYDiff = cy - m_nOldY; //positive if bigger
	
		m_rectLogStatic.right  += nXDiff;
		m_rectLogStatic.bottom += nYDiff;
		nNewWidth  = (m_rectLogStatic.right  - m_rectLogStatic.left);
		nNewHeight = (m_rectLogStatic.bottom - m_rectLogStatic.top );
		m_ctlLogStatic.SetWindowPos(NULL,m_rectLogStatic.left, m_rectLogStatic.top, nNewWidth, nNewHeight,0);

		m_rectLogEdit.right  += nXDiff;
		m_rectLogEdit.bottom += nYDiff;
		nNewWidth  = (m_rectLogEdit.right  - m_rectLogEdit.left);
		nNewHeight = (m_rectLogEdit.bottom - m_rectLogEdit.top );
		m_ctlLogEdit.SetWindowPos(NULL,m_rectLogEdit.left,m_rectLogEdit.top, nNewWidth, nNewHeight,0);
		
		RECT wndCtrlRect;
		m_ctlOpenDirButton.GetWindowRect(&wndCtrlRect);
		ScreenToClient(&wndCtrlRect);
		wndCtrlRect.top    += nYDiff;
		wndCtrlRect.bottom += nYDiff;
		nNewWidth  = (wndCtrlRect.right  - wndCtrlRect.left);
		nNewHeight = (wndCtrlRect.bottom - wndCtrlRect.top );
		m_ctlOpenDirButton.SetWindowPos(NULL,wndCtrlRect.left,wndCtrlRect.top, nNewWidth, nNewHeight,0);
		
		m_ctlEditConfigButton.GetWindowRect(&wndCtrlRect);
		ScreenToClient(&wndCtrlRect);
		wndCtrlRect.top    += nYDiff;
		wndCtrlRect.bottom += nYDiff;
		nNewWidth  = (wndCtrlRect.right  - wndCtrlRect.left);
		nNewHeight = (wndCtrlRect.bottom - wndCtrlRect.top );
		m_ctlEditConfigButton.SetWindowPos(NULL,wndCtrlRect.left,wndCtrlRect.top, nNewWidth, nNewHeight,0);
		
		m_ctlProfileEditorButton.GetWindowRect(&wndCtrlRect);
		ScreenToClient(&wndCtrlRect);
		wndCtrlRect.top    += nYDiff;
		wndCtrlRect.bottom += nYDiff;
		nNewWidth  = (wndCtrlRect.right  - wndCtrlRect.left);
		nNewHeight = (wndCtrlRect.bottom - wndCtrlRect.top );
		m_ctlProfileEditorButton.SetWindowPos(NULL,wndCtrlRect.left,wndCtrlRect.top, nNewWidth, nNewHeight,0);

		m_nOldX = cx;
		m_nOldY = cy;
	}
}

void CSolidSBCSrvCfgGUIDlg::OnBnClickedApplySettingsButton()
{
}



bool CSolidSBCSrvCfgGUIDlg::StartStopClientService(bool bStart)
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
        return false;
    }

    // Get a handle to the service.
    schService = OpenService( 
        schSCManager,						// SCM database 
        SSBC_SRVSVC_NAME,					// name of service 
        SERVICE_ALL_ACCESS);				// full access 
 
    if (schService == NULL)
    { 
        CloseServiceHandle(schSCManager);
        return false;
    }

	if(bStart)
	{
		return !StartService(schService, 0, 0);
	}
	else
	{
		SERVICE_STATUS ss;
  		if( !::ControlService( schService, SERVICE_CONTROL_STOP, &ss ) )
  		{
			return false;
  		}
	}

	return true;
}

bool CSolidSBCSrvCfgGUIDlg::ExecuteCmd(const CString& sBinaryPathName, const CString& sCommandline)
{
	PROCESS_INFORMATION processInformation;
    STARTUPINFO startupInfo;
    memset(&processInformation, 0, sizeof(processInformation));
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);

	BOOL result;
	TCHAR tempCmdLine[MAX_PATH * 2];  //Needed since CreateProcessW may change the contents of CmdLine
	memset(tempCmdLine,0,sizeof(TCHAR)*MAX_PATH * 2);
    if (sCommandline != "")
    {
        _tcscpy_s(tempCmdLine, MAX_PATH *2, sCommandline);
        result = ::CreateProcess(sBinaryPathName, tempCmdLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation);
    }
    else
    {
        result = ::CreateProcess(sBinaryPathName, tempCmdLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation);
    }

	if (result == 0)
    {
       return false;
    }
    else
    {
        WaitForSingleObject( processInformation.hProcess, INFINITE );
        CloseHandle( processInformation.hProcess );
        CloseHandle( processInformation.hThread );
    }

	return result ? true : false;
}