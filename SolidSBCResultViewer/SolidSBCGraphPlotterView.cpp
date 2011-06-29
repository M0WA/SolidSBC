// SolidSBCGraphPlotterView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCGraphPlotterView.h"
#include "SolidSBCGraphPlotterDoc.h"


// CSolidSBCGraphPlotterView

IMPLEMENT_DYNCREATE(CSolidSBCGraphPlotterView, CScrollView)

CSolidSBCGraphPlotterView::CSolidSBCGraphPlotterView()
{

}

CSolidSBCGraphPlotterView::~CSolidSBCGraphPlotterView()
{
}


BEGIN_MESSAGE_MAP(CSolidSBCGraphPlotterView, CScrollView)
END_MESSAGE_MAP()


// CSolidSBCGraphPlotterView drawing

void CSolidSBCGraphPlotterView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CSolidSBCGraphPlotterView::OnDraw(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(clientRect);

	CBrush* brush = new CBrush((COLORREF)0);
	pDC->FillRect(&clientRect,brush);
	delete brush;

	CSolidSBCGraphPlotterDoc* pDoc = (CSolidSBCGraphPlotterDoc*)GetDocument();
	std::map<int, int> mapCoordinates = pDoc->GetResults();

	int nMinX = 0, nMaxX = 0, nMinY = 0, nMaxY = 0;
	std::map<int, int>::iterator itCoord = mapCoordinates.begin();
	for( ;itCoord != mapCoordinates.end();itCoord++)
	{
		nMinX = ( (*itCoord).first < nMinX ) ? (*itCoord).first : nMinX;
		nMaxX = ( (*itCoord).first > nMaxX ) ? (*itCoord).first : nMaxX;
		nMinY = ( (*itCoord).second < nMinY ) ? (*itCoord).second : nMinY;
		nMaxY = ( (*itCoord).second > nMaxY ) ? (*itCoord).second : nMaxY;
	}
	CRect rectData(nMinX,nMinY,nMaxX,nMaxY);

	PaintCoordinateSystem(pDC,rectData);
}


// CSolidSBCGraphPlotterView diagnostics

#ifdef _DEBUG
void CSolidSBCGraphPlotterView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCGraphPlotterView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCGraphPlotterView message handlers

void CSolidSBCGraphPlotterView::PaintCoordinateSystem(CDC* pDC, const CRect& rectData)
{
	CRect clientRect;
	GetClientRect(clientRect);

	int nHeightDivisionCount = 10;
	int nHeight = abs(clientRect.top - clientRect.bottom);
	int nHeightDivision = nHeight / nHeightDivisionCount;

	CPen pen( PS_SOLID, 0, RGB( 0, 255, 0 ) );
	CPen* pOldPen = pDC->SelectObject( &pen );
	for(int nCount = 1; nCount < nHeightDivisionCount; nCount++)
	{	
		pDC->MoveTo(clientRect.left,nCount * nHeightDivision);
		pDC->LineTo(clientRect.right,nCount * nHeightDivision);
	}
	pDC->SelectObject( pOldPen );
}