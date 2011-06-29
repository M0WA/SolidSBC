// SolidSBCGraphPlotterDoc.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCGraphPlotterDoc.h"


// CSolidSBCGraphPlotterDoc

IMPLEMENT_DYNCREATE(CSolidSBCGraphPlotterDoc, CDocument)

CSolidSBCGraphPlotterDoc::CSolidSBCGraphPlotterDoc()
{
}

BOOL CSolidSBCGraphPlotterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSolidSBCGraphPlotterDoc::~CSolidSBCGraphPlotterDoc()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCGraphPlotterDoc, CDocument)
END_MESSAGE_MAP()


// CSolidSBCGraphPlotterDoc diagnostics

#ifdef _DEBUG
void CSolidSBCGraphPlotterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCGraphPlotterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSolidSBCGraphPlotterDoc serialization

void CSolidSBCGraphPlotterDoc::Serialize(CArchive& ar)
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


// CSolidSBCGraphPlotterDoc commands
void CSolidSBCGraphPlotterDoc::SetResults(const std::map<int, int>& mapResults)
{
	m_mapCoordinates = mapResults;
}