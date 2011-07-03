
// SolidSBCResultViewer.h : main header file for the SolidSBCResultViewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include <map>


// CSolidSBCResultViewerApp:
// See SolidSBCResultViewer.cpp for the implementation of this class
//

class CSolidSBCResultViewerApp : public CWinApp
{
public:
	CSolidSBCResultViewerApp();

	void OnDatabaseConnected();
	void OnClientHistoryView(CString strClientUUID);
	void OnHDResultView(CString strClientUUID, int nIdentity = -1);
	void OnMemoryResultView(CString strClientUUID, int nIdentity = -1);
	void OnCPUMeasureResultView(CString strClientUUID, int nIdentity = -1);
	void OnTCPResultView(CString strClientUUID, int nIdentity = -1);
	void OnPingResultView(CString strClientUUID, int nIdentity = -1);
	void OnGraphPlotterView(
		const CString& strClientUUID, 
		const int& nIdentity, 
		const std::map<int, std::map<int, int>>&      mapMapResults, 
		const std::map<int, COLORREF>& mapColors, 
		const std::map<int, std::pair<CString, CString>>&  mapPairsUnits);

	CString GetSettingsFileName(void);

	CMainFrame* m_pMainFrame;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();

private:
	CString GetFilePath(void);
	CDocument* OpenView(CString strViewName);
};

extern CSolidSBCResultViewerApp theApp;
