// SolidSBCGraphPlotterView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCGraphPlotterView.h"


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
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
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
