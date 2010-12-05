#pragma once
#include "afxdtctl.h"


// CSolidSBCTimeChooseDlg dialog

class CSolidSBCTimeChooseDlg : public CDialog
{
	DECLARE_DYNAMIC(CSolidSBCTimeChooseDlg)

public:
	CSolidSBCTimeChooseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSolidSBCTimeChooseDlg();

	CString m_strStartDate;
	CString m_strStartTime;

	CString m_strStopDate;
	CString m_strStopTime;


// Dialog Data
	enum { IDD = IDD_SOLIDSBC_CHOOSE_TIME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:	
	virtual void OnOK();

	CDateTimeCtrl m_ctlStartDate;
	CDateTimeCtrl m_ctlStartTime;
	CDateTimeCtrl m_ctlStopDate;
	CDateTimeCtrl m_ctlStopTime;
};
