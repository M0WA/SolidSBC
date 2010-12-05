#pragma once
#include "afxcmn.h"



// CSolidSBCClientListView form view

class CSolidSBCClientListView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCClientListView)

protected:
	CSolidSBCClientListView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCClientListView();

public:
	enum { IDD = IDD_SOLIDSBCCLIENTLISTVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickClientTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClientlistcontextmenuViewhistory();
	afx_msg void OnViewresultsHarddrive();
	afx_msg void OnViewresultsCpumeasure();
	afx_msg void OnViewresultsMemory();
	afx_msg void OnViewresultsPing();
	afx_msg void OnViewresultsTcpconnect();
	afx_msg void OnClientlistcontextmenuDatabasestats();

	DECLARE_MESSAGE_MAP()

private:
	CString GetSelectedClientUUID(void);
	void    RefreshTree(void);
	void    EmptyList(void);

	CTreeCtrl    m_ctlClientTree;
	CStringArray m_arUUIDs;
};


