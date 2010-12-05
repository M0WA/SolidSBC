// SolidSBCClientListDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCClientListDoc.h"


// CSolidSBCClientListDoc

IMPLEMENT_DYNCREATE(CSolidSBCClientListDoc, CDocument)

CSolidSBCClientListDoc::CSolidSBCClientListDoc()
{
}

BOOL CSolidSBCClientListDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CString strTitle = _T("");
	strTitle.Format(_T("client list ( %s@%s:%d/%s )"), g_cDatabaseConnection.m_strUser,g_cDatabaseConnection.m_strServer, g_cDatabaseConnection.m_nPort, g_cDatabaseConnection.m_strDatabase);
	SetTitle(strTitle);
	return TRUE;
}

CSolidSBCClientListDoc::~CSolidSBCClientListDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCClientListDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCClientListDoc diagnostics

#ifdef _DEBUG
void CSolidSBCClientListDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCClientListDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCClientListDoc serialization

void CSolidSBCClientListDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif


// CSolidSBCClientListDoc commands
