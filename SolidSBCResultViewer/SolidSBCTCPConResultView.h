#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CSolidSBCTCPConResultView form view

class CSolidSBCTCPConResultView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCTCPConResultView)

protected:
	CSolidSBCTCPConResultView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCTCPConResultView();

public:
	void SetClientUUID(CString strUUID, int nIdentity = -1);
	enum { IDD = IDD_SOLIDSBCTCPCONRESULTVIEW };
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
	afx_msg void OnNMRClickTcpResultList(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

private:
	void OnFilterDate(void);
	void OnExportSelectedResults(void);
	void OnExportAllResults(void);
	void ExportResults(CString strFileName,bool bSelectedOnly = false);
	void RefreshList(void);

	CString m_strUUID;
	CListCtrl m_ctlTcpResultList;
	CStatic m_ctlTcpResultStatic;
	int m_nIdentity;
};


