// SolidSBCCPUMeasureResultDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCCPUMeasureResultDoc.h"


// CSolidSBCCPUMeasureResultDoc

IMPLEMENT_DYNCREATE(CSolidSBCCPUMeasureResultDoc, CDocument)

CSolidSBCCPUMeasureResultDoc::CSolidSBCCPUMeasureResultDoc()
{
}

BOOL CSolidSBCCPUMeasureResultDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCCPUMeasureResultDoc::~CSolidSBCCPUMeasureResultDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCCPUMeasureResultDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCCPUMeasureResultDoc diagnostics

#ifdef _DEBUG
void CSolidSBCCPUMeasureResultDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCCPUMeasureResultDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCCPUMeasureResultDoc serialization

void CSolidSBCCPUMeasureResultDoc::Serialize(CArchive& ar)
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


// CSolidSBCCPUMeasureResultDoc commands
