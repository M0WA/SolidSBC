// SolidSBCHDResultView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCHDResultView.h"


// CSolidSBCHDResultView

IMPLEMENT_DYNCREATE(CSolidSBCHDResultView, CFormView)

CSolidSBCHDResultView::CSolidSBCHDResultView()
	: CFormView(CSolidSBCHDResultView::IDD)
	, m_strUUID(_T(""))
	, m_nIdentity(-1)
{
}

CSolidSBCHDResultView::~CSolidSBCHDResultView()
{
}

void CSolidSBCHDResultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HD_RESULT_LIST, m_ctlHDResultList);
	DDX_Control(pDX, IDC_HD_STATIC, m_ctlHDStatic);
}

BEGIN_MESSAGE_MAP(CSolidSBCHDResultView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_HD_RESULT_LIST, &CSolidSBCHDResultView::OnNMRClickHdResultList)
END_MESSAGE_MAP()


// CSolidSBCHDResultView diagnostics

#ifdef _DEBUG
void CSolidSBCHDResultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCHDResultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCHDResultView message handlers

void CSolidSBCHDResultView::SetClientUUID(CString strUUID, int nIdentity)
{
	m_strUUID = strUUID;
	m_nIdentity = nIdentity;

	CString strTitle = _T("");
	strTitle.Format(_T("harddrive results ( uuid: %s )"),m_strUUID);
	GetDocument()->SetTitle(strTitle);

	RefreshList();
}

void CSolidSBCHDResultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);
	
	m_ctlHDResultList.SetExtendedStyle( m_ctlHDResultList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_ctlHDResultList.InsertColumn(0, _T("type")                , 0, 60);
	m_ctlHDResultList.InsertColumn(1, _T("duration ( sec )")    , 0, 120);
	m_ctlHDResultList.InsertColumn(2, _T("size ( mb, bytes )")  , 0, 120);
	m_ctlHDResultList.InsertColumn(3, _T("speed ( mb/sec )") , 0, 120);
	m_ctlHDResultList.InsertColumn(4, _T("wait (sec)")          , 0, 60);
	m_ctlHDResultList.InsertColumn(5, _T("time")                , 0, 130);

	CRect rectWnd;
	GetClientRect(&rectWnd);	
	m_ctlHDResultList.MoveWindow(10,10,rectWnd.right-20,rectWnd.bottom-80,1);
	
	CRect rectList, rectStatic;
	m_ctlHDResultList.GetClientRect(&rectList);
	m_ctlHDStatic.GetClientRect(&rectStatic);
	m_ctlHDStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	
	m_ctlHDStatic.SetWindowText(_T(""));
}


void CSolidSBCHDResultView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if ( (m_ctlHDResultList.m_hWnd) && (m_ctlHDStatic.m_hWnd) ){

		m_ctlHDResultList.MoveWindow(10,10,cx-20,cy-80);
		
		CRect rectList, rectStatic;
		m_ctlHDResultList .GetClientRect(&rectList);
		m_ctlHDStatic.GetClientRect(&rectStatic);
		m_ctlHDStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	}
}

void CSolidSBCHDResultView::RefreshList(void)
{
	CString strStatusBar = _T("");
	strStatusBar.Format( _T("Fetching harddrive results for client uuid: %s..."), m_strUUID);
	theApp.m_pMainFrame->SetStatusBar(strStatusBar);

	m_ctlHDResultList.DeleteAllItems();
	
	ULONGLONG ullTotalRead = 0, ullTotalWrite = 0;
	long double dTotalReadDuration = 0.0f, dTotalWriteDuration = 0.0f;
	double dReadMin = 0.0f, dReadMax = 0.0f, dWriteMin = 0.0f, dWriteMax = 0.0f;

	std::vector<SSBC_CLIENT_HD_RESULT> hdResults = g_cDatabaseConnection.GetClientHDResults(m_strUUID, m_nIdentity);
	std::vector<SSBC_CLIENT_HD_RESULT>::iterator iIter;
	for ( iIter = hdResults.begin(); iIter < hdResults.end(); iIter++ )
	{
		long double dDuration = static_cast<long double>( _ttof( (*iIter).strDuration ) );
		long double dBytes = static_cast<long double>( _ttol( (*iIter).strBytes ) );
		CString strBytes;
		strBytes.Format(_T("%.03f, %d"), (dBytes/1024.0f)/1024.0f,_ttol((*iIter).strBytes));

		double dSpeed = ((dBytes/dDuration)/1024.0f)/1024.0f;
		CString strSpeed = _T("");
		strSpeed.Format(_T("%.06f"), dSpeed );
		
		CString strType = (*iIter).strType;
		switch ( _ttoi( (*iIter).strType) )
		{
		case 0: //SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ
			strType = _T("read");
			ullTotalRead += static_cast<ULONGLONG>( _ttol( (*iIter).strBytes ) );
			dTotalReadDuration += dDuration;
			if ( dSpeed > dReadMax )
				dReadMax = dSpeed;
			else if ( dSpeed < dReadMin )
				dReadMin = dSpeed;
			if ( dReadMin == 0.0f )
				dReadMin = dSpeed;
			break;
		case 1: //SSBC_TEST_HARDDRIVE_RESULT_TYPE_WRITE
			strType = _T("write");
			ullTotalWrite += static_cast<ULONGLONG>( _ttol( (*iIter).strBytes ) );
			dTotalWriteDuration += dDuration;
			if ( dSpeed > dWriteMax )
				dWriteMax = dSpeed;
			else if ( dSpeed < dWriteMin )
				dWriteMin = dSpeed;
			if ( dWriteMin == 0.0f )
				dWriteMin = dSpeed;
			break;
		case 2: //SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ_INIT
			strType = _T("init read");
			break;
		default:
			break;
		}

		//insert into list
		int nItem = m_ctlHDResultList.InsertItem( m_ctlHDResultList.GetItemCount(), strType );
		m_ctlHDResultList.SetItemText( nItem, 1, (*iIter).strDuration );
		m_ctlHDResultList.SetItemText( nItem, 2, strBytes             );
		m_ctlHDResultList.SetItemText( nItem, 3, strSpeed             );
		m_ctlHDResultList.SetItemText( nItem, 4, (*iIter).strWait     );
		m_ctlHDResultList.SetItemText( nItem, 5, (*iIter).strTime     );
	}
	
	long double dAvgRead = 0.0f;
	long double dTotalRead  = static_cast<long double>(ullTotalRead);
	long double dTotalWrite = static_cast<long double>(ullTotalWrite);

	if ( dTotalReadDuration > 0.0f ){
		dAvgRead = dTotalRead / dTotalReadDuration;}
	
	long double dAvgWrite = 0.0f;
	if ( dTotalWriteDuration > 0.0f ){
		dAvgWrite = dTotalWrite / dTotalWriteDuration;}

	CString staticTxt = _T("");
	staticTxt.Format( 
		_T("# results: %d")
		_T("\nread %0.03f mb ( %llu bytes ) in %.06f secs")
		_T("\n avg/min/max mb/sec: %.03f / %.03f / %.03f")
		_T("\nwritten %0.03f mb ( %llu bytes ) in %.06f secs")
		_T("\n avg/min/max mb/sec: %.03f / %.03f / %.03f")
		, hdResults.size()
		, (dTotalRead/1024.0f)/1024.0f, ullTotalRead, dTotalReadDuration
		, (dAvgRead/1024.0f)/1024.0f, dReadMin, dReadMax
		, (dTotalWrite/1024.0f)/1024.0f, ullTotalWrite, dTotalWriteDuration
		, (dAvgWrite/1024.0f)/1024.0f, dWriteMin, dWriteMax  );
	m_ctlHDStatic.SetWindowText(staticTxt);

	theApp.m_pMainFrame->SetStatusBar(_T("Ready"));
}


void CSolidSBCHDResultView::OnNMRClickHdResultList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint orgPoint;
	GetCursorPos(&orgPoint);

	CMenu contextMenu;
	contextMenu.LoadMenu(IDR_EXPORT_RESULTS_MENU);
	CResultContextMenu* pContextPopUp = (CResultContextMenu*)contextMenu.GetSubMenu(0);

	if ( pContextPopUp ) {

		pContextPopUp->AddIdentities( m_strUUID, SSBC_HARDDRIVE, m_nIdentity );

		int nSel = (int)pContextPopUp->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, orgPoint.x,orgPoint.y, this );

		std::vector<SSBC_CLIENT_IDENTITY> vIdentities = g_cDatabaseConnection.GetClientIdentities(m_strUUID);
		if ( ( nSel >= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX ) && ( nSel < (SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX + (int)vIdentities.size() ) ) )
		{
			//filter single run
			nSel -= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX;
			theApp.OnHDResultView(m_strUUID,nSel);

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

void CSolidSBCHDResultView::OnFilterDate()
{
	CSolidSBCTimeChooseDlg timeDlg;
	
	if ( timeDlg.DoModal() == IDOK){
	}
}

void CSolidSBCHDResultView::OnExportSelectedResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("harddrive_part_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Seperated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName(),true);
	}
}

void CSolidSBCHDResultView::OnExportAllResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("harddrive_all_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Separated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName());
	}
}

void CSolidSBCHDResultView::ExportResults(CString strFileName,bool bSelectedOnly)
{
	POSITION pos = 0;
	int nItem = -1;
	if ( bSelectedOnly )
		pos = m_ctlHDResultList.GetFirstSelectedItemPosition();
	else {
		nItem = m_ctlHDResultList.GetNextItem(-1,LVNI_ALL);
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
			strMsg.Format(_T("HD CSV File: %s"),pszErr);
			AfxMessageBox(strMsg);
		}
#endif
		return;
	}
	END_CATCH
	
	while ( (pos && bSelectedOnly) || ( (nItem >= 0) && !bSelectedOnly ) )  
	{

		if ( bSelectedOnly ){
			nItem = m_ctlHDResultList.GetNextSelectedItem(pos);
		}

		CString strType     = m_ctlHDResultList.GetItemText(nItem,0);
		CString strDuration = m_ctlHDResultList.GetItemText(nItem,1);
		CString strBytes    = m_ctlHDResultList.GetItemText(nItem,2);
		CString strSpeed    = m_ctlHDResultList.GetItemText(nItem,3);
		CString strWait     = m_ctlHDResultList.GetItemText(nItem,4);

		CString strCSVLine = _T("");
		strCSVLine.Format(
			_T("%s%s%s%s%s%s%s%s%s%s"),
			strType, SSCB_CSV_COMMA_STRING,
			strDuration, SSCB_CSV_COMMA_STRING,
			strBytes, SSCB_CSV_COMMA_STRING,
			strSpeed, SSCB_CSV_COMMA_STRING,
			strWait, SSCB_CSV_LINEEND_STRING
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
				strMsg.Format(_T("HD CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
		}
		END_CATCH

		delete [] pszLine;
		pszLine = NULL;

		if ( !bSelectedOnly ) {
			nItem = m_ctlHDResultList.GetNextItem(nItem,LVNI_ALL);
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
				strMsg.Format(_T("HD CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
	}
	END_CATCH
}