// SolidSBCHDResultDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCHDResultDoc.h"


// CSolidSBCHDResultDoc

IMPLEMENT_DYNCREATE(CSolidSBCHDResultDoc, CDocument)

CSolidSBCHDResultDoc::CSolidSBCHDResultDoc()
{
}

BOOL CSolidSBCHDResultDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCHDResultDoc::~CSolidSBCHDResultDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCHDResultDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCHDResultDoc diagnostics

#ifdef _DEBUG
void CSolidSBCHDResultDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCHDResultDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCHDResultDoc serialization

void CSolidSBCHDResultDoc::Serialize(CArchive& ar)
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


// CSolidSBCHDResultDoc commands
