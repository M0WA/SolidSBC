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

	SetTitle(_T("SolidSBC Graph Viewer"));

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
void CSolidSBCGraphPlotterDoc::SetResults(
		const std::map<int, std::map<int, int>>&			mapMapResults, 
		const std::map<int, COLORREF>&						mapColors, 
		const std::map<int, std::pair<CString, CString>>&	mapPairsUnits
		)
{
	m_mapMapCoordinates = mapMapResults;
	m_mapColors         = mapColors;
	m_mapPairsUnits     = mapPairsUnits;
}

void CSolidSBCGraphPlotterDoc::GetResults(
		std::map<int, std::map<int, int>>&				mapMapResults, 
		std::map<int, COLORREF>&						mapColors, 
		std::map<int, std::pair<CString, CString>>&		mapPairsUnits
		)
{
	mapMapResults = m_mapMapCoordinates;
	mapColors     = m_mapColors;
	mapPairsUnits = m_mapPairsUnits;
}