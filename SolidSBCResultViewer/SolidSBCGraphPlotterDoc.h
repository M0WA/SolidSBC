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
	void SetResults(const std::map<int, int>& mapResults);
	std::map<int,int> GetResults() { return m_mapCoordinates; }

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

private:
	std::map<int,int> m_mapCoordinates;
};
