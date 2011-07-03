#pragma once

// CSolidSBCGraphPlotterDoc document
#include <map>

class CSolidSBCGraphPlotterView;

class CSolidSBCGraphPlotterDoc : public CDocument
{
	DECLARE_DYNCREATE(CSolidSBCGraphPlotterDoc)

public:
	CSolidSBCGraphPlotterDoc();
	virtual ~CSolidSBCGraphPlotterDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	void SetResults(
		const std::map<int, std::map<int, int>>&			mapMapResults, 
		const std::map<int, COLORREF>&						mapColors, 
		const std::map<int, std::pair<CString, CString>>&	mapPairsUnits
		);
	void GetResults(
		std::map<int, std::map<int, int>>&				mapMapResults, 
		std::map<int, COLORREF>&						mapColors, 
		std::map<int, std::pair<CString, CString>>&		mapPairsUnits
		);

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

private:
	std::map<int, std::map<int, int>>			m_mapMapCoordinates;
	std::map<int, COLORREF>						m_mapColors;
	std::map<int, std::pair<CString, CString>>  m_mapPairsUnits;
};
