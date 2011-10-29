
// SolidSBCCliCfgGUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliCfgGUI.h"
#include "SolidSBCCliCfgGUIDlg.h"

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


// CSolidSBCCliCfgGUIDlg dialog




CSolidSBCCliCfgGUIDlg::CSolidSBCCliCfgGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSolidSBCCliCfgGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSolidSBCCliCfgGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AUTO_RECONNECT_CHECK, m_ctlAutoReconnectCheck);
	DDX_Control(pDX, IDC_SERVER_IPADDRESS, m_ctlServerIP);
	DDX_Control(pDX, IDC_CONFIG_PORT_EDIT, m_ctlServerCfgPort);
	DDX_Control(pDX, IDC_DATA_PORT_EDIT, m_ctlServerDtaPort);
	DDX_Control(pDX, IDC_LOG_RICHEDIT2, m_ctlLogEdit);
	DDX_Control(pDX, IDC_AUTO_START_CHECK, m_ctlAutoStartCheck);
	DDX_Control(pDX, IDC_START_BUTTON, m_ctlStartServiceButton);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_ctlStopServiceButton);
	DDX_Control(pDX, IDC_INSTALL_BUTTON, m_ctlInstallServiceButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_ctlDeleteServiceButton);
	DDX_Control(pDX, IDC_LOG_STATIC, m_ctlLogStatic);
	DDX_Control(pDX, IDC_OPEN_CLIENT_DIR_BUTTON, m_ctlOpenDirButton);
}

BEGIN_MESSAGE_MAP(CSolidSBCCliCfgGUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START_BUTTON, &CSolidSBCCliCfgGUIDlg::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CSolidSBCCliCfgGUIDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_INSTALL_BUTTON, &CSolidSBCCliCfgGUIDlg::OnBnClickedInstallButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CSolidSBCCliCfgGUIDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_APPLY_BUTTON, &CSolidSBCCliCfgGUIDlg::OnBnClickedApplyButton)
	ON_BN_CLICKED(IDC_OPEN_CLIENT_DIR_BUTTON, &CSolidSBCCliCfgGUIDlg::OnBnClickedOpenClientDirButton)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSolidSBCCliCfgGUIDlg message handlers

BOOL CSolidSBCCliCfgGUIDlg::OnInitDialog()
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

	if ( !LoadParameters() )
	{
		CString strMsg;
		strMsg.Format( _T("Could not load parameters from file: %s\r\n\r\nUsing default values..."),GetSettingsFileName());
		AfxMessageBox(strMsg);

		m_ctlAutoReconnectCheck.SetCheck(TRUE);
		m_ctlServerIP.SetWindowText(_T("127.0.0.1"));
		m_ctlServerCfgPort.SetWindowText(_T("52410"));
		m_ctlServerDtaPort.SetWindowText(_T("52411"));
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

void CSolidSBCCliCfgGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSolidSBCCliCfgGUIDlg::OnPaint()
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
HCURSOR CSolidSBCCliCfgGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSolidSBCCliCfgGUIDlg::OnBnClickedStartButton()
{
	StartStopClientService(true);
	MakeButtonStates();
}

void CSolidSBCCliCfgGUIDlg::OnBnClickedStopButton()
{
	StartStopClientService(false);
	MakeButtonStates();
}

void CSolidSBCCliCfgGUIDlg::OnBnClickedInstallButton()
{
	CString strAutoReconnect = _T(""), strAutoStart = _T("");
	if ( m_ctlAutoReconnectCheck.GetCheck() ){
		strAutoReconnect = _T(" /autoreconnect");}

	if ( m_ctlAutoStartCheck.GetCheck() ){
		strAutoStart = _T(" /autostart");}

	CString strServerIP = _T("");
	m_ctlServerIP.GetWindowText(strServerIP);

	CString strInstallString = _T("");
	strInstallString.Format(_T("\"%s\\SolidSBCCliSvc.exe\" /install /remote %s%s%s"),GetFilePath(),strServerIP,strAutoReconnect,strAutoStart);

	USES_CONVERSION;
	system(T2A(strInstallString));

	MakeButtonStates();
}

void CSolidSBCCliCfgGUIDlg::OnBnClickedDeleteButton()
{
	CString strCmd;
	strCmd.Format(_T("\"%s\\SolidSBCCliSvc.exe\" /delete"),GetFilePath());	

	USES_CONVERSION;
	system(T2A(strCmd));

	MakeButtonStates();
}

CString CSolidSBCCliCfgGUIDlg::GetFilePath(void)
{
	int nMaxSize = 1024;
	TCHAR* pszFileNamePath = new TCHAR[nMaxSize];
	ZeroMemory(pszFileNamePath,nMaxSize);
	GetModuleFileName(NULL,pszFileNamePath,nMaxSize);
	
	CString strFileNamePath = _T("");
	strFileNamePath.Format(_T("%s"),pszFileNamePath);
	delete [] pszFileNamePath;
	pszFileNamePath = NULL;

	int nPos = strFileNamePath.ReverseFind(_T('\\'));
	return strFileNamePath.Left(nPos);
}

BOOL CSolidSBCCliCfgGUIDlg::LoadParameters(void)
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
		return FALSE;
	}
	END_CATCH

	m_ctlAutoReconnectCheck.SetCheck(tmpServiceParam.bAutoReconnect);
	m_ctlAutoStartCheck.SetCheck(tmpServiceParam.bAutoStart);
	
	CString strTmp;
	strTmp.Format(_T("%s"),tmpServiceParam.szDataSource);
	m_ctlServerIP.SetWindowText(strTmp);

	strTmp.Format(_T("%d"),tmpServiceParam.dwSrvConfPort);
	m_ctlServerCfgPort.SetWindowText(strTmp);

	strTmp.Format(_T("%d"),tmpServiceParam.dwSrvDataPort);
	m_ctlServerDtaPort.SetWindowText(strTmp);

	return TRUE;
}

CString CSolidSBCCliCfgGUIDlg::GetSettingsFileName(void)
{
	CString strPath = GetFilePath();
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),strPath,SSBC_CLISVC_SETTINGS_FILENAME);
	return strFileName;
}

CString CSolidSBCCliCfgGUIDlg::GetLogFileName(void)
{
	CString strPath = GetFilePath();
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),strPath,SSBC_CLISVC_LOG_FILENAME);
	return strFileName;
}

void CSolidSBCCliCfgGUIDlg::OnBnClickedApplyButton()
{
	SSBC_CLISVC_PARAM tmpServiceParam;
	ZeroMemory(&tmpServiceParam,sizeof(tmpServiceParam));

	tmpServiceParam.bAutoStart      = m_ctlAutoStartCheck.GetCheck();
	tmpServiceParam.bAutoReconnect  = m_ctlAutoReconnectCheck.GetCheck();

	CString strTmp;

	m_ctlServerIP.GetWindowText(strTmp);
	_stprintf_s( (TCHAR*)tmpServiceParam.szDataSource, MAX_SERVER_NAME, strTmp );

	USES_CONVERSION;
	m_ctlServerCfgPort.GetWindowText(strTmp);
	tmpServiceParam.dwSrvConfPort = _ttoi(strTmp);

	m_ctlServerDtaPort.GetWindowText(strTmp);
	tmpServiceParam.dwSrvDataPort = _ttoi(strTmp);
	
	TCHAR szPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szPath);
	CString strFileName = GetSettingsFileName();

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
		_tprintf_s(_T("%s\r\n"),strMsg);
	}
	END_CATCH

	OnBnClickedInstallButton();

	//refesh parameters in memory
	LoadParameters();
}


void CSolidSBCCliCfgGUIDlg::OnTimer(UINT_PTR nIDEvent)
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
			CFile cLogFile( GetLogFileName(), nOpenFlags );
			int nRead = 0;
			while ( (nRead = cLogFile.Read( pszBuffer, nBufferSize - 1 ) ) > 0 ){
				pszBuffer[nRead] = 0;
				strFileContent += A2T(pszBuffer);
				ZeroMemory(pszBuffer,nBufferSize);
			}
			cLogFile.Close( );
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

			m_strLogFileContent = strFileContent;
			m_ctlLogEdit.SetWindowText(m_strLogFileContent);

			int nBegin = m_ctlLogEdit.GetTextLength();
			m_ctlLogEdit.SetSel(nBegin, nBegin);   // Select last character
			m_ctlLogEdit.SetFocus();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CSolidSBCCliCfgGUIDlg::DestroyWindow()
{
	KillTimer(m_nTimer);

	return CDialog::DestroyWindow();
}

void CSolidSBCCliCfgGUIDlg::OnBnClickedOpenClientDirButton()
{
	TCHAR szFileNamePath[1024];
	GetModuleFileName(NULL,szFileNamePath,1024);
	CString strFileNamePath = szFileNamePath;
	int nPos = strFileNamePath.ReverseFind(_T('\\'));
	CString strPath = strFileNamePath.Left(nPos);

	CString strCmd;
	strCmd.Format(_T("explorer %s"),strPath);
	USES_CONVERSION;
	system(T2A(strCmd));
}


bool CSolidSBCCliCfgGUIDlg::IsServiceInstalled(void)
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

void CSolidSBCCliCfgGUIDlg::MakeButtonStates(void)
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

bool CSolidSBCCliCfgGUIDlg::IsServiceRunning(void)
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
        SSBC_CLISVC_NAME,					// name of service 
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

bool CSolidSBCCliCfgGUIDlg::StartStopClientService(bool bStart)
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
        SSBC_CLISVC_NAME,					// name of service 
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

void CSolidSBCCliCfgGUIDlg::OnCancel()
{
	OnOK();
}

void CSolidSBCCliCfgGUIDlg::OnClose()
{
	OnOK();
}

void CSolidSBCCliCfgGUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	if ( m_ctlLogEdit.m_hWnd && (nType == SIZE_RESTORED) ){

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
		m_ctlOpenDirButton.SetWindowPos(NULL,wndCtrlRect.left,wndCtrlRect.top,(wndCtrlRect.right - wndCtrlRect.left),(wndCtrlRect.bottom - wndCtrlRect.top),0);
		
		m_nOldX = cx;
		m_nOldY = cy;
	}
}

bool CSolidSBCCliCfgGUIDlg::ExecuteCmd(const CString& sBinaryPathName, const CString& sCommandline)
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
        result = ::CreateProcess(sBinaryPathName, tempCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
    }
    else
    {
        result = ::CreateProcess(sBinaryPathName, tempCmdLine, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation);
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