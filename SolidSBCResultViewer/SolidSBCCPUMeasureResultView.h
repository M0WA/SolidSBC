#pragma once
#include "afxcmn.h"



// CSolidSBCCPUMeasureResultView form view

class CSolidSBCCPUMeasureResultView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCCPUMeasureResultView)

public:
	void SetClientUUID(CString strUUID, int nIdentity = -1);

protected:
	CSolidSBCCPUMeasureResultView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCCPUMeasureResultView();

public:
	enum { IDD = IDD_SOLIDSBCCPUMEASURERESULTVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnNMRClickCpumeasureResultList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:	
	void OnFilterDate(void);
	void OnExportSelectedResults(void);
	void OnExportAllResults(void);
	void OnExportresultsPlot(void);
	void ExportResults(CString strFileName,bool bSelectedOnly = false);

	void RefreshList(void);

	CString   m_strUUID;
	CListCtrl m_ctlCPUMeasureList;
	int       m_nIdentity;
};


