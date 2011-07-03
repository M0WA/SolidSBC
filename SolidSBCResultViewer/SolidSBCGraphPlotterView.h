#pragma once

#include <map>

class CSolidSBCGraphPlotterDoc;

// CSolidSBCGraphPlotterView view

class CSolidSBCGraphPlotterView : public CScrollView
{
	DECLARE_DYNCREATE(CSolidSBCGraphPlotterView)

protected:
	CSolidSBCGraphPlotterView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCGraphPlotterView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()

private:
	void PaintCoordinateSystem(CDC* pDC);
	void PaintGraph(CDC* pDC);
	void PaintUnits(CDC* pDC, const std::pair<CString,CString>& pairUnits, const CRect& rectMinMax,int& nLabelYOffset, int& nYValueOffset, COLORREF colorGraph);
};


