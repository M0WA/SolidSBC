// XMLDlg.cpp : implementation file
//

#include "stdafx.h"
#include <SolidSBCTestSDK.h>
#include "SolidSBCTestSDKTools.h"
#include "XMLDlg.h"
#include "afxdialogex.h"


// CXMLDlg dialog

IMPLEMENT_DYNAMIC(CXMLDlg, CDialog)

CXMLDlg::CXMLDlg(CString sXmlText,CWnd* pParent /*=NULL*/)
: CDialog(CXMLDlg::IDD, pParent)
, m_sXmlText(sXmlText)
{
}

CXMLDlg::~CXMLDlg()
{
}

void CXMLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XML_EDIT, m_ctlXmlEdit);
}


BEGIN_MESSAGE_MAP(CXMLDlg, CDialog)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CXMLDlg::OnBnClickedSaveButton)
END_MESSAGE_MAP()


// CXMLDlg message handlers
BOOL CXMLDlg::OnInitDialog( )
{	
	UpdateData(false);
	m_ctlXmlEdit.SetWindowText(m_sXmlText);
	UpdateData(true);
	return FALSE;
}

void CXMLDlg::OnBnClickedSaveButton()
{
	USES_CONVERSION;

	CFileDialog cFileDlg(FALSE,_T("xml"),_T("*.xml"));
	if ( cFileDlg.DoModal() == IDOK )
	{
		CString xmlConfigFileName = cFileDlg.GetPathName();
		CString strXML;
		m_ctlXmlEdit.GetWindowText(strXML);

		//writing xml to file
		CFile cFile;
		CFileException ex;
		if ( !cFile.Open(xmlConfigFileName, CFile::modeCreate | CFile::modeReadWrite, &ex) )
			return;
		cFile.Write( T2A(strXML), strlen(T2A(strXML)));
		cFile.Close();
	}
}
