#pragma once
#include "resource.h"
#include "afxwin.h"
#include "SolidSBCProfileListCtrl.h"
#include "afxdialogex.h"


// CSolidSBCProfileEditorDlg dialog

class CSolidSBCProfileEditorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSolidSBCProfileEditorDlg)

public:
	CSolidSBCProfileEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSolidSBCProfileEditorDlg();

// Dialog Data
	enum { IDD = IDD_PROFILE_EDITOR_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCbnSelchangeProfileCombo();
	afx_msg void OnBnClickedSaveChangesButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_ctlProfileCombo;
	CProfileListCtrl m_cProfileListCtrl;
	CButton m_ctlApplyButton;
	RECT	m_rectProfileList;
	int     m_nOldX;
	int     m_nOldY;
};
