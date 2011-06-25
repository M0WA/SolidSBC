// SolidSBCClientListView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCClientListView.h"


// CSolidSBCClientListView

IMPLEMENT_DYNCREATE(CSolidSBCClientListView, CFormView)

CSolidSBCClientListView::CSolidSBCClientListView()
	: CFormView(CSolidSBCClientListView::IDD)
{
}

CSolidSBCClientListView::~CSolidSBCClientListView()
{
}

void CSolidSBCClientListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENT_TREE, m_ctlClientTree);
}

BEGIN_MESSAGE_MAP(CSolidSBCClientListView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_CLIENT_TREE, &CSolidSBCClientListView::OnNMRClickClientTree)
	ON_COMMAND(ID_CLIENTLISTCONTEXTMENU_VIEWHISTORY, &CSolidSBCClientListView::OnClientlistcontextmenuViewhistory)
	ON_COMMAND(ID_VIEWRESULTS_HARDDRIVE, &CSolidSBCClientListView::OnViewresultsHarddrive)
	ON_COMMAND(ID_VIEWRESULTS_CPUMEASURE, &CSolidSBCClientListView::OnViewresultsCpumeasure)
	ON_COMMAND(ID_VIEWRESULTS_MEMORY, &CSolidSBCClientListView::OnViewresultsMemory)
	ON_COMMAND(ID_VIEWRESULTS_PING, &CSolidSBCClientListView::OnViewresultsPing)
	ON_COMMAND(ID_VIEWRESULTS_TCPCONNECT, &CSolidSBCClientListView::OnViewresultsTcpconnect)
	ON_COMMAND(ID_CLIENTLISTCONTEXTMENU_DATABASESTATS, &CSolidSBCClientListView::OnClientlistcontextmenuDatabasestats)
END_MESSAGE_MAP()


// CSolidSBCClientListView diagnostics

#ifdef _DEBUG
void CSolidSBCClientListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCClientListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCClientListView message handlers


void CSolidSBCClientListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	RefreshTree();

	ResizeParentToFit(FALSE);

	CRect rect;
	GetClientRect(&rect);
	m_ctlClientTree.MoveWindow(10,10,rect.right-20,rect.bottom-20,1);
}

void CSolidSBCClientListView::RefreshTree(void)
{
	EmptyList();

	std::vector<CString> vClientUUIDs;
	int nCli = g_cDatabaseConnection.GetUniqueClients(&vClientUUIDs);
	
	if (!nCli)
		return;

	std::vector<CString>::iterator iCliUUIDIter = vClientUUIDs.begin();
	for ( iCliUUIDIter = vClientUUIDs.begin(); iCliUUIDIter < vClientUUIDs.end(); iCliUUIDIter++)
	{
		INT_PTR nUUIDArrayIdx = m_arUUIDs.Add( (*iCliUUIDIter) );

		CString strNames = _T("");
		std::vector<CString> vNames = g_cDatabaseConnection.GetClientNames( (*iCliUUIDIter) );

		std::vector<CString>::iterator iNames;
		int nNameCnt = 0;
		for ( iNames = vNames.begin();  iNames < vNames.end(); iNames++){

			if ( !nNameCnt ){
				strNames += (*iNames);
				nNameCnt++;
			}else
				strNames += _T(", ") + (*iNames);
			
		}

		int nHDCnt         = g_cDatabaseConnection.GetHDResultCount((*iCliUUIDIter));
		int nCPUMeasureCnt = g_cDatabaseConnection.GetCPUMeasureResultCount((*iCliUUIDIter));
		int nMemCnt        = g_cDatabaseConnection.GetMemResultCount((*iCliUUIDIter));
		int nPingCnt       = g_cDatabaseConnection.GetPingResultCount((*iCliUUIDIter));
		int nTCPConCnt     = g_cDatabaseConnection.GetTCPResultCount((*iCliUUIDIter));
		std::vector<SSBC_CLIENT_IDENTITY> vIdentity = g_cDatabaseConnection.GetClientIdentities((*iCliUUIDIter));
		int nTimesSeen     = (int)vIdentity.size();
		
		CString strTmp = _T("");
		HTREEITEM hClientItem = m_ctlClientTree.InsertItem( strNames, 0, 0);
		m_ctlClientTree.SetItemData(hClientItem,(DWORD_PTR)nUUIDArrayIdx);
		
		strTmp.Format( _T("Client UUID: %s"), (*iCliUUIDIter) );
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);
		
		strTmp.Format( _T("Times seen: %d"), nTimesSeen );
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);

		strTmp.Format(_T("HD Results: %d"),nHDCnt);
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);

		strTmp.Format(_T("CPU Measure Results: %d"),nCPUMeasureCnt);
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);

		strTmp.Format(_T("Memory Results: %d"),nMemCnt);
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);

		strTmp.Format(_T("Ping Results: %d"),nPingCnt);
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);

		strTmp.Format(_T("TCP Connection Results: %d"),nTCPConCnt);
		m_ctlClientTree.InsertItem(strTmp,0,0,hClientItem);
	}
}

void CSolidSBCClientListView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if ( m_ctlClientTree.m_hWnd )
		m_ctlClientTree.MoveWindow(10,10,cx-20,cy-20,1);
}

void CSolidSBCClientListView::OnNMRClickClientTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint orgPoint;
	GetCursorPos(&orgPoint);
	
	CPoint point;
	point = orgPoint;
	m_ctlClientTree.ScreenToClient(&point);

	UINT uFlags;
	HTREEITEM hItem = m_ctlClientTree.HitTest(point, &uFlags);

	if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		m_ctlClientTree.Select(hItem, TVGN_CARET);
	}
	else { // not on item
	}

	CMenu contextMenu;
	contextMenu.LoadMenu(IDR_CLIENT_LIST_CONTEXT_MENU);
	CMenu* pContextPopUp = contextMenu.GetSubMenu(0);

	if ( pContextPopUp )
		pContextPopUp->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON, orgPoint.x,orgPoint.y, AfxGetMainWnd() );

	*pResult = 0;
}

CString CSolidSBCClientListView::GetSelectedClientUUID(void)
{
	HTREEITEM hSelItem    = m_ctlClientTree.GetSelectedItem();

	if ( !hSelItem)
		return _T("");
	
	HTREEITEM hParentItem = hSelItem;
	HTREEITEM hPrevItem   = hSelItem;
	
	do{
		hPrevItem = hParentItem;
		hParentItem = m_ctlClientTree.GetParentItem( hParentItem ) ;
	} while ( hParentItem != NULL );

	hParentItem = hPrevItem;
	int nArrayIdx = (int)m_ctlClientTree.GetItemData( hParentItem );

	return m_arUUIDs.GetAt(nArrayIdx);
}

void CSolidSBCClientListView::EmptyList(void)
{
	m_ctlClientTree.DeleteAllItems();
	m_arUUIDs.RemoveAll();
}

void CSolidSBCClientListView::OnClientlistcontextmenuViewhistory()
{
	theApp.OnClientHistoryView( GetSelectedClientUUID() );
}

void CSolidSBCClientListView::OnViewresultsHarddrive()
{
	theApp.OnHDResultView( GetSelectedClientUUID() );
}

void CSolidSBCClientListView::OnViewresultsCpumeasure()
{
	theApp.OnCPUMeasureResultView( GetSelectedClientUUID() );
}

void CSolidSBCClientListView::OnViewresultsMemory()
{
	theApp.OnMemoryResultView( GetSelectedClientUUID() );
}

void CSolidSBCClientListView::OnViewresultsPing()
{
	theApp.OnPingResultView( GetSelectedClientUUID() );
}

void CSolidSBCClientListView::OnViewresultsTcpconnect()
{
	theApp.OnTCPResultView( GetSelectedClientUUID() );
}

void CSolidSBCClientListView::OnClientlistcontextmenuDatabasestats()
{
	// TODO: Add your command handler code here
}
