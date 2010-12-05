// ProfileEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCProfileEditorDlg.h"


// CSolidSBCProfileEditorDlg dialog

IMPLEMENT_DYNAMIC(CSolidSBCProfileEditorDlg, CDialogEx)

CSolidSBCProfileEditorDlg::CSolidSBCProfileEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSolidSBCProfileEditorDlg::IDD, pParent)
{

}

CSolidSBCProfileEditorDlg::~CSolidSBCProfileEditorDlg()
{
}

void CSolidSBCProfileEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROFILE_COMBO, m_ctlProfileCombo);
	DDX_Control(pDX, IDC_PROFILE_LIST, m_cProfileListCtrl);
	DDX_Control(pDX, IDC_SAVE_CHANGES_BUTTON, m_ctlApplyButton);
}


BEGIN_MESSAGE_MAP(CSolidSBCProfileEditorDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_PROFILE_COMBO, &CSolidSBCProfileEditorDlg::OnCbnSelchangeProfileCombo)
	ON_BN_CLICKED(IDC_SAVE_CHANGES_BUTTON, &CSolidSBCProfileEditorDlg::OnBnClickedSaveChangesButton)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSolidSBCProfileEditorDlg message handlers


void CSolidSBCProfileEditorDlg::OnCbnSelchangeProfileCombo()
{
	CString strProfile = _T("");
	m_ctlProfileCombo.GetLBText(m_ctlProfileCombo.GetCurSel(),strProfile);
	m_cProfileListCtrl.SetActiveProfile(strProfile);
}

BOOL CSolidSBCProfileEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cProfileListCtrl.InitStructure(&m_ctlApplyButton);
	
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = m_cProfileListCtrl.m_vClientConfProfiles.begin(); i < m_cProfileListCtrl.m_vClientConfProfiles.end(); i++){
		m_ctlProfileCombo.InsertString( -1, (*i).m_strName ); }
	m_ctlProfileCombo.SetCurSel(0);
	
	CString strProfile = _T("");
	m_ctlProfileCombo.GetLBText(m_ctlProfileCombo.GetCurSel(),strProfile);
	m_cProfileListCtrl.SetActiveProfile(strProfile);

	RECT wndRect;
	GetClientRect(&wndRect);
	m_nOldX = wndRect.right  - wndRect.left;
	m_nOldY = wndRect.bottom - wndRect.top;

	m_cProfileListCtrl.GetWindowRect(&m_rectProfileList);
	ScreenToClient(&m_rectProfileList);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSolidSBCProfileEditorDlg::OnBnClickedSaveChangesButton()
{
	m_cProfileListCtrl.SaveProfile();

	CString strProfile = _T("");
	m_ctlProfileCombo.GetLBText(m_ctlProfileCombo.GetCurSel(),strProfile);
	m_cProfileListCtrl.SetActiveProfile(strProfile);
}

void CSolidSBCProfileEditorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	if (m_cProfileListCtrl.m_hWnd && (nType == SIZE_RESTORED) ){

		int nNewWidth = 0, nNewHeight = 0;

		int nXDiff = cx - m_nOldX; //positive if bigger
		int nYDiff = cy - m_nOldY; //positive if bigger
	
		m_rectProfileList.right  += nXDiff;
		m_rectProfileList.bottom += nYDiff;
		nNewWidth  = (m_rectProfileList.right  - m_rectProfileList.left);
		nNewHeight = (m_rectProfileList.bottom - m_rectProfileList.top );
		m_cProfileListCtrl.SetWindowPos(NULL,m_rectProfileList.left, m_rectProfileList.top, nNewWidth, nNewHeight,0);

		m_nOldX = cx;
		m_nOldY = cy;
	}
}
