// SolidSBCMemoryResultView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCMemoryResultView.h"
#include <map>


// CSolidSBCMemoryResultView

IMPLEMENT_DYNCREATE(CSolidSBCMemoryResultView, CFormView)

CSolidSBCMemoryResultView::CSolidSBCMemoryResultView()
	: CFormView(CSolidSBCMemoryResultView::IDD)
	, m_strUUID(_T(""))
	, m_nIdentity(-1)
{

}

CSolidSBCMemoryResultView::~CSolidSBCMemoryResultView()
{
}

void CSolidSBCMemoryResultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEM_RESULT_LIST, m_ctlMemResultList);
	DDX_Control(pDX, IDC_AVG_SPEED_STATIC, m_ctlAvgSpeedStatic);
}

BEGIN_MESSAGE_MAP(CSolidSBCMemoryResultView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_MEM_RESULT_LIST, &CSolidSBCMemoryResultView::OnNMRClickMemResultList)
	ON_COMMAND(ID_EXPORTRESULTS_PLOT, &CSolidSBCMemoryResultView::OnExportresultsPlot)
END_MESSAGE_MAP()


// CSolidSBCMemoryResultView diagnostics

#ifdef _DEBUG
void CSolidSBCMemoryResultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCMemoryResultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCMemoryResultView message handlers


void CSolidSBCMemoryResultView::SetClientUUID(CString strUUID, int nIdentity)
{
	m_strUUID = strUUID;
	m_nIdentity = nIdentity;
	
	CString strTitle = _T("");
	strTitle.Format(_T("memory results ( uuid: %s )"),m_strUUID);
	GetDocument()->SetTitle(strTitle);

	RefreshList();
}

void CSolidSBCMemoryResultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit(FALSE);
	
	m_ctlMemResultList.SetExtendedStyle( m_ctlMemResultList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_ctlMemResultList.InsertColumn(0,_T("duration ( sec )"),0,100);
	m_ctlMemResultList.InsertColumn(1,_T("size ( mb )"),0,100);
	m_ctlMemResultList.InsertColumn(2,_T("speed ( mb/sec )"),0,130);
	m_ctlMemResultList.InsertColumn(3,_T("time"),0,130);
	
	CRect rectWnd;
	GetClientRect(&rectWnd);	
	m_ctlMemResultList.MoveWindow(10,10,rectWnd.right-20,rectWnd.bottom-80,1);
	
	CRect rectList, rectStatic;
	m_ctlMemResultList .GetClientRect(&rectList);
	m_ctlAvgSpeedStatic.GetClientRect(&rectStatic);
	m_ctlAvgSpeedStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	
	m_ctlAvgSpeedStatic.SetWindowText(_T(""));
}


void CSolidSBCMemoryResultView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if ( (m_ctlMemResultList.m_hWnd) && (m_ctlAvgSpeedStatic.m_hWnd) ){

		m_ctlMemResultList.MoveWindow(10,10,cx-20,cy-80);
		
		CRect rectList, rectStatic;
		m_ctlMemResultList .GetClientRect(&rectList);
		m_ctlAvgSpeedStatic.GetClientRect(&rectStatic);
		m_ctlAvgSpeedStatic.MoveWindow( 10, rectList.bottom + 15, 10 + (rectStatic.right - rectStatic.left), (rectList.bottom + 15) + (rectStatic.bottom - rectStatic.top), 1);
	}
}

void CSolidSBCMemoryResultView::RefreshList(void)
{
	CString strStatusBar = _T("");
	strStatusBar.Format( _T("Fetching memory results for client uuid: %s..."), m_strUUID);
	theApp.m_pMainFrame->SetStatusBar(strStatusBar);

	ULONGLONG ullTotalBytes  = 0;
	double    dTotalDuration = 0.0f;
	double    dMaxSpeed = 0.0f;
	double    dMinSpeed = 0.0f;
	
	m_ctlMemResultList.DeleteAllItems();
	std::vector<SSBC_CLIENT_MEM_RESULT> memResults = g_cDatabaseConnection.GetClientMemResults(m_strUUID,m_nIdentity);
	std::vector<SSBC_CLIENT_MEM_RESULT>::iterator iIter;
	for ( iIter = memResults.begin(); iIter < memResults.end(); iIter++ )
	{
		//calculate speed
		long   nBytes    = _ttol( (*iIter).strBytes );
		double dBytes    = static_cast<double>(nBytes);
		double dDuration = _ttof( (*iIter).strMallocZeroDuration );
		double dSpeed    = ((dBytes/dDuration)/1024.0f)/1024.0f;
		
		if ( dSpeed > dMaxSpeed )
			dMaxSpeed = dSpeed;
		else if ( dSpeed < dMinSpeed )
			dMinSpeed = dSpeed;
		
		if ( dMinSpeed == 0.0f )
			dMinSpeed = dSpeed;

		CString strSpeed = _T("");
		strSpeed.Format(_T("%f"),dSpeed);

		CString strSize = _T("");
		strSize.Format( _T("%.06f"), (dBytes/1024.0f)/1024.0f );

		//insert into list
		int nItem = m_ctlMemResultList.InsertItem( m_ctlMemResultList.GetItemCount(), (*iIter).strMallocZeroDuration );
		m_ctlMemResultList.SetItemText( nItem, 1, strSize           );
		m_ctlMemResultList.SetItemText( nItem, 2, strSpeed          );
		m_ctlMemResultList.SetItemText( nItem, 3, (*iIter).strTime  );

		//set counters
		ullTotalBytes  += nBytes;
		dTotalDuration += dDuration;
	}
	
	long double dAvgSpeed = 0.0f;
	long double dTotalBytes = static_cast<long double>(ullTotalBytes);
	long double dTotalMegaBytes = (dTotalBytes/1024.0f)/1024.0f;
	if ( dTotalDuration != 0.0f )
		dAvgSpeed = static_cast<long double>(ullTotalBytes) / static_cast<long double>(dTotalDuration);

	CString staticTxt = _T("");
	staticTxt.Format( _T("# results: %d\ntotal size: %.06f mb ( %llu bytes )\ntotal duration: %.06f secs\nmin/max: %.06f mb/sec / %.06f mb/sec\navg: %.06f mb/sec ( %.01f bytes/sec )\n") 
		, memResults.size(), dTotalMegaBytes, ullTotalBytes, dTotalDuration, dMinSpeed, dMaxSpeed, ( ( dAvgSpeed / 1024.0f ) /1024.0f ), dAvgSpeed );
	m_ctlAvgSpeedStatic.SetWindowText(staticTxt);

	theApp.m_pMainFrame->SetStatusBar(_T("Ready"));
}

void CSolidSBCMemoryResultView::OnNMRClickMemResultList(NMHDR *pNMHDR, LRESULT *pResult)
{
		CPoint orgPoint;
	GetCursorPos(&orgPoint);

	CMenu contextMenu;
	contextMenu.LoadMenu(IDR_EXPORT_RESULTS_MENU);
	CResultContextMenu* pContextPopUp = (CResultContextMenu*)contextMenu.GetSubMenu(0);

	if ( pContextPopUp ) {

		pContextPopUp->AddIdentities( m_strUUID, SSBC_MEMORY, m_nIdentity );

		int nSel = (int)pContextPopUp->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, orgPoint.x,orgPoint.y, this );

		std::vector<SSBC_CLIENT_IDENTITY> vIdentities = g_cDatabaseConnection.GetClientIdentities(m_strUUID);
		if ( ( nSel >= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX ) && ( nSel < (SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX + (int)vIdentities.size() ) ) )
		{
			//filter single run
			nSel -= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX;
			theApp.OnMemoryResultView(m_strUUID,nSel);

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
			case ID_EXPORTRESULTS_PLOT:
				OnExportresultsPlot();
				break;
			default:
				//could not determine action
				break;
			}
		}
	}

	*pResult = 0;
}

void CSolidSBCMemoryResultView::OnFilterDate()
{
	CSolidSBCTimeChooseDlg timeDlg;
	
	if ( timeDlg.DoModal() == IDOK){
	}
}

void CSolidSBCMemoryResultView::OnExportSelectedResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("memory_part_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Seperated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName(),true);
	}
}

void CSolidSBCMemoryResultView::OnExportAllResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("memory_all_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Separated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName());
	}
}

void CSolidSBCMemoryResultView::ExportResults(CString strFileName,bool bSelectedOnly)
{
	POSITION pos = 0;
	int nItem = -1;
	if ( bSelectedOnly )
		pos = m_ctlMemResultList.GetFirstSelectedItemPosition();
	else {
		nItem = m_ctlMemResultList.GetNextItem(-1,LVNI_ALL);
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
			strMsg.Format(_T("Memory CSV File: %s"),pszErr);
			AfxMessageBox(strMsg);
		}
#endif
		return;
	}
	END_CATCH
	
	while ( (pos && bSelectedOnly) || ( (nItem >= 0) && !bSelectedOnly ) )  
	{

		if ( bSelectedOnly ){
			nItem = m_ctlMemResultList.GetNextSelectedItem(pos);
		}

		CString strMallocZeroDuration = m_ctlMemResultList.GetItemText(nItem,0);
		CString strSize  = m_ctlMemResultList.GetItemText(nItem,1);
		CString strSpeed = m_ctlMemResultList.GetItemText(nItem,2);

		CString strCSVLine = _T("");
		strCSVLine.Format(
			_T("%s%s%s%s%s%s"),
			strMallocZeroDuration, SSCB_CSV_COMMA_STRING,
			strSize,	SSCB_CSV_COMMA_STRING,
			strSpeed, SSCB_CSV_LINEEND_STRING
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
				strMsg.Format(_T("Memory CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
		}
		END_CATCH

		delete [] pszLine;
		pszLine = NULL;

		if ( !bSelectedOnly ) {
			nItem = m_ctlMemResultList.GetNextItem(nItem,LVNI_ALL);
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
				strMsg.Format(_T("Memory CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
	}
	END_CATCH
}

void CSolidSBCMemoryResultView::OnExportresultsPlot(void)
{
	USES_CONVERSION;
	
	std::map<int,int> mapCoordinates;

	std::vector<SSBC_CLIENT_MEM_RESULT> memResults = g_cDatabaseConnection.GetClientMemResults(m_strUUID,m_nIdentity);
	std::vector<SSBC_CLIENT_MEM_RESULT>::iterator iIter;

	unsigned int nPos = 0;	
	for ( iIter = memResults.begin(); iIter < memResults.end(); iIter++ )
	{
		double dSpeed = -1.0f;
		double dMallocZeroDuration = _ttof((*iIter).strMallocZeroDuration);
		if ( dMallocZeroDuration != 0.0f )
		{
			dSpeed = (_ttof((*iIter).strBytes)/(1024.0f * 1024.0f)) / dMallocZeroDuration;
		}
		int nDuration = static_cast<int>(dSpeed);
		mapCoordinates[nPos] = nDuration;
		nPos++;
	}
	
	std::map<int,std::map<int,int>>			  mapMapCoordinates;
	std::map<int,COLORREF>					  mapColors;
	std::map<int,std::pair<CString,CString>>  mapPairUnits;
	mapMapCoordinates[0] = mapCoordinates;
	mapColors[0]         = RGB( 0, 0, 255 );
	mapPairUnits[0]      = std::pair<CString,CString>(_T("result"),_T("mb/s"));

	theApp.OnGraphPlotterView(m_strUUID,m_nIdentity,mapMapCoordinates,mapColors,mapPairUnits);
}
