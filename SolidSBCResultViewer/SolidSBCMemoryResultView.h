#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CSolidSBCMemoryResultView form view

class CSolidSBCMemoryResultView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCMemoryResultView)

protected:
	CSolidSBCMemoryResultView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCMemoryResultView();

public:
	void SetClientUUID(CString strUUID, int nIdentity = -1);
	enum { IDD = IDD_SOLIDSBCMEMORYRESULTVIEW };
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
	afx_msg void OnNMRClickMemResultList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	void RefreshList(void);	
	void OnFilterDate(void);
	void OnExportSelectedResults(void);
	void OnExportAllResults(void);
	void ExportResults(CString strFileName,bool bSelectedOnly = false);
	void OnExportresultsPlot(void);

	CListCtrl m_ctlMemResultList;
	CString m_strUUID;
	CStatic m_ctlAvgSpeedStatic;
	int m_nIdentity;
};


