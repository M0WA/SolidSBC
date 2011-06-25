// SolidSBCPingResultView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCPingResultView.h"


// CSolidSBCPingResultView

IMPLEMENT_DYNCREATE(CSolidSBCPingResultView, CFormView)

CSolidSBCPingResultView::CSolidSBCPingResultView()
	: CFormView(CSolidSBCPingResultView::IDD)
	, m_strUUID(_T(""))
	, m_nIdentity(-1)
{

}

CSolidSBCPingResultView::~CSolidSBCPingResultView()
{
}

void CSolidSBCPingResultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PING_RESULT_LIST, m_ctlPingResultList);
	DDX_Control(pDX, IDC_PING_RESULT_STATIC, m_ctlPingStatic);
}

BEGIN_MESSAGE_MAP(CSolidSBCPingResultView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_PING_RESULT_LIST, &CSolidSBCPingResultView::OnNMRClickPingResultList)
END_MESSAGE_MAP()


// CSolidSBCPingResultView diagnostics

#ifdef _DEBUG
void CSolidSBCPingResultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCPingResultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCPingResultView message handlers

void CSolidSBCPingResultView::SetClientUUID(CString strUUID, int nIdentity)
{
	m_strUUID = strUUID;
	m_nIdentity = nIdentity;
	
	CString strTitle = _T("");
	strTitle.Format(_T("ping results ( uuid: %s )"),m_strUUID);
	GetDocument()->SetTitle(strTitle);

	RefreshList();
}

void CSolidSBCPingResultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);
	
	m_ctlPingResultList.SetExtendedStyle( m_ctlPingResultList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_ctlPingResultList.InsertColumn(0,_T("RTT (ms)"),0,100);
	m_ctlPingResultList.InsertColumn(1,_T("ICMP Flags"),0,100);
	m_ctlPingResultList.InsertColumn(2,_T("Time"),0,130);

	CRect rectWnd;
	GetClientRect(&rectWnd);	
	m_ctlPingResultList.MoveWindow(10,10,rectWnd.right-20,rectWnd.bottom-80,1);
	
	CRect rectList, rectStatic;
	m_ctlPingResultList .GetClientRect(&rectList);
	m_ctlPingStatic.GetClientRect(&rectStatic);
	m_ctlPingStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	
	m_ctlPingStatic.SetWindowText(_T(""));
}

void CSolidSBCPingResultView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if ( (m_ctlPingResultList.m_hWnd) && (m_ctlPingStatic.m_hWnd) ){

		m_ctlPingResultList.MoveWindow(10,10,cx-20,cy-80);
		
		CRect rectList, rectStatic;
		m_ctlPingResultList .GetClientRect(&rectList);
		m_ctlPingStatic.GetClientRect(&rectStatic);
		m_ctlPingStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	}
}

void CSolidSBCPingResultView::RefreshList(void)
{
	CString strStatusBar = _T("");
	strStatusBar.Format( _T("Fetching ping results for client uuid: %s..."), m_strUUID);
	theApp.m_pMainFrame->SetStatusBar(strStatusBar);
	
	m_ctlPingResultList.DeleteAllItems();

	std::vector<SSBC_CLIENT_PING_RESULT> pingResults = g_cDatabaseConnection.GetClientPingResults(m_strUUID,m_nIdentity);
	std::vector<SSBC_CLIENT_PING_RESULT>::iterator iIter;
	
	long lAvgDuration = 0, lMinDuration = 0, lMaxDuration = 0;
	for ( iIter = pingResults.begin(); iIter < pingResults.end(); iIter++ )
	{
		long lCurrentDuration = _ttol((*iIter).strRTTDuration);
		lAvgDuration += lCurrentDuration;
		
		if ( lCurrentDuration > lMaxDuration )
			lMaxDuration = lCurrentDuration;
		else if ( lCurrentDuration < lMinDuration )
			lMinDuration = lCurrentDuration;
		if ( lMinDuration == 0 )
			lMinDuration = lCurrentDuration;

		//insert into list
		int nItem = m_ctlPingResultList.InsertItem( m_ctlPingResultList.GetItemCount(), (*iIter).strRTTDuration );
		m_ctlPingResultList.SetItemText( nItem, 1, (*iIter).strICMPOpts );
		m_ctlPingResultList.SetItemText( nItem, 2, (*iIter).strTime  );
	}
	
	if ( pingResults.size() )
		lAvgDuration /= (long)pingResults.size();

	CString staticTxt = _T("");
	staticTxt.Format( 
		_T("# results: %d\n")
		_T("avg/min/max rtt ms: %d/%d/%d\n")
		, pingResults.size()
		, lAvgDuration, lMinDuration, lMaxDuration );
	m_ctlPingStatic.SetWindowText(staticTxt);

	theApp.m_pMainFrame->SetStatusBar(_T("Ready"));
}

void CSolidSBCPingResultView::OnNMRClickPingResultList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint orgPoint;
	GetCursorPos(&orgPoint);

	CMenu contextMenu;
	contextMenu.LoadMenu(IDR_EXPORT_RESULTS_MENU);
	CResultContextMenu* pContextPopUp = (CResultContextMenu*)contextMenu.GetSubMenu(0);

	if ( pContextPopUp ) {

		pContextPopUp->AddIdentities( m_strUUID, SSBC_PING, m_nIdentity );

		int nSel = (int)pContextPopUp->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, orgPoint.x,orgPoint.y, this );

		std::vector<SSBC_CLIENT_IDENTITY> vIdentities = g_cDatabaseConnection.GetClientIdentities(m_strUUID);
		if ( ( nSel >= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX ) && ( nSel < (SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX + (int)vIdentities.size() ) ) )
		{
			//filter single run
			nSel -= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX;
			theApp.OnPingResultView(m_strUUID,nSel);

		} else {
			switch ( nSel )
			{
			//filter by date
			case ID_FILTER_DATE:
				OnFilterDate();
				break;
			//export selected items
			case ID_EXPORT_SELECTED:
				OnExportSelectedResults();
				break;
			//export all items
			case ID_EXPORT_ALL:
				OnExportAllResults();
				break;
			default:
				//could not determine action
				break;
			}
		}
	}

	*pResult = 0;
}

void CSolidSBCPingResultView::OnFilterDate()
{
	CSolidSBCTimeChooseDlg timeDlg;
	
	if ( timeDlg.DoModal() == IDOK){
	}
}

void CSolidSBCPingResultView::OnExportSelectedResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("ping_part_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Seperated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName(),true);
	}
}

void CSolidSBCPingResultView::OnExportAllResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("ping_all_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Separated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName());
	}
}

void CSolidSBCPingResultView::ExportResults(CString strFileName,bool bSelectedOnly)
{
	POSITION pos = 0;
	int nItem = -1;
	if ( bSelectedOnly )
		pos = m_ctlPingResultList.GetFirstSelectedItemPosition();
	else {
		nItem = m_ctlPingResultList.GetNextItem(-1,LVNI_ALL);
	}
	
	CFile cCSVFile;
	TRY
	{
		cCSVFile.Open(strFileName,CFile::modeReadWrite|CFile::modeCreate);
	}
	CATCH(CException,e)
	{
#ifdef _DEBUG
		{
			TCHAR pszErr[2048];
			e->GetErrorMessage(pszErr,2048);
			CString strMsg;
			strMsg.Format(_T("Ping CSV File: %s"),pszErr);
			AfxMessageBox(strMsg);
		}
#endif
		return;
	}
	END_CATCH
	
	while ( (pos && bSelectedOnly) || ( (nItem >= 0) && !bSelectedOnly ) )  
	{

		if ( bSelectedOnly ){
			nItem = m_ctlPingResultList.GetNextSelectedItem(pos);
		}

		CString strRTTDuration = m_ctlPingResultList.GetItemText(nItem,0);
		CString strICMPOpts    = m_ctlPingResultList.GetItemText(nItem,1);

		CString strCSVLine = _T("");
		strCSVLine.Format(
			_T("%s%s%s%s%s%s%s%s%s%s"),
			strRTTDuration, SSCB_CSV_COMMA_STRING,
			strICMPOpts, SSCB_CSV_LINEEND_STRING
			);
		
		USES_CONVERSION;
		CHAR* pszLine = new CHAR[strCSVLine.GetLength() + 1];
		ZeroMemory(pszLine,sizeof(CHAR) * (strCSVLine.GetLength() + 1) );
		strcpy_s(pszLine,strCSVLine.GetLength() + 1,T2A(strCSVLine));

		TRY
		{
			cCSVFile.Write(pszLine,strCSVLine.GetLength());
		}
		CATCH(CException,e)
		{
#ifdef _DEBUG
			{
				TCHAR pszErr[2048];
				e->GetErrorMessage(pszErr,2048);
				CString strMsg;
				strMsg.Format(_T("Ping CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
		}
		END_CATCH

		delete [] pszLine;
		pszLine = NULL;

		if ( !bSelectedOnly ) {
			nItem = m_ctlPingResultList.GetNextItem(nItem,LVNI_ALL);
		}
	}
	TRY
	{
		cCSVFile.Close();
	}
	CATCH(CException,e)
	{
#ifdef _DEBUG
			{
				TCHAR pszErr[2048];
				e->GetErrorMessage(pszErr,2048);
				CString strMsg;
				strMsg.Format(_T("Ping CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
	}
	END_CATCH
}