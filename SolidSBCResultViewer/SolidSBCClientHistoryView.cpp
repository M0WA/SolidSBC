// SolidSBCClientHistoryView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCClientHistoryView.h"


// CSolidSBCClientHistoryView

IMPLEMENT_DYNCREATE(CSolidSBCClientHistoryView, CFormView)

CSolidSBCClientHistoryView::CSolidSBCClientHistoryView()
	: CFormView(CSolidSBCClientHistoryView::IDD)
	, m_strUUID(_T(""))
{

}

CSolidSBCClientHistoryView::~CSolidSBCClientHistoryView()
{
}

void CSolidSBCClientHistoryView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTORY_LIST, m_ctlHistoryList);
}

BEGIN_MESSAGE_MAP(CSolidSBCClientHistoryView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSolidSBCClientHistoryView diagnostics

#ifdef _DEBUG
void CSolidSBCClientHistoryView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCClientHistoryView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCClientHistoryView message handlers


void CSolidSBCClientHistoryView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);

	m_ctlHistoryList.SetExtendedStyle( m_ctlHistoryList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_ctlHistoryList.InsertColumn(0,_T("action"),0,50);
	m_ctlHistoryList.InsertColumn(1,_T("param"),0,100);
	m_ctlHistoryList.InsertColumn(2,_T("client id"),0,50);
	m_ctlHistoryList.InsertColumn(3,_T("client name"),0,150);
	m_ctlHistoryList.InsertColumn(4,_T("time"),0,150);

	CRect rect;
	GetClientRect(&rect);
	m_ctlHistoryList.MoveWindow(10,10,rect.right-20,rect.bottom-20,1);

}

void CSolidSBCClientHistoryView::SetClientUUID(CString strUUID)
{
	m_strUUID = strUUID;
	
	CString strTitle = _T("");
	strTitle.Format(_T("client history ( uuid: %s )"),m_strUUID);
	GetDocument()->SetTitle(strTitle);

	RefreshList();
}

void CSolidSBCClientHistoryView::RefreshList(void)
{
	std::vector<SSBC_CLIENT_HISTORY> vHistory = g_cDatabaseConnection.GetClientHistory(m_strUUID);	

	std::vector<SSBC_CLIENT_HISTORY>::iterator iCliIter = vHistory.begin();
	for ( iCliIter = vHistory.begin(); iCliIter < vHistory.end(); iCliIter++)
	{
		CString strAction = _T("na");
		CString strParam  = _T("na");

		switch( _ttoi( (*iCliIter).strAction ) )
		{
		case 1:
			strAction = _T("login");
			strParam.Format(_T("profile id: %s"),(*iCliIter).strParam);
			break;
		case 2:
			strAction = _T("logout");
			strParam  = _T("");
			break;
		default:
			break;
		}

		int nItem = m_ctlHistoryList.InsertItem( m_ctlHistoryList.GetItemCount(), strAction, 0 );
		m_ctlHistoryList.SetItemText(nItem,1, strParam);
		m_ctlHistoryList.SetItemText(nItem,2, (*iCliIter).strClientID);
		m_ctlHistoryList.SetItemText(nItem,3, (*iCliIter).strClientName);
		m_ctlHistoryList.SetItemText(nItem,4, (*iCliIter).strTime);
	}
}

void CSolidSBCClientHistoryView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	if ( m_ctlHistoryList.m_hWnd )
		m_ctlHistoryList.MoveWindow(10,10,cx-20,cy-20,1);
}
