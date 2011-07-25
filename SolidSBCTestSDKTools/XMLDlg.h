#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CXMLDlg dialog

class CXMLDlg : public CDialog
{
	DECLARE_DYNAMIC(CXMLDlg)

public:
	CXMLDlg(CString sXmlText,CWnd* pParent = NULL);   // standard constructor
	virtual ~CXMLDlg();

// Dialog Data
	enum { IDD = IDD_XML_DIALOG };

protected:
	virtual BOOL OnInitDialog( );
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedSaveButton();

	DECLARE_MESSAGE_MAP()

private:
	CString       m_sXmlText;
	CEdit m_ctlXmlEdit;
};
