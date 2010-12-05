// SolidSBCClientHistoryDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCClientHistoryDoc.h"


// CSolidSBCClientHistoryDoc

IMPLEMENT_DYNCREATE(CSolidSBCClientHistoryDoc, CDocument)

CSolidSBCClientHistoryDoc::CSolidSBCClientHistoryDoc()
{
}

BOOL CSolidSBCClientHistoryDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCClientHistoryDoc::~CSolidSBCClientHistoryDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCClientHistoryDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCClientHistoryDoc diagnostics

#ifdef _DEBUG
void CSolidSBCClientHistoryDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCClientHistoryDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCClientHistoryDoc serialization

void CSolidSBCClientHistoryDoc::Serialize(CArchive& ar)
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


// CSolidSBCClientHistoryDoc commands
