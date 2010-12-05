// ProfileListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCProfileListCtrl.h"


// CProfileListCtrl

IMPLEMENT_DYNAMIC(CProfileListCtrl, CListCtrl)

CProfileListCtrl::CProfileListCtrl()
: m_bIsInitialized(FALSE)
, m_bIsChanged(FALSE)
, m_pApplyButton(NULL)
{	
}

CProfileListCtrl::~CProfileListCtrl()
{
}


BEGIN_MESSAGE_MAP(CProfileListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CProfileListCtrl::OnEndLabelEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// CProfileListCtrl message handlers

int CProfileListCtrl::InitStructure(CButton* pButton)
{
	m_pApplyButton = pButton;
	InsertColumn( 0, CString(MAKEINTRESOURCE(IDS_PROFILE_HEADER_ATTRIBUTE_STRING)) , 0, 350);
	InsertColumn( 1, CString(MAKEINTRESOURCE(IDS_PROFILE_HEADER_VALUE_STRING))     , 0, 165);

	int nItmCnt = 0;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_ID_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NAME_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_ENABLE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_RANDOM_READ_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_RANDOM_WRITE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_READ_MAX_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_WRITE_MAX_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_READWRITE_DELAY_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_HD_TRANSMIT_DATA_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPU_ENABLE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPU_RANDOMIZE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPU_MAX_RANDOM_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPU_MAX_SLEEP_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPU_THREAD_COUNT_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPUMEASURE_ENABLE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPUMEASURE_ADDITION_COUNT_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPUMEASURE_DIVISION_COUNT_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPUMEASURE_TRANSMIT_DATA_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPUMEASURE_FIX_MULTI_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_CPUMEASURE_FIX_MULTI_THRESHOLD_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_MEMORY_ENABLE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_MEMORY_RANDOMIZE_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_MEMORY_MIN_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_MEMORY_MAX_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_MEMORY_TRANSMIT_DATA_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_ENABLEPINGTEST_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_ENABLETCPCONTEST_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_PINGINTERVAL_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_PINGHOST_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_PINGTTL_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_PINGPAYLOAD_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_PINGTRANSMITDATA_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_TCPCONINTERVAL_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_TCPCONHOST_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_TCPCONPORT_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_TCPCONTTL_STRING)) );nItmCnt++;
	InsertItem( nItmCnt, CString(MAKEINTRESOURCE(IDS_PROFILE_NETWORK_TCPCONTRANSMITDATA_STRING)) );nItmCnt++;

	SetExtendedStyle(GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	InitProfiles();

	m_bIsInitialized = TRUE;
	return 0;
}

int CProfileListCtrl::SetActiveProfile(CString strProfileName)
{	
	if ( !m_bIsInitialized )
		InitStructure(m_pApplyButton);

	std::vector<CSolidSBCClientConfigProfile> vClientConfigProfiles = m_vClientConfProfiles;

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	CSolidSBCClientConfigProfile cClientConfigProfile;
	BOOL bFound = FALSE;
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = vClientConfigProfiles.begin(); i < vClientConfigProfiles.end(); i++){
		if ( ((*i).m_strName) == strProfileName ){
			bFound = TRUE;
			cClientConfigProfile = (*i);}
	}

	if ( !bFound ) {return -1;} //TODO: an error occurred
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	TCHAR szValue[255] = {0};
	int nItmCnt = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//profile settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_nProfileID,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//id	
	SetItemText( nItmCnt, 1, cClientConfigProfile.m_strName );nItmCnt++;	//name


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//harddrive settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bHDCfgEnable,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_enable	
	
	_itot_s(cClientConfigProfile.m_bHDCfgRandomRead,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_random_read	

	_itot_s(cClientConfigProfile.m_bHDCfgRandomWrite,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_random_write

	_itot_s(cClientConfigProfile.m_nHDCfgReadMax,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_read_max

	_itot_s(cClientConfigProfile.m_nHDCfgWriteMax,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_write_max

	_itot_s(cClientConfigProfile.m_nHDCfgReadWriteDelay,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_readwrite_delay
	
	_itot_s(cClientConfigProfile.m_bHDCfgTransmitData,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//harddrive_transmit_data	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//cpu settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bCpuCfgEnable,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpu_enable

	_itot_s(cClientConfigProfile.m_bCpuCfgRandom,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpu_random	
	
	_itot_s(cClientConfigProfile.m_nCpuCfgMaxRand,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpu_randmax	

	CString strSleepMS; strSleepMS.Format(_T("%f"),cClientConfigProfile.m_fCpuCfgSleepMS);
	SetItemText( nItmCnt, 1, strSleepMS );nItmCnt++;						//cpu_sleep_ms	

	_itot_s(cClientConfigProfile.m_nCpuCfgThreadCnt,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpu_thread_cnt


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//cpumeasure settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bCpuMeasureCfgEnable,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpumeasure_enable
	
	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgAdditionCount,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpumeasure_addition_count

	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgDivisionCount,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpumeasure_division_count

	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgTransmitData,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpumeasure_transmit_data

	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgAutoFixMultipliers,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//cpumeasure_autofix_multipliers
	
	CString strMultiplierThresholdSec;										//cpumeasure_fixmultipliers_threshold
	strMultiplierThresholdSec.Format(_T("%f"),cClientConfigProfile.m_dCpuMeasureCfgFixMultipliersThreshold);
	SetItemText( nItmCnt, 1, strMultiplierThresholdSec );nItmCnt++;		


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//memory settings	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bMemCfgEnable,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//memory_enable	

	_itot_s(cClientConfigProfile.m_bMemCfgRandomize,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;							//memory_randomize	
	
	_itot_s(cClientConfigProfile.m_nMemCfgMinMem,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//memory_min_mem

	_itot_s(cClientConfigProfile.m_nMemCfgMaxMem,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//memory_max_mem

	_itot_s(cClientConfigProfile.m_bMemCfgTransmitData,szValue,10);
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;							//memory_transmit_data

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//network settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	_itot_s(cClientConfigProfile.m_bNetCfgEnablePingTest,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_bNetCfgEnableTcpConnTest,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_nNetCfgPingInterval,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	SetItemText(nItmCnt, 1, cClientConfigProfile.m_szNetCfgPingHost );nItmCnt++;

	_itot_s(cClientConfigProfile.m_nNetCfgPingTTL,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_nNetCfgPingPayloadSize,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_bNetCfgPingTransmitData,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_nNetCfgTcpConnInterval,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	SetItemText(nItmCnt, 1, cClientConfigProfile.m_szNetCfgTcpConnHost );nItmCnt++;

	_itot_s(cClientConfigProfile.m_nNetCfgTcpConnPort,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_nNetCfgTcpConnTTL,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	_itot_s(cClientConfigProfile.m_bNetCfgTcpConnTransmitData,szValue,10);
	SetItemText(nItmCnt, 1, szValue );nItmCnt++;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	SetClean();
	return 0;
}

int CProfileListCtrl::SetActiveProfile(UINT nProfileID)
{
	std::vector<CSolidSBCClientConfigProfile> vClientConfigProfiles = m_vClientConfProfiles;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	CSolidSBCClientConfigProfile cClientConfigProfile;
	BOOL bFound = FALSE;
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = vClientConfigProfiles.begin(); i < vClientConfigProfiles.end(); i++){
		if ( ((*i).m_nProfileID) == nProfileID ){
			bFound = TRUE;
			cClientConfigProfile = (*i);}
	}

	if ( !bFound ) {return -1;} //TODO: an error occurred
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	return SetActiveProfile(cClientConfigProfile.m_strName);
}

int CProfileListCtrl::SaveProfile(void)
{
	std::vector<CSolidSBCClientConfigProfile> vClientConfigProfiles = m_vClientConfProfiles;

	CString strProfileID = GetItemText( 0, 1 );
	int nProfileID = _ttoi( strProfileID );

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	//find fitting profile and fill out cClientConfigProfile accordingly
	CSolidSBCClientConfigProfile cClientConfigProfile;
	BOOL bFound = FALSE;
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = vClientConfigProfiles.begin(); i < vClientConfigProfiles.end(); i++){
		if ( ((*i).m_nProfileID) == nProfileID ){
			bFound = TRUE;
			cClientConfigProfile = (*i);}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	int nItmCnt = 0, nChangesFound = 0;
	TCHAR szValue[255] = {0};
	CString strItem = _T("");
	CXMLFile xmlProfileConfigFile;
	xmlProfileConfigFile.Init(SSBC_SRV_PROFILEFILE);
	CString strXPath = _T(""), strXPathFormat = _T("/portfolio/profile[@id=\"%d\"]/%s");

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//profile settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//id	

	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//name
	if ( cClientConfigProfile.m_strName != strItem ){
		strXPath.Format(strXPathFormat,nProfileID,_T("name[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//harddrive settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bHDCfgEnable,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_enable	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ) {
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/enable[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}
	
	_itot_s(cClientConfigProfile.m_bHDCfgRandomRead,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_random_read	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/randomizeread[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bHDCfgRandomWrite,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_random_write
	if (  ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/randomizewrite[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nHDCfgReadMax,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_read_max
	if (  ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/readmax[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nHDCfgWriteMax,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_write_max
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/writemax[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nHDCfgReadWriteDelay,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_readwrite_delay
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/readwritedelay[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}
	
	_itot_s(cClientConfigProfile.m_bHDCfgTransmitData,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//harddrive_transmit_data	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/transmitdata[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//cpu settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bCpuCfgEnable,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpu_enable
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/enable[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bCpuCfgRandom,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpu_random	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/randomize[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}
	
	_itot_s(cClientConfigProfile.m_nCpuCfgMaxRand,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpu_randmax	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/maxrand[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	CString strSleepMS; strSleepMS.Format(_T("%f"),cClientConfigProfile.m_fCpuCfgSleepMS);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpu_sleep_ms	
	if ( ( strSleepMS != strItem ) && CheckFLOAT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/sleepms[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nCpuCfgThreadCnt,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpu_thread_cnt
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/threadcount[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//cpumeasure settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bCpuMeasureCfgEnable,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpumeasure_enable
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/enable[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}
	
	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgAdditionCount,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpumeasure_addition_count
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/additioncnt[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgDivisionCount,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpumeasure_division_count
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/divisioncnt[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgTransmitData,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpumeasure_transmit_data
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/transmitdata[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nCpuMeasureCfgAutoFixMultipliers,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//cpumeasure_autofix_multipliers
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/autofixmultipliers[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}
	
	CString strMultiplierThresholdSec;										//cpumeasure_fixmultipliers_threshold
	strMultiplierThresholdSec.Format(_T("%f"),cClientConfigProfile.m_dCpuMeasureCfgFixMultipliersThreshold);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;		
	if ( ( strMultiplierThresholdSec != strItem ) && CheckFLOAT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/fixmultipliersthreshold[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//memory settings	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bMemCfgEnable,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//memory_enable	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/enable[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bMemCfgRandomize,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//memory_randomize	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/randomize[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}
	
	_itot_s(cClientConfigProfile.m_nMemCfgMinMem,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//memory_min_mem
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/minmem[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nMemCfgMaxMem,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//memory_max_mem
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/maxmem[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bMemCfgTransmitData,szValue,10);
	strItem = GetItemText( nItmCnt, 1 );nItmCnt++;							//memory_transmit_data
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/transmitdata[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//network settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	_itot_s(cClientConfigProfile.m_bNetCfgEnablePingTest,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_enable_ping	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/enableping[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bNetCfgEnableTcpConnTest,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_enable_tcpconn	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/enabletcpconn[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nNetCfgPingInterval,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_ping_interval	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pinginterval[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_ping_host	
	if ( ( cClientConfigProfile.m_szNetCfgPingHost != strItem ) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pinghost[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nNetCfgPingTTL,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_ping_ttl	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingttl[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nNetCfgPingPayloadSize,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_ping_payload	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingpayload[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bNetCfgPingTransmitData,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_ping_transmitdata	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingtransmitdata[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nNetCfgTcpConnInterval,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_tcpcon_interval	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconinterval[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_tcpcon_host	
	if ( ( cClientConfigProfile.m_szNetCfgTcpConnHost != strItem ) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconhost[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nNetCfgTcpConnPort,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_tcpcon_port	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconport[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_nNetCfgTcpConnTTL,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_tcpcon_ttl	
	if ( ( szValue != strItem ) && CheckINT(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconttl[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	_itot_s(cClientConfigProfile.m_bNetCfgTcpConnTransmitData,szValue,10);
	strItem = GetItemText(nItmCnt, 1 );nItmCnt++;									//network_tcpcon_transmitdata	
	if ( ( szValue != strItem ) && CheckBOOL(strItem) ){
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpcontransmitdata[1]"));
		xmlProfileConfigFile.SetNodeString(strXPath, strItem);
		nChangesFound++;}

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	if ( nChangesFound ) {
		xmlProfileConfigFile.Save(SSBC_SRV_PROFILEFILE);

		//TODO: does not save xml here
		//xxx

		SetClean();
		InitProfiles();
		SetActiveProfile(nProfileID);}
	
	return 0;
}

void CProfileListCtrl::SetClean(void)
{	
	m_bIsChanged = FALSE;

	if (m_pApplyButton)
		m_pApplyButton->EnableWindow(FALSE);
}

int CProfileListCtrl::HitTestEx(CPoint &point, int *col) const
{
	int colnum = 0;
	int row = HitTest( point, NULL );
	
	if( col ) *col = 0;

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return row;

	// Get the top and bottom row visible
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();
	
	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left 
					&& point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

CEdit* CProfileListCtrl::EditSubLabel( int nItem, int nCol )
{
	// The returned pointer should not be saved

	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 5 )
		return NULL;

	// Get the column offset
	int offset = 0;
	for( int i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;
	if( rect.right > rcClient.right) rect.right = rcClient.right;

	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL;
	
	m_strOldItemText = GetItemText(nItem, nCol);
	CEdit *pEdit = new CProfileListEditCtrl(nItem, nCol, GetItemText( nItem, nCol ));
	pEdit->Create( dwStyle, rect, this, IDC_PROFILE_LIST_EDIT );
	
	return pEdit;
}

void CProfileListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM	*plvItem = &plvDispInfo->item;

	if (plvItem->pszText != NULL)
	{
		SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
		if ( m_strOldItemText != plvItem->pszText ){
			if (m_pApplyButton)
				m_pApplyButton->EnableWindow(TRUE);
			m_bIsChanged = TRUE;}
	}
	*pResult = FALSE;
}

void CProfileListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int index;
	CListCtrl::OnLButtonDown(nFlags, point);

	int colnum;
	if( ( index = HitTestEx( point, &colnum )) != -1 )
	{
		UINT flag = LVIS_FOCUSED;
		if( (GetItemState( index, flag ) & flag) == flag && colnum > 0)
		{
			// no check for LVS_EDITLABELS...
			//if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
			if (   ( colnum == 1 )  //only values(column 1) are editable
				&& ( index  != 0 )  //ids(row 0) are not editable
			   )
				EditSubLabel( index, colnum );
		}
		else
			SetItemState( index, LVIS_SELECTED | LVIS_FOCUSED ,	LVIS_SELECTED | LVIS_FOCUSED); 
	}
}

void CProfileListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CProfileListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this ) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CProfileListCtrl::CheckBOOL(CString strToBoolean, BOOL bMsgBoxOnError)
{
	BOOL bReturn = TRUE;
	PTCHAR ptstrStop;
	ULONG long_var = _tcstoul((PCTSTR)strToBoolean, &ptstrStop, 0);

	if (ERANGE == errno)
	{
		bReturn = FALSE;
	}
	else if ( *ptstrStop != 0 )
	{
		bReturn = FALSE;
	}

	if ( (!bReturn) && bMsgBoxOnError  ){
		CString strMsg;
		strMsg.Format(CString(MAKEINTRESOURCE(IDS_PROFILE_MB_CANNOT_CONVERT_BOOL_STRING)),strToBoolean);
		AfxMessageBox(strMsg);
	}

	return bReturn;
}

BOOL CProfileListCtrl::CheckINT(CString strToInteger, BOOL bMsgBoxOnError)
{
	BOOL bReturn = TRUE;
	PTCHAR ptstrStop;
	long long_var = _tcstol((PCTSTR)strToInteger, &ptstrStop, 0);

	if (ERANGE == errno)
	{
		bReturn = FALSE;
	}
	else if ( *ptstrStop != 0 )
	{
		bReturn = FALSE;
	}

	if ( (!bReturn) && bMsgBoxOnError  ){
		CString strMsg;
		strMsg.Format(CString(MAKEINTRESOURCE(IDS_PROFILE_MB_CANNOT_CONVERT_INT_STRING)),strToInteger);
		AfxMessageBox(strMsg);
	}

	return bReturn;
}

BOOL CProfileListCtrl::CheckFLOAT(CString strToFloat, BOOL bMsgBoxOnError)
{
	BOOL bReturn = TRUE;
	PTCHAR ptstrStop;
	double d = _tcstod((PCTSTR)strToFloat, &ptstrStop);

	if (ERANGE == errno)
	{
		bReturn = FALSE;
	}
	else if ( *ptstrStop != 0 )
	{
		bReturn = FALSE;
	}

	if ( (!bReturn) && bMsgBoxOnError  ){
		CString strMsg;
		strMsg.Format( CString(MAKEINTRESOURCE(IDS_PROFILE_MB_CANNOT_CONVERT_FLOAT_STRING) ),strToFloat);
		AfxMessageBox(strMsg);
	}
	
	return bReturn;
}

int CProfileListCtrl::InitNewProfile(void)
{
	TCHAR szValue[255] = {0};
	int nItmCnt = 0;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//profile settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	UINT nNextProfileID = 0;

	//find next profile id
	std::vector<CSolidSBCClientConfigProfile> vProfile = m_vClientConfProfiles;
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = vProfile.begin(); i < vProfile.end(); i++)
		if ( (*i).m_nProfileID > nNextProfileID )
			nNextProfileID = (*i).m_nProfileID;
	nNextProfileID++;

	_itot_s( nNextProfileID, szValue, 10 );
	SetItemText( nItmCnt, 1, szValue );nItmCnt++;												//id	
	SetItemText( nItmCnt, 1, CString(MAKEINTRESOURCE(IDS_PROFILE_NEW_NAME_STRING)) );nItmCnt++;	//name


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//harddrive settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_enable	
	
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_random_read	

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_random_write

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_read_max

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_write_max

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_readwrite_delay
	
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//harddrive_transmit_data	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//cpu settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpu_enable

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpu_random	
	
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpu_randmax	

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpu_sleep_ms	

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpu_thread_cnt


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//cpumeasure settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpumeasure_enable
	
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpumeasure_addition_count

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpumeasure_division_count

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpumeasure_transmit_data

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpumeasure_autofix_multipliers
	
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//cpumeasure_fixmultipliers_threshold


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//memory settings	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//memory_enable	

	SetItemText(nItmCnt, 1, _T("0") );nItmCnt++;							//memory_randomize	
	
	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//memory_min_mem

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//memory_max_mem

	SetItemText( nItmCnt, 1, _T("0") );nItmCnt++;							//memory_transmit_data


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//network settings
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	SetItemText(nItmCnt, 1, _T("0") );nItmCnt++;			//network_ping_enable

	SetItemText(nItmCnt, 1, _T("0") );nItmCnt++;			//network_tcpcon_enable

	SetItemText(nItmCnt, 1, _T("60000") );nItmCnt++;		//network_ping_interval

	SetItemText(nItmCnt, 1, _T("127.0.0.1") );nItmCnt++;	//network_ping_host

	SetItemText(nItmCnt, 1, _T("1000") );nItmCnt++;			//network_ping_ttl

	SetItemText(nItmCnt, 1, _T("32") );nItmCnt++;			//network_ping_payload

	SetItemText(nItmCnt, 1, _T("1") );nItmCnt++;			//network_ping_transmit_data

	SetItemText(nItmCnt, 1, _T("60000") );nItmCnt++;		//network_tcpcon_interval

	SetItemText(nItmCnt, 1, _T("127.0.0.1") );nItmCnt++;	//network_tcpcon_host

	SetItemText(nItmCnt, 1, _T("80") );nItmCnt++;			//network_tcpcon_port

	SetItemText(nItmCnt, 1, _T("1000") );nItmCnt++;			//network_tcpcon_ttl

	SetItemText(nItmCnt, 1, _T("1" ) );nItmCnt++;			//network_tcpcon_transmit_data

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	SetActiveProfile(nNextProfileID);
	m_bIsChanged = TRUE;
	return 0;
}

CString CProfileListCtrl::GetFilePath(void)
{
	TCHAR szFileNamePath[1024];
	GetModuleFileName(NULL,szFileNamePath,1024);
	CString strFileNamePath = szFileNamePath;
	int nPos = strFileNamePath.ReverseFind(_T('\\'));
	CString strPath = strFileNamePath.Left(nPos);
	return strPath;
}

CString CProfileListCtrl::GetProfileFileNamePath(void)
{
	//assemble name for svc settings file
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),GetFilePath(),SSBC_SRV_PROFILEFILE);
	return strFileName;
}

int CProfileListCtrl::InitProfiles(void)
{
	m_xmlProfileConfigFile.Init(GetProfileFileNamePath());

	CString strXPath = _T(""), strXPathFormat = _T("/portfolio/profile[@id=\"%d\"]/%s");
	CString strValue = _T(""); int nValue = 0; double dValue = 0.0f;
	UINT nProfileID = 0; int nErr = 0;

	m_vClientConfProfiles.clear();
	while ( 1 )
	{
		CSolidSBCClientConfigProfile cClientConfigProfile;

		// =================== profile settings ===================
		cClientConfigProfile.m_nProfileID = nProfileID;

		strXPath.Format(strXPathFormat,nProfileID,_T("name[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeString(strXPath, &strValue);
		if (!nErr){cClientConfigProfile.m_strName = strValue;} 
		else {
			break;}
		
		// =================== harddrive settings =================== 
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgEnable = (BOOL)nValue;} 
		else {
			break;}
			
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/randomizeread[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgRandomRead = (BOOL)nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/randomizewrite[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgRandomWrite = (BOOL)nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/readmax[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nHDCfgReadMax = nValue;} 
		else {
			break;}

		
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/writemax[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nHDCfgWriteMax = nValue;} 
		else {
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/readwritedelay[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nHDCfgReadWriteDelay = nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/transmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgTransmitData = (BOOL)nValue;} 
		else {
			break;}


		// =================== cpu settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bCpuCfgEnable = (BOOL)nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/randomize[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bCpuCfgRandom = (BOOL)nValue;}
		else {
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/maxrand[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuCfgMaxRand = (BOOL)nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/sleepms[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeFloat(strXPath, &dValue);
		if (!nErr){cClientConfigProfile.m_fCpuCfgSleepMS = dValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/threadcount[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuCfgThreadCnt = nValue;} 
		else {
			break;}

		// =================== cpumeasure settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bCpuMeasureCfgEnable = (BOOL)nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/additioncnt[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgAdditionCount = nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/divisioncnt[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgDivisionCount = nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/transmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgTransmitData = (BOOL)nValue;} 
		else {
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/autofixmultipliers[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgAutoFixMultipliers = nValue;} 
		else {
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/fixmultipliersthreshold[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeFloat(strXPath, &dValue);
		if (!nErr){cClientConfigProfile.m_dCpuMeasureCfgFixMultipliersThreshold = dValue;} 
		else {
			break;}

		// =================== memory settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bMemCfgEnable = (BOOL)nValue;} 
		else {
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/randomize[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bMemCfgRandomize = (BOOL)nValue;} 
		else {
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/minmem[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nMemCfgMinMem = nValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/maxmem[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nMemCfgMaxMem = nValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/transmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bMemCfgTransmitData = (BOOL)nValue;} 
		else {
			break;}

		// =================== network settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/enableping[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgEnablePingTest = (BOOL)nValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/enabletcpconn[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgEnableTcpConnTest = (BOOL)nValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pinginterval[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgPingInterval = (BOOL)nValue;}  
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pinghost[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeString(strXPath, &strValue);
		if (!nErr){cClientConfigProfile.m_szNetCfgPingHost = strValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingttl[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgPingTTL = (BOOL)nValue;}  
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingpayload[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgPingPayloadSize = (BOOL)nValue;}  
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingtransmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgPingTransmitData = (BOOL)nValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconinterval[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgTcpConnInterval = nValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconhost[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeString(strXPath, &strValue);
		if (!nErr){cClientConfigProfile.m_szNetCfgTcpConnHost = strValue;} 
		else {
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconport[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgTcpConnPort = nValue;} 
		else {
			break;}	
		
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconttl[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgTcpConnTTL = nValue;} 
		else {
			break;}	
		
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpcontransmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgTcpConnTransmitData = (BOOL)nValue;} 
		else {
			break;}	

		m_vClientConfProfiles.push_back(cClientConfigProfile);
		nProfileID++;
	}

	return nProfileID;
}