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


	PaintCoordinateSystem(pDC);
	PaintGraph(pDC);
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

void CSolidSBCGraphPlotterView::PaintCoordinateSystem(CDC* pDC)
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
		pDC->MoveTo(clientRect.left,nCount  * nHeightDivision);
		pDC->LineTo(clientRect.right,nCount * nHeightDivision);
	}
	pDC->SelectObject( pOldPen );
}

void CSolidSBCGraphPlotterView::PaintGraph(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(clientRect);
	
	float nHeight = (float)abs(clientRect.top - clientRect.bottom);
	float nLength = (float)abs(clientRect.right - clientRect.left);

	CSolidSBCGraphPlotterDoc* pDoc = (CSolidSBCGraphPlotterDoc*)GetDocument();
	
	std::map<int,std::map<int,int>>			  mapMapCoordinates;
	std::map<int,COLORREF>					  mapColors;
	std::map<int,std::pair<CString,CString>>  mapPairUnits;
	pDoc->GetResults(mapMapCoordinates,mapColors,mapPairUnits);

	int nLabelYOffset = clientRect.top+10, nYValueOffset = 3;
	std::map<int,std::map<int, int>>::iterator iterMapMapCoord = mapMapCoordinates.begin();
	for(; iterMapMapCoord != mapMapCoordinates.end(); iterMapMapCoord++)
	{
		std::map<int, int> mapCoordinates = (*iterMapMapCoord).second;

		int nMapPos = (*iterMapMapCoord).first;
		std::pair<CString,CString> pairUnits = mapPairUnits[nMapPos];
		COLORREF colorGraph = mapColors[nMapPos];

		int nMinX = 0, nMaxX = 0, nMinY = 0, nMaxY = 0;
		std::map<int, int>::iterator itCoord = mapCoordinates.begin();
		for( ;itCoord != mapCoordinates.end();itCoord++)
		{
			nMinX = ( (*itCoord).first < nMinX ) ? (*itCoord).first : nMinX;
			nMaxX = ( (*itCoord).first > nMaxX ) ? (*itCoord).first : nMaxX;
			nMinY = ( (*itCoord).second < nMinY ) ? (*itCoord).second : nMinY;
			nMaxY = ( (*itCoord).second > nMaxY ) ? (*itCoord).second : nMaxY;
		}
		if ( nMinY > 0 )
			nMinY = 0;
		PaintUnits(pDC, pairUnits, CRect(nMinX,nMinY,nMaxX,nMaxY), nLabelYOffset, nYValueOffset, colorGraph);

		float nGraphHeight = (float)abs(nMinY - nMaxY);
		float nYStep = 0.0f;
		if(nGraphHeight > 0.0f)
			nYStep = (float)nHeight/nGraphHeight;
		if ( nYStep <= 0.0f )
			nYStep = 1.0f;

		float nXStep = 0.0f;
		if( mapCoordinates.size() > 0 )
			nXStep = nLength/(float)mapCoordinates.size();
		if ( nXStep <= 0.0f )
			nXStep = 1.0f;
	
		CPen pen( PS_SOLID, 0, colorGraph );
		CPen* pOldPen = pDC->SelectObject( &pen );

		pDC->MoveTo(clientRect.left,clientRect.bottom);
		std::map<int, int>::iterator iterCoordinates = mapCoordinates.begin();
		int nX = clientRect.left;
		int nY = clientRect.bottom - (int)( (*iterCoordinates).second * nYStep );
		pDC->MoveTo( nX, nY );
		iterCoordinates++;
		for(;iterCoordinates != mapCoordinates.end(); iterCoordinates++)
		{
			nX = clientRect.left   + (int)( (*iterCoordinates).first  * nXStep );
			nY = clientRect.bottom - (int)( (*iterCoordinates).second * nYStep );
			pDC->LineTo( nX, nY );
			pDC->MoveTo( nX, nY );
		}
		pDC->SelectObject( pOldPen );
	}
}

void CSolidSBCGraphPlotterView::PaintUnits(CDC* pDC, const std::pair<CString,CString>& pairUnits, const CRect& rectMinMax,int& nLabelYOffset, int& nYValueOffset, COLORREF colorGraph)
{
	int nHeightDivisionCount = 10;

	CRect clientRect;
	GetClientRect(clientRect);
	int nHeight = abs(clientRect.top - clientRect.bottom);
	int nHeightDivision = nHeight / nHeightDivisionCount;
	
	int nCoordHeight         = rectMinMax.Height();
	int nCoordHeightDivision = nCoordHeight / nHeightDivisionCount;

	COLORREF oldBkColor = pDC->GetBkColor();
	pDC->SetBkColor(RGB(0,0,0));

	int oldBkMode = pDC->GetBkMode();
	pDC->SetBkMode(TRANSPARENT);

	COLORREF oldColor = pDC->GetTextColor();
	pDC->SetTextColor(colorGraph);

	CFont* oldFont = pDC->GetCurrentFont();
	CFont newFont;
	newFont.CreateFont(17, 0, 0, 0, FW_NORMAL, 0, 0, 0,  
    DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,    
    DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
	oldFont = pDC->SelectObject(&newFont);

	//draw axis labels
	CString sLabel;
	sLabel.Format(_T("x: %s | y: %s"),pairUnits.first,pairUnits.second);
	CSize sizeLabel = pDC->GetTextExtent(sLabel);
	pDC->TextOut(clientRect.right-(sizeLabel.cx+10), clientRect.top + nLabelYOffset,sLabel);
	nLabelYOffset += sizeLabel.cy + 3;

	//draw each y-line value
	CSize sizeValue;
	for(int nCount = 0; nCount < nHeightDivisionCount; nCount++)
	{	
		CString sYValue;
		sYValue.Format(_T("%d"),nCoordHeightDivision * (nHeightDivisionCount-nCount));
		sizeValue = pDC->GetTextExtent(sYValue);

		int nYPos = nCount * nHeightDivision;
		pDC->TextOut(clientRect.left+5,nYPos+nYValueOffset,sYValue);
	}
	nYValueOffset += sizeValue.cy + 3;
	
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SetBkColor(oldBkColor);
}