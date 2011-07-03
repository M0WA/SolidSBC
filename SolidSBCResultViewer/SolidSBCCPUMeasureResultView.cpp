// SolidSBCCPUMeasureResultView.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultViewer.h"
#include "SolidSBCCPUMeasureResultView.h"


// CSolidSBCCPUMeasureResultView

IMPLEMENT_DYNCREATE(CSolidSBCCPUMeasureResultView, CFormView)

CSolidSBCCPUMeasureResultView::CSolidSBCCPUMeasureResultView()
	: CFormView(CSolidSBCCPUMeasureResultView::IDD)
	, m_strUUID(_T(""))
	, m_nIdentity(-1)
{

}

CSolidSBCCPUMeasureResultView::~CSolidSBCCPUMeasureResultView()
{
}

void CSolidSBCCPUMeasureResultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CPUMEASURE_RESULT_LIST, m_ctlCPUMeasureList);
}

BEGIN_MESSAGE_MAP(CSolidSBCCPUMeasureResultView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_CPUMEASURE_RESULT_LIST, &CSolidSBCCPUMeasureResultView::OnNMRClickCpumeasureResultList)
END_MESSAGE_MAP()

// CSolidSBCCPUMeasureResultView diagnostics

#ifdef _DEBUG
void CSolidSBCCPUMeasureResultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSolidSBCCPUMeasureResultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSolidSBCCPUMeasureResultView message handlers

void CSolidSBCCPUMeasureResultView::SetClientUUID(CString strUUID, int nIdentity)
{
	m_strUUID   = strUUID;
	m_nIdentity = nIdentity;
	
	CString strTitle = _T("");
	strTitle.Format(_T("cpu measure results ( uuid: %s )"),m_strUUID);
	GetDocument()->SetTitle(strTitle);

	RefreshList();
}

void CSolidSBCCPUMeasureResultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	m_ctlCPUMeasureList.SetExtendedStyle( m_ctlCPUMeasureList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_ctlCPUMeasureList.InsertColumn(0, _T("add duration ( sec )"), 0, 120);
	m_ctlCPUMeasureList.InsertColumn(1, _T("div duration ( sec )"), 0, 120);
	m_ctlCPUMeasureList.InsertColumn(2, _T("all duration ( sec )"), 0, 120);
	m_ctlCPUMeasureList.InsertColumn(3, _T("add multiplier")      , 0, 120);
	m_ctlCPUMeasureList.InsertColumn(4, _T("div multiplier")      , 0, 120);
	m_ctlCPUMeasureList.InsertColumn(5, _T("time")                , 0, 130);
	
	CRect rectWnd;
	GetClientRect(&rectWnd);	
	m_ctlCPUMeasureList.MoveWindow(10,10,rectWnd.right-20,rectWnd.bottom-80,1);
}


void CSolidSBCCPUMeasureResultView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if ( (m_ctlCPUMeasureList.m_hWnd) ){
		m_ctlCPUMeasureList.MoveWindow(10,10,cx-20,cy-80);}

}

void CSolidSBCCPUMeasureResultView::RefreshList(void)
{
	CString strStatusBar = _T("");
	strStatusBar.Format( _T("Fetching cpu measure results for client uuid: %s..."), m_strUUID);
	theApp.m_pMainFrame->SetStatusBar(strStatusBar);

	m_ctlCPUMeasureList.DeleteAllItems();
	
	std::vector<SSBC_CLIENT_CPUMEASURE_RESULT> cpuResults = g_cDatabaseConnection.GetClientCPUMeasureResults(m_strUUID,m_nIdentity);

	std::vector<SSBC_CLIENT_CPUMEASURE_RESULT>::iterator iIter;
	for ( iIter = cpuResults.begin(); iIter < cpuResults.end(); iIter++ )
	{
		//insert into list
		int nItem = m_ctlCPUMeasureList.InsertItem( m_ctlCPUMeasureList.GetItemCount(), (*iIter).strAddDuration );
		m_ctlCPUMeasureList.SetItemText( nItem, 1, (*iIter).strDivDuration   );
		m_ctlCPUMeasureList.SetItemText( nItem, 2, (*iIter).strAllDuration   );
		m_ctlCPUMeasureList.SetItemText( nItem, 3, (*iIter).strAddMultiplier );
		m_ctlCPUMeasureList.SetItemText( nItem, 4, (*iIter).strDivMultiplier );
		m_ctlCPUMeasureList.SetItemText( nItem, 5, (*iIter).strTime          );
	}
	
	theApp.m_pMainFrame->SetStatusBar(_T("Ready"));
}

void CSolidSBCCPUMeasureResultView::OnNMRClickCpumeasureResultList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint orgPoint;
	GetCursorPos(&orgPoint);

	CMenu contextMenu;
	contextMenu.LoadMenu(IDR_EXPORT_RESULTS_MENU);
	CResultContextMenu* pContextPopUp = (CResultContextMenu*)contextMenu.GetSubMenu(0);

	if ( pContextPopUp ) {

		pContextPopUp->AddIdentities( m_strUUID, SSBC_CPUMEASURE, m_nIdentity );

		int nSel = (int)pContextPopUp->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, orgPoint.x,orgPoint.y, this );

		std::vector<SSBC_CLIENT_IDENTITY> vIdentities = g_cDatabaseConnection.GetClientIdentities(m_strUUID);
		if ( ( nSel >= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX ) && ( nSel < (SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX + (int)vIdentities.size() ) ) )
		{
			//filter single run
			nSel -= SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX;
			theApp.OnCPUMeasureResultView(m_strUUID,nSel);

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

void CSolidSBCCPUMeasureResultView::OnFilterDate()
{
	CSolidSBCTimeChooseDlg timeDlg;
	
	if ( timeDlg.DoModal() == IDOK){
		int i = 0;
		i++;
	}
}

void CSolidSBCCPUMeasureResultView::OnExportSelectedResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("cpumeasure_part_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Seperated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){		
		ExportResults(cFile.GetPathName(),true);
	}
}

void CSolidSBCCPUMeasureResultView::OnExportAllResults()
{
	CString strFilename = _T("");
	strFilename.Format(_T("cpumeasure_all_%s.csv"),m_strUUID);
	CFileDialog cFile(FALSE,_T("*.csv"),strFilename,OFN_EXPLORER|OFN_ENABLESIZING,_T("Comma Separated (*.cvs)|*.csv|All Files (*.*)|*.*||"));
	if ( cFile.DoModal() == IDOK ){
		ExportResults(cFile.GetPathName());
	}
}

void CSolidSBCCPUMeasureResultView::ExportResults(CString strFileName,bool bSelectedOnly)
{
	POSITION pos = 0;
	int nItem = -1;
	if ( bSelectedOnly )
		pos = m_ctlCPUMeasureList.GetFirstSelectedItemPosition();
	else {
		nItem = m_ctlCPUMeasureList.GetNextItem(-1,LVNI_ALL);
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
			strMsg.Format(_T("CPUMeasure CSV File: %s"),pszErr);
			AfxMessageBox(strMsg);
		}
#endif
		return;
	}
	END_CATCH
	
	while ( (pos && bSelectedOnly) || ( (nItem >= 0) && !bSelectedOnly ) )  
	{

		if ( bSelectedOnly ){
			nItem = m_ctlCPUMeasureList.GetNextSelectedItem(pos);
		}

		CString strAddDuration   = m_ctlCPUMeasureList.GetItemText(nItem,0);
		CString strDivDuration   = m_ctlCPUMeasureList.GetItemText(nItem,1);
		CString strAllDuration   = m_ctlCPUMeasureList.GetItemText(nItem,2);
		CString strAddMultiplier = m_ctlCPUMeasureList.GetItemText(nItem,3);
		CString strDivMultiplier = m_ctlCPUMeasureList.GetItemText(nItem,4);

		CString strCSVLine = _T("");
		strCSVLine.Format(
			_T("%s%s%s%s%s%s%s%s%s%s"),
			strAddDuration, SSCB_CSV_COMMA_STRING,
			strDivDuration,	SSCB_CSV_COMMA_STRING,
			strAllDuration, SSCB_CSV_COMMA_STRING,
			strAddMultiplier, SSCB_CSV_COMMA_STRING,
			strDivMultiplier, SSCB_CSV_LINEEND_STRING
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
				strMsg.Format(_T("CPUMeasure CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
		}
		END_CATCH

		delete [] pszLine;
		pszLine = NULL;

		if ( !bSelectedOnly ) {
			nItem = m_ctlCPUMeasureList.GetNextItem(nItem,LVNI_ALL);
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
				strMsg.Format(_T("CPUMeasure CSV File: %s"),pszErr);
				AfxMessageBox(strMsg);
			}
#endif
	}
	END_CATCH
}

void CSolidSBCCPUMeasureResultView::OnExportresultsPlot(void)
{
	USES_CONVERSION;
	std::vector<SSBC_CLIENT_CPUMEASURE_RESULT> cpuResults = g_cDatabaseConnection.GetClientCPUMeasureResults(m_strUUID,m_nIdentity);
	std::vector<SSBC_CLIENT_CPUMEASURE_RESULT>::iterator iIter;

	std::map<int,std::map<int,int>> mapMapCoordinates;
	std::map<int,COLORREF>			mapColors;
	std::map<int,std::pair<CString,CString>>  mapPairUnits;

	mapColors[0] = RGB(255,0,0);
	mapColors[1] = RGB(0,0,255);
	mapPairUnits[0] = std::pair<CString,CString>(_T("result"),_T("mult/s (addition)"));
	mapPairUnits[1] = std::pair<CString,CString>(_T("result"),_T("mult/s (division)"));

	unsigned int nPos  = 0;
	std::map<int,int> addCoords, divCoords;
	for ( iIter = cpuResults.begin(); iIter < cpuResults.end(); iIter++ )
	{
		int nAddSpeed = (int)((_ttof((*iIter).strAddMultiplier) / _ttof((*iIter).strAddDuration)));
		addCoords[nPos] = nAddSpeed;
		int nDivSpeed = (int)((_ttof((*iIter).strDivMultiplier) / _ttof((*iIter).strDivDuration)));
		divCoords[nPos] = nDivSpeed;
		nPos++;
	}
	mapMapCoordinates[0] = addCoords;
	mapMapCoordinates[1] = divCoords;

	theApp.OnGraphPlotterView(m_strUUID,m_nIdentity,mapMapCoordinates,mapColors,mapPairUnits);
}