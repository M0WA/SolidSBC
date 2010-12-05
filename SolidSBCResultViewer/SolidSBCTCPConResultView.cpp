// SolidSBCTCPConResultView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCTCPConResultView.h"


// CSolidSBCTCPConResultView

IMPLEMENT_DYNCREATE(CSolidSBCTCPConResultView, CFormView)

CSolidSBCTCPConResultView::CSolidSBCTCPConResultView()
	: CFormView(CSolidSBCTCPConResultView::IDD)
	, m_strUUID(_T(""))
	, m_nIdentity(-1)
{

}

CSolidSBCTCPConResultView::~CSolidSBCTCPConResultView()
{
}

void CSolidSBCTCPConResultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCP_RESULT_LIST, m_ctlTcpResultList);
	DDX_Control(pDX, IDC_TCP_RESULT_STATIC, m_ctlTcpResultStatic);
}

BEGIN_MESSAGE_MAP(CSolidSBCTCPConResultView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_TCP_RESULT_LIST, &CSolidSBCTCPConResultView::OnNMRClickTcpResultList)
END_MESSAGE_MAP()


// CSolidSBCTCPConResultView diagnostics

#ifdef _DEBUG
void CSolidSBCTCPConResultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCTCPConResultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCTCPConResultView message handlers

void CSolidSBCTCPConResultView::SetClientUUID(CString strUUID, int nIdentity)
{
	m_strUUID = strUUID;
	m_nIdentity = nIdentity;
	
	CString strTitle = _T("");
	strTitle.Format(_T("tcp connect results ( uuid: %s )"),m_strUUID);
	GetDocument()->SetTitle(strTitle);

	RefreshList();
}

void CSolidSBCTCPConResultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);
	
	m_ctlTcpResultList.SetExtendedStyle( m_ctlTcpResultList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_ctlTcpResultList.InsertColumn(0,_T("Handshake (ms)"),0,100);
	m_ctlTcpResultList.InsertColumn(1,_T("Time"),0,100);

	CRect rectWnd;
	GetClientRect(&rectWnd);	
	m_ctlTcpResultList.MoveWindow(10,10,rectWnd.right-20,rectWnd.bottom-80,1);
	
	CRect rectList, rectStatic;
	m_ctlTcpResultList .GetClientRect(&rectList);
	m_ctlTcpResultStatic.GetClientRect(&rectStatic);
	m_ctlTcpResultStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	
	m_ctlTcpResultStatic.SetWindowText(_T(""));
}

void CSolidSBCTCPConResultView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if ( (m_ctlTcpResultList.m_hWnd) && (m_ctlTcpResultStatic.m_hWnd) ){

		m_ctlTcpResultList.MoveWindow(10,10,cx-20,cy-80);
		
		CRect rectList, rectStatic;
		m_ctlTcpResultList .GetClientRect(&rectList);
		m_ctlTcpResultStatic.GetClientRect(&rectStatic);
		m_ctlTcpResultStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	}
}

void CSolidSBCTCPConResultView::RefreshList(void)
{
	m_ctlTcpResultList.DeleteAllItems();

	std::vector<SSBC_CLIENT_TCPCON_RESULT> tcpResults = g_cDatabaseConnection.GetClientTCPConResults(m_strUUID,m_nIdentity);
	std::vector<SSBC_CLIENT_TCPCON_RESULT>::iterator iIter;
	
	long lAvgHandshakeDuration = 0, lMinHandshakeDuration = 0, lMaxHandshakeDuration = 0;
	for ( iIter = tcpResults.begin(); iIter < tcpResults.end(); iIter++ )
	{
		long lCurrentDuration = _ttol((*iIter).strHandshakeDuration);
		lAvgHandshakeDuration += lCurrentDuration;
		
		if ( lCurrentDuration > lMaxHandshakeDuration )
			lMaxHandshakeDuration = lCurrentDuration;
		else if ( lCurrentDuration < lMinHandshakeDuration )
			lMinHandshakeDuration = lCurrentDuration;
		if ( lMinHandshakeDuration == 0 )
			lMinHandshakeDuration = lCurrentDuration;
		
		//insert into list
		int nItem = m_ctlTcpResultList.InsertItem( m_ctlTcpResultList.GetItemCount(), (*iIter).strHandshakeDuration );
		m_ctlTcpResultList.SetItemText( nItem, 1, (*iIter).strTime );
	}

	if ( tcpResults.size() )
		lAvgHandshakeDuration /= (long)tcpResults.size();

	CString staticTxt = _T("");
	staticTxt.Format( 
		_T("# results: %d\n")
		_T("avg/min/max handshake ms: %d/%d/%d\n")
		, tcpResults.size()
		, lAvgHandshakeDuration, lMinHandshakeDuration, lMaxHandshakeDuration
		);
	m_ctlTcpResultStatic.SetWindowText(staticTxt);
}

void CSolidSBCTCPConResultView::OnNMRClickTcpResultList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint orgPoint;
	GetCursorPos(&orgPoint);

	CMenu contextMenu;
	contextMenu.LoadMenu(IDR_EXPORT_RESULTS_MENU);
	CResultContextMenu* pContextPopUp = (CResultContextMenu*)contextMenu.GetSubMenu(0);

	if ( pContextPopUp ) {

		pContextPopUp->AddIdentities( m_strUUID, SSBC_TCPCON, m_nIdentity );

		int nSel = (int)pContextPopUp->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, orgPoint.x,orgPoint.y, this );

		std::vector<SSBC_CLIENT_IDENTITY> vIdentities = g_cDatabaseConnection.GetClientIdentities(m_strUUID);
		if ( ( nSel >= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX ) && ( nSel < (SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX + (int)vIdentities.size() ) ) )
		{
			//filter single run
			nSel -= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX;
			theApp.OnTCPResultView(m_strUUID,nSel);
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

void CSolidSBCTCPConResultView::OnFilterDate()
{
	
}

void CSolidSBCTCPConResultView::OnExportSelectedResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("tcpcon_part_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Seperated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName(),true);
	}
}

void CSolidSBCTCPConResultView::OnExportAllResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("tcpcon_all_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Separated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName());
	}
}

void CSolidSBCTCPConResultView::ExportResults(CString strFileName,bool bSelectedOnly)
{
	POSITION pos = 0;
	int nItem = -1;
	if ( bSelectedOnly )
		pos = m_ctlTcpResultList.GetFirstSelectedItemPosition();
	else {
		nItem = m_ctlTcpResultList.GetNextItem(-1,LVNI_ALL);
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
			strMsg.Format(_T("TCPCon CSV File: %s"),pszErr);
			AfxMessageBox(strMsg);
		}
#endif
		return;
	}
	END_CATCH
	
	while ( (pos && bSelectedOnly) || ( (nItem >= 0) && !bSelectedOnly ) )  
	{

		if ( bSelectedOnly ){
			nItem = m_ctlTcpResultList.GetNextSelectedItem(pos);
		}
		
		CString strHandshakeDuration = m_ctlTcpResultList.GetItemText(nItem,0);

		CString strCSVLine = _T("");
		strCSVLine.Format(
			_T("%s%s"),
			strHandshakeDuration, SSCB_CSV_LINEEND_STRING
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
				strMsg.Format(_T("TCPCon CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
		}
		END_CATCH

		delete [] pszLine;
		pszLine = NULL;

		if ( !bSelectedOnly ) {
			nItem = m_ctlTcpResultList.GetNextItem(nItem,LVNI_ALL);
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
				strMsg.Format(_T("TCPCon CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
	}
	END_CATCH
}