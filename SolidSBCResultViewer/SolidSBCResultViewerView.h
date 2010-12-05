
// SolidSBCResultViewerView.h : interface of the CSolidSBCResultViewerView class
//

#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"


class CSolidSBCResultViewerView : public CFormView
{
protected: // create from serialization only
	CSolidSBCResultViewerView();
	DECLARE_DYNCREATE(CSolidSBCResultViewerView)

public:
	enum{ IDD = IDD_SOLIDSBCRESULTVIEWER_FORM };
	CSolidSBCResultViewerDoc* GetDocument() const;


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual ~CSolidSBCResultViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg void OnBnClickedConnectButton();
	afx_msg void OnBnClickedSaveButton();
	DECLARE_MESSAGE_MAP()

private:
	CButton m_ctlConnectButton;
	CIPAddressCtrl m_ctlDbIP;
	CEdit m_ctlPortEdit;
	CEdit m_ctlDbNameEdit;
	CEdit m_ctlDbUserEdit;
	CEdit m_ctlDbPassEdit;
};

#ifndef _DEBUG  // debug version in SolidSBCResultViewerView.cpp
inline CSolidSBCResultViewerDoc* CSolidSBCResultViewerView::GetDocument() const
   { return reinterpret_cast<CSolidSBCResultViewerDoc*>(m_pDocument); }
#endif

