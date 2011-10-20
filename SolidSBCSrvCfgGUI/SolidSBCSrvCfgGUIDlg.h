
// SolidSBCSrvCfgGUIDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SolidSBCProfileEditorDlg.h"


// CSolidSBCSrvCfgGUIDlg dialog
class CSolidSBCSrvCfgGUIDlg : public CDialog
{
// Construction
public:
	CSolidSBCSrvCfgGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SOLIDSBCSRVCFGGUI_DIALOG };

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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOpenServerDirButton();
	afx_msg void OnBnClickedApplySettingsButton();
	afx_msg void OnBnClickedEditConfigButton();
	afx_msg void OnBnClickedProfileEditorButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	bool IsServiceInstalled(void);
	bool IsServiceRunning(void);
	void MakeButtonStates(void);
	bool StartStopClientService(bool bStart);

	CString GetLogFileName(void);
	CString GetSettingsFileName(void);
	CString GetFilePath(void);
	BOOL    LoadParameters(void);

	CRichEditCtrl m_ctlLogEdit;
	CMutex m_cTimerMutex;
	UINT_PTR m_nTimer;
	CComboBox m_ctlDbTypeCombo;
	CIPAddressCtrl m_ctlDBIp;
	CEdit m_ctlDBPortEdit;
	CEdit m_ctlDBNameEdit;
	CEdit m_ctlDBUserEdit;
	CEdit m_ctlDBPassEdit;
	CSolidSBCProfileEditorDlg* m_pProfileEditor;
	CButton m_ctlInstallServiceButton;
	CButton m_ctlDeleteServiceButton;
	CButton m_ctlStartServiceButton;
	CButton m_ctlStopServiceButton;
	CButton m_ctlOpenDirButton;
	CButton m_ctlEditConfigButton;
	CButton m_ctlProfileEditorButton;
	CStatic m_ctlLogStatic;
	CString m_strLogFileContent;
	RECT    m_rectLogEdit;
	RECT    m_rectLogStatic;
	int     m_nOldX;
	int     m_nOldY;
};
