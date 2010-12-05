#pragma once
#include "afxcmn.h"



// CSolidSBCClientHistoryView form view

class CSolidSBCClientHistoryView : public CFormView
{
	DECLARE_DYNCREATE(CSolidSBCClientHistoryView)

protected:
	CSolidSBCClientHistoryView();           // protected constructor used by dynamic creation
	virtual ~CSolidSBCClientHistoryView();
	virtual void OnInitialUpdate();

public:
	void SetClientUUID(CString strUUID);
	enum { IDD = IDD_SOLIDSBCCLIENTHISTORYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	void RefreshList(void);

	CString   m_strUUID;
	CListCtrl m_ctlHistoryList;
};


