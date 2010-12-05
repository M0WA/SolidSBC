// SolidSBCPingResultDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCPingResultDoc.h"


// CSolidSBCPingResultDoc

IMPLEMENT_DYNCREATE(CSolidSBCPingResultDoc, CDocument)

CSolidSBCPingResultDoc::CSolidSBCPingResultDoc()
{
}

BOOL CSolidSBCPingResultDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCPingResultDoc::~CSolidSBCPingResultDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCPingResultDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCPingResultDoc diagnostics

#ifdef _DEBUG
void CSolidSBCPingResultDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCPingResultDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCPingResultDoc serialization

void CSolidSBCPingResultDoc::Serialize(CArchive& ar)
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


// CSolidSBCPingResultDoc commands
