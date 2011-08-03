
// SolidSBCCliCfgGUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <Winsvc.h>


// CSolidSBCCliCfgGUIDlg dialog
class CSolidSBCCliCfgGUIDlg : public CDialog
{
// Construction
public:
	CSolidSBCCliCfgGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOLIDSBCCLICFGGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnBnClickedInstallButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedApplyButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOpenClientDirButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	bool IsServiceInstalled(void);
	bool IsServiceRunning(void);
	void MakeButtonStates(void);

	CString GetSettingsFileName(void);
	CString GetLogFileName(void);
	CString GetFilePath(void);
	BOOL LoadParameters(void);	

	CButton m_ctlAutoReconnectCheck;
	CIPAddressCtrl m_ctlServerIP;
	CEdit m_ctlServerCfgPort;
	CEdit m_ctlServerDtaPort;
	CButton m_ctlAutoStartCheck;
	CButton m_ctlStartServiceButton;
	CButton m_ctlStopServiceButton;
	CButton m_ctlInstallServiceButton;
	CButton m_ctlDeleteServiceButton;
	CMutex m_cTimerMutex;
	CButton m_ctlOpenDirButton;
	CRichEditCtrl m_ctlLogEdit;
	CStatic m_ctlLogStatic;
	CString m_strLogFileContent;
	RECT    m_rectLogEdit;
	RECT    m_rectLogStatic;
	UINT_PTR m_nTimer;
	int m_nOldX;
	int m_nOldY;
};
