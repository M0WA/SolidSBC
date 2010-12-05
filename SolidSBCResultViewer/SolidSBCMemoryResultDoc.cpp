// SolidSBCMemoryResultDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCMemoryResultDoc.h"


// CSolidSBCMemoryResultDoc

IMPLEMENT_DYNCREATE(CSolidSBCMemoryResultDoc, CDocument)

CSolidSBCMemoryResultDoc::CSolidSBCMemoryResultDoc()
{
}

BOOL CSolidSBCMemoryResultDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCMemoryResultDoc::~CSolidSBCMemoryResultDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCMemoryResultDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCMemoryResultDoc diagnostics

#ifdef _DEBUG
void CSolidSBCMemoryResultDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCMemoryResultDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCMemoryResultDoc serialization

void CSolidSBCMemoryResultDoc::Serialize(CArchive& ar)
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


// CSolidSBCMemoryResultDoc commands
