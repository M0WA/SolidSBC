// SolidSBCTimeChooseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCTimeChooseDlg.h"
#include "afxdialogex.h"


// CSolidSBCTimeChooseDlg dialog

IMPLEMENT_DYNAMIC(CSolidSBCTimeChooseDlg, CDialog)

CSolidSBCTimeChooseDlg::CSolidSBCTimeChooseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSolidSBCTimeChooseDlg::IDD, pParent)
	, m_strStartDate(_T(""))
	, m_strStartTime(_T(""))
	, m_strStopDate(_T(""))
	, m_strStopTime(_T(""))
{

}

CSolidSBCTimeChooseDlg::~CSolidSBCTimeChooseDlg()
{
}

void CSolidSBCTimeChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_DATEPICKER, m_ctlStartDate);
	DDX_Control(pDX, IDC_START_TIMEPICKER, m_ctlStartTime);
	DDX_Control(pDX, IDC_STOP_DATEPICKER, m_ctlStopDate);
	DDX_Control(pDX, IDC_STOP_TIMEPICKER, m_ctlStopTime);
}


BEGIN_MESSAGE_MAP(CSolidSBCTimeChooseDlg, CDialog)
END_MESSAGE_MAP()


// CSolidSBCTimeChooseDlg message handlers


void CSolidSBCTimeChooseDlg::OnOK()
{
	m_ctlStartDate.SetFormat(_T("yyyy-MM-dd"));
	m_ctlStopDate .SetFormat(_T("yyyy-MM-dd"));

	m_ctlStartDate.GetWindowText(m_strStartDate);
	m_ctlStartTime.GetWindowText(m_strStartTime);
	m_ctlStopDate .GetWindowText(m_strStopDate);
	m_ctlStopTime .GetWindowText(m_strStopTime);

	CDialog::OnOK();
}
