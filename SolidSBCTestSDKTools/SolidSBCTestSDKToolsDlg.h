
// SolidSBCTestSDKToolsDlg.h : header file
//
#pragma once


#include "afxwin.h"
#include <vector>
#include <SolidSBCTestSDK.h>

// CSolidSBCTestSDKToolsDlg dialog
class CSolidSBCTestSDKToolsDlg : public CDialogEx
{
// Construction
public:
	CSolidSBCTestSDKToolsDlg(CWnd* pParent = NULL);	// standard constructor
	~CSolidSBCTestSDKToolsDlg();

// Dialog Data
	enum { IDD = IDD_SOLIDSBCTESTSDKTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBrowseDllfileButton();
	afx_msg void OnBnClickedLoadDllfileButton();
	afx_msg void OnBnClickedGenerateEmptyConfigXmlButton();
	afx_msg void OnBnClickedStartStopButton();
	afx_msg void OnLbnSelchangeTestList();
	DECLARE_MESSAGE_MAP()

private:
	CSolidSBCTestManager* GetTestManager();
	bool LoadTestLibrary();
	bool UnloadTestLibrary();
	bool GetTestNames(std::vector<CString>& vecNames);

	CButton  m_ctlGenerateConfigButton;
	CButton  m_ctlStartStopButton;
	CEdit    m_cDllFileName;
	CListBox m_ctlTestList;
	HMODULE  m_hTestLibrary;
public:
	afx_msg void OnBnClickedGenerateStuctureButton();
private:
	CButton m_ctlGenerateDBStructure;
public:
	afx_msg void OnBnClickedCreateDatabaseButton();
};
