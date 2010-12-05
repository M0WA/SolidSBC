#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CSolidSBCPingResultView form view

class CSolidSBCPingResultView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCPingResultView)

protected:
	CSolidSBCPingResultView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCPingResultView();

public:
	void SetClientUUID(CString strUUID, int nIdentity = -1);
	enum { IDD = IDD_SOLIDSBCPINGRESULTVIEW };
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
	afx_msg void OnNMRClickPingResultList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	void OnFilterDate(void);
	void OnExportSelectedResults(void);
	void OnExportAllResults(void);
	void ExportResults(CString strFileName,bool bSelectedOnly = false);
	void RefreshList(void);

	CString   m_strUUID;
	CListCtrl m_ctlPingResultList;
	CStatic   m_ctlPingStatic;
	int m_nIdentity;
};


