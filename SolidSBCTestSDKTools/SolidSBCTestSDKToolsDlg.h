
// SolidSBCTestSDKToolsDlg.h : header file
//
#pragma once


#include "afxwin.h"
#include <vector>
#include <SolidSBCTestSDK.h>
#include <SolidSBCDatabaseLib.h>
#include "afxcmn.h"

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
	afx_msg void OnBnClickedCreateDatabaseButton();
	afx_msg void OnBnClickedConnectDatabaseButton();
	afx_msg void OnBnClickedGenerateStuctureButton();
	DECLARE_MESSAGE_MAP()

private:
	CSolidSBCTestManager* GetTestManager();
	bool LoadTestLibrary();
	bool UnloadTestLibrary();
	bool GetTestNames(std::vector<CString>& vecNames);
	void CreateDatabase(void);
	void CreateGetClientNameProcedure(void);
	void CreateViews(void);

	CButton  m_ctlGenerateConfigButton;
	CButton  m_ctlStartStopButton;
	CButton m_ctlGenerateDBStructure;
	CButton m_ctlConnectButton;
	CButton m_ctlCreateDatabaseButton;
	CEdit    m_cDllFileName;
	CListBox m_ctlTestList;
	HMODULE  m_hTestLibrary;
	CIPAddressCtrl m_ctlDbHostIp;
	CEdit m_ctlDbPort;
	CEdit m_ctlDbDatabase;
	CEdit m_ctlDbUser;
	CEdit m_ctlDbPass;

	CSolidSBCResultDBConnector* m_pDatabase;
};
