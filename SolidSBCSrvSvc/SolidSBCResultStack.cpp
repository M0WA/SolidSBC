#include "StdAfx.h"
#include "SolidSBCResultStack.h"

/*
CResultStack::CResultStack(void)
: m_lNextHDResultID(0)
, m_lNextCPUMeasureResultID(0)
{	
}

CResultStack::~CResultStack(void)
{
	Flush();
}

int CResultStack::Flush(void)
{	
	FlushHD();
	FlushCPUMeasure();

	return 0;
}

int CResultStack::FlushHD(void)
{
	CString strHDCVSFile;
	if ( ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_HD_RESULT_CSVFILE, &strHDCVSFile) != 0 ) )
		strHDCVSFile = VMPERFORMER_CFGFILE_SRV_HD_RESULT_CSVFILE_DEFAULT;

	if ( strHDCVSFile == _T("") ){
		m_vHDTestResult.clear();
		return 1;}

	CString strCSVCellSeparator = _T("");
	if ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_CSV_CELL_SEPARATOR, &strCSVCellSeparator ) != 0 ){
		strCSVCellSeparator = VMPERFORMER_CFGFILE_SRV_CSV_CELL_SEPARATOR_DEFAULT;}
	strCSVCellSeparator.Replace(_T("\\t"),_T("\t"));

	CString strCSVRowSeparator = _T("");
	if ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_CSV_ROW_SEPARATOR, &strCSVRowSeparator ) != 0 ){
		strCSVRowSeparator = VMPERFORMER_CFGFILE_SRV_CSV_ROW_SEPARATOR_DEFAULT;}
	strCSVRowSeparator.Replace(_T("\\n"),_T("\n"));
	strCSVRowSeparator.Replace(_T("\\r"),_T("\r"));

	//assemble csv string
	CString strCompleteLog;
	for ( std::vector<VMPERFSRV_HD_TESTRESULT>::iterator i = m_vHDTestResult.begin(); i < m_vHDTestResult.end(); i++){
		CString strLogLine;
		strLogLine.Format(_T("%d%s%d%s%f%s%d%s%d%s")
			, (*i).nResultID, strCSVCellSeparator
			, (*i).nClientID, strCSVCellSeparator
			, (*i).hdResult.dSeconds, strCSVCellSeparator
			, (*i).hdResult.ulBytes,  strCSVCellSeparator
			, (*i).hdResult.nType,    strCSVRowSeparator
			);

		strCompleteLog += strLogLine;
	}
	m_vHDTestResult.clear();

	//write to log
	FlushFile(strCompleteLog,strHDCVSFile);

	return 0;
}

int CResultStack::FlushFile(CString strCompleteLog, CString strFileName)
{
	//fix decimal seperators
	CString strCSVDecimalSeparator = _T("");
	if ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_CSV_DECIMAL_SEPARATOR, &strCSVDecimalSeparator ) != 0 ){
		strCSVDecimalSeparator = VMPERFORMER_CFGFILE_SRV_CSV_DECIMAL_SEPARATOR_DEFAULT;}
	strCompleteLog.Replace( _T("."),strCSVDecimalSeparator);

	int length = strCompleteLog.GetLength() * sizeof(TCHAR);
	TRY
	{
		CFile csvFile;		
		csvFile.Open(strFileName,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate);
		csvFile.SeekToEnd();
		csvFile.Write( strCompleteLog.GetBuffer(length), length );
		strCompleteLog.ReleaseBuffer();
		csvFile.Close();
	}
	CATCH(CException, e)
	{
		e->ReportError();
	}
	END_CATCH

	return 0;
}

int CResultStack::AddHDResult(PVMPERFSRV_HD_TESTRESULT pResult)
{
	pResult->nResultID = m_lNextHDResultID;
	m_vHDTestResult.push_back( *pResult );
	m_lNextHDResultID++;
	
	int nMaxHDEntries = 0;
	if ( theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_MAX_HD_RES, &nMaxHDEntries) != 0 ){
		nMaxHDEntries = VMPERFORMER_CFGFILE_SRV_MAX_HD_RES_DEFAULT;}

	if (m_vHDTestResult.size() == nMaxHDEntries){
		FlushHD();}
	

#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
	int nDebug = 0;
	if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
		nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

	if (nDebug)
	{
		CString strLogLine;
		strLogLine.Format(_T("DEBUG: hd-result received:\n     ResultID:%d\n     ClientID:%d\n     Seconds:%f\n     Bytes:%d\n     Read(0)/Write(1): %d")
				, pResult->nResultID
				, pResult->nClientID
				, pResult->hdResult.dSeconds
				, pResult->hdResult.ulBytes
				, pResult->hdResult.nType
			);
		theApp.WriteToVMPerfSrvLog( strLogLine );
	}
#endif
	return 0;
}

int CResultStack::AddCPUMeasureResult(PVMPERF_CPUMEASURE_TESTRESULT pResult)
{
	pResult->nResultID = m_lNextCPUMeasureResultID;
	m_vCPUMeasureResult.push_back( *pResult );
	m_lNextCPUMeasureResultID++;

	int nMaxCPUMeasureEntries = 0;
	if ( theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_MAX_CPUMEASURE_RES, &nMaxCPUMeasureEntries) != 0 ){
		nMaxCPUMeasureEntries = VMPERFORMER_CFGFILE_SRV_MAX_CPUMEASURE_RES_DEFAULT;}

	if (m_vCPUMeasureResult.size() == nMaxCPUMeasureEntries){
		FlushCPUMeasure();}

#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
	int nDebug = 0;
	if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
		nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

	if (nDebug)
	{
		CString strLogLine;
		strLogLine.Format(_T("DEBUG: cpumeasure-result received:\n     ResultID:%d\n     ClientID:%d\n     AddDuration:%f\n     DivDuration:%f\n     OverallDuration: %f\n     AddMultiplier: %u\n     DivMultiplier: %u")
			, pResult->nResultID
			, pResult->nClientID
			, pResult->cpuMeasure.dAddDuration
			, pResult->cpuMeasure.dDivDuration
			, pResult->cpuMeasure.dOverallDuration
			, pResult->cpuMeasure.ullAddMultiplier
			, pResult->cpuMeasure.ullDivMultiplier
		);
		theApp.WriteToVMPerfSrvLog( strLogLine );
	}
#endif
	return 0;
}

int CResultStack::FlushCPUMeasure(void)
{
	CString strCPUMeasureCVSFile;
	if ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_CPUMEASURE_RESULT_CSVFILE, &strCPUMeasureCVSFile) != 0 )
		strCPUMeasureCVSFile = VMPERFORMER_CFGFILE_SRV_CPUMEASURE_RESULT_CSVFILE_DEFAULT;

	if ( strCPUMeasureCVSFile == _T("") ){
		m_vCPUMeasureResult.clear();
		return 1;}

	CString strCSVCellSeparator = _T("");
	if ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_CSV_CELL_SEPARATOR, &strCSVCellSeparator ) != 0 ){
		strCSVCellSeparator = VMPERFORMER_CFGFILE_SRV_CSV_CELL_SEPARATOR_DEFAULT;}
	strCSVCellSeparator.Replace(_T("\\t"),_T("\t"));

	CString strCSVRowSeparator = _T("");
	if ( theApp.m_cConfigFile.GetParamStr( VMPERFORMER_CFGFILE_SRV_CSV_ROW_SEPARATOR, &strCSVRowSeparator ) != 0 ){
		strCSVRowSeparator = VMPERFORMER_CFGFILE_SRV_CSV_ROW_SEPARATOR_DEFAULT;}
	strCSVRowSeparator.Replace(_T("\\n"),_T("\n"));
	strCSVRowSeparator.Replace(_T("\\r"),_T("\r"));

	//assemble csv string
	CString strCompleteLog;
	for ( std::vector<VMPERF_CPUMEASURE_TESTRESULT>::iterator i = m_vCPUMeasureResult.begin(); i < m_vCPUMeasureResult.end(); i++){
		CString strLogLine;
		strLogLine.Format( 
			_T("%d%s%d%s%f%s%f%s%f%s%I64u%s%I64u%s")
			, (*i).nResultID			  ,		strCSVCellSeparator
			, (*i).nClientID			  ,		strCSVCellSeparator
			, (*i).cpuMeasure.dAddDuration,		strCSVCellSeparator
			, (*i).cpuMeasure.dDivDuration,		strCSVCellSeparator
			, (*i).cpuMeasure.dOverallDuration, strCSVCellSeparator
			, (*i).cpuMeasure.ullAddMultiplier,	strCSVCellSeparator
			, (*i).cpuMeasure.ullDivMultiplier,	strCSVRowSeparator
		);
		strCompleteLog += strLogLine;
	}
	m_vCPUMeasureResult.clear();

	//write to log
	FlushFile(strCompleteLog,strCPUMeasureCVSFile);

	return 0;
}

int  CResultStack::AddMemResult(PVMPERF_MEM_TESTRESULT pResult)
{
	pResult->nResultID = m_lNextHDResultID;
	m_lNextHDResultID++;
	//TODO: add mem result to csv file
	return 0;
}

int CResultStack::AddNetPingResult(PVMPERF_NET_PING_TESTRESULT pResult)
{
	pResult->nResultID = m_lNextHDResultID;
	m_lNextHDResultID++;

	return 0;
}

int CResultStack::AddNetTCPConResult(PVMPERF_NET_TCPCON_TESTRESULT pResult)
{
	pResult->nResultID = m_lNextHDResultID;
	m_lNextHDResultID++;

	return 0;
}

int CResultStack::AddClientHistoryResult(CClientResultInfo* pResult, VMPERF_CLIENT_HISTORY_ACTION nAction, UINT nActionParam)
{
	//TODO: add client history to csv file
	return 0;
}
*/