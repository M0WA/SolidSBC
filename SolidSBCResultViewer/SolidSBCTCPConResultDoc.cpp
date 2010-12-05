// SolidSBCTCPConResultDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCTCPConResultDoc.h"


// CSolidSBCTCPConResultDoc

IMPLEMENT_DYNCREATE(CSolidSBCTCPConResultDoc, CDocument)

CSolidSBCTCPConResultDoc::CSolidSBCTCPConResultDoc()
{
}

BOOL CSolidSBCTCPConResultDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCTCPConResultDoc::~CSolidSBCTCPConResultDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCTCPConResultDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCTCPConResultDoc diagnostics

#ifdef _DEBUG
void CSolidSBCTCPConResultDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCTCPConResultDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCTCPConResultDoc serialization

void CSolidSBCTCPConResultDoc::Serialize(CArchive& ar)
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


// CSolidSBCTCPConResultDoc commands
