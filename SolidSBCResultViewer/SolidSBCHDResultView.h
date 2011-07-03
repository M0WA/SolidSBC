#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CSolidSBCHDResultView form view

class CSolidSBCHDResultView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCHDResultView)

protected:
	CSolidSBCHDResultView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCHDResultView();

public:
	void SetClientUUID(CString strUUID, int nIdentity = -1);
	enum { IDD = IDD_SOLIDSBCHDRESULTVIEW };
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
	afx_msg void OnNMRClickHdResultList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	void RefreshList(void);	
	void OnFilterDate(void);
	void OnExportSelectedResults(void);
	void OnExportAllResults(void);
	void ExportResults(CString strFileName,bool bSelectedOnly = false);
	void OnExportresultsPlot(void);

	CString m_strUUID;
	CListCtrl m_ctlHDResultList;
	CStatic m_ctlHDStatic;
	int m_nIdentity;
};


