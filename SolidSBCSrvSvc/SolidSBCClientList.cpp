#include "StdAfx.h"
#include "SolidSBCClientList.h"

CSolidSBCClientList g_cClientList;

CSolidSBCClientList::CSolidSBCClientList(void)
: m_pResultDBConnector(NULL)
{
}

CSolidSBCClientList::~CSolidSBCClientList(void)
{
	//Empty();
}

int CSolidSBCClientList::AddResultClient(CSolidSBCClientResultInfo clientInfo)
{
	BOOL bFound = FALSE;
	m_cClientConfResultMutex.Lock();

	//check if duplicate, sanity check...
	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) {
		
		//client with same ip and port is already connected ???
		if ( (clientInfo.GetSockAddr().sin_addr.s_addr == (*i).GetSockAddr().sin_addr.s_addr) &&
			 (clientInfo.GetSockAddr().sin_port        == (*i).GetSockAddr().sin_port)  ) {

		    //throw out client,close connection
			if ( !((*i).GetResultClientHandlerSocket()) ) 
				((*i).GetResultClientHandlerSocket())->Close();

			//cleanup old connection socket
			delete ((*i).GetResultClientHandlerSocket());

			//update information
			(*i) = clientInfo;

			bFound = TRUE;
			break;
		}
	}

	//must be an new client, add it...
	if ( !bFound )
		m_vClientResultInfos.push_back(clientInfo);

	m_cClientConfResultMutex.Unlock();

	{
		USES_CONVERSION;
		CString strMsg;
		strMsg.Format( 
			_T("Client ID %d at %s:%d requested result/control connection")
			, clientInfo.GetClientID()
			, A2T(inet_ntoa(clientInfo.GetSockAddr().sin_addr)) 
			, htons(clientInfo.GetSockAddr().sin_port) );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
	}

	return 0;
}

void CSolidSBCClientList::Empty(void)
{
	m_cClientConfResultMutex.Lock();

	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) 
	{
		if ( ((*i).GetResultClientHandlerSocket()) ) 
		{
			{
				USES_CONVERSION;
				CString strMsg;
				strMsg.Format( 
					_T("Deleting and closing result/control connection: client id: %d, name: %s, ip:port %s:%d, profile: %s")
					, (*i).GetClientID(), (*i).GetClientName(), A2T(inet_ntoa((*i).GetSockAddr().sin_addr))
					, htons((*i).GetSockAddr().sin_port), GetProfileNameByID((*i).GetProfileID())	);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
			}

			((*i).GetResultClientHandlerSocket())->Close();
			delete ((*i).GetResultClientHandlerSocket()); 
			(*i).SetResultClientHandlerSocket(NULL);
		}
	}
	m_vClientResultInfos.clear();

	m_cClientConfResultMutex.Unlock();
}

CString CSolidSBCClientList::GetProfileFileNamePath(void)
{
	//assemble name for svc settings file
	CString strFileName = _T("");
	strFileName.Format(_T("%s\\%s"),g_strImagePath,SSBC_SRV_PROFILEFILE);
	return strFileName;
}

int CSolidSBCClientList::Init(void)
{
	if ( !m_cConfigFile.Init() )
		return 0;

	if ( !InitProfiles() )
		return 0;

	if ( !InitDBConnection() )
		return 0;

	return 1;
}

int CSolidSBCClientList::InitDBConnection(void)
{	
	if ( m_pResultDBConnector ){
		m_pResultDBConnector->Disconnect();
		delete m_pResultDBConnector;
		m_pResultDBConnector = NULL; };

	CString strType = _T("")
		, strDbHost = _T("")
		, strDbName = _T("")
		, strDbUser = _T("")
		, strDbPass = _T("");
	int nPort = -1;

	if ( m_cConfigFile.GetParamStr(SSBC_DATABASE_TYPE,  &strType   ) != 0 ){
		return 0;}
	if ( m_cConfigFile.GetParamStr( SSBC_DATABASE_HOST, &strDbHost ) != 0 ){
		return 0;}
	if ( m_cConfigFile.GetParamInt( SSBC_DATABASE_PORT, &nPort     ) != 0 ){
		return 0;}
	if ( m_cConfigFile.GetParamStr( SSBC_DATABASE_NAME, &strDbName ) != 0 ){
		return 0;}
	if ( m_cConfigFile.GetParamStr( SSBC_DATABASE_USER, &strDbUser ) != 0 ){
		return 0;}
	if ( m_cConfigFile.GetParamStr( SSBC_DATABASE_PASS, &strDbPass ) != 0 ){
		return 0;}

	//do db type releated constructor
	if ( strType.MakeLower() == _T("mysql") ){		//mysql
		m_pResultDBConnector = new CSolidSBCResultDBConnectorMySQL();}
	else if ( strType.MakeLower() == _T("mssql") ){	//TODO: mssql (not implemented)
		return 0;}
	//no database type found
	else {		
		return 0;}

	m_pResultDBConnector->SetConfig(strDbHost,nPort,strDbName, strDbUser, strDbPass);
	return m_pResultDBConnector->Connect();
}

int CSolidSBCClientList::InitProfiles(void)
{
	m_xmlProfileConfigFile.Init(GetProfileFileNamePath());

	CString strXPath = _T(""), strXPathFormat = _T("/portfolio/profile[@id=\"%d\"]/%s");
	CString strValue = _T(""); int nValue = 0; double dValue = 0.0f;
	UINT nProfileID = 0; int nErr = 0;

	m_cClientConfProfilesMutex.Lock();
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
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"name\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
		
		// =================== harddrive settings =================== 
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgEnable = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"enable\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
			
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/randomizeread[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgRandomRead = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"randomizeread\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/randomizewrite[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgRandomWrite = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"randomizewrite\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/readmax[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nHDCfgReadMax = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"readmax\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/writemax[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nHDCfgWriteMax = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"writemax\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/readwritedelay[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nHDCfgReadWriteDelay = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"readwritedelay\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("harddrive[1]/transmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bHDCfgTransmitData = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"harddrive\"->\"transmitdata\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}


		// =================== cpu settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bCpuCfgEnable = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpu\"->\"enable\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/randomize[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bCpuCfgRandom = (BOOL)nValue;}
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpu\"->\"randomize\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/maxrand[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuCfgMaxRand = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpu\"->\"maxrand\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/sleepms[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeFloat(strXPath, &dValue);
		if (!nErr){cClientConfigProfile.m_fCpuCfgSleepMS = dValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpu\"->\"sleepms\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpu[1]/threadcount[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuCfgThreadCnt = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpu\"->\"threadcount\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		// =================== cpumeasure settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bCpuMeasureCfgEnable = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpumeasure\"->\"enable\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/additioncnt[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgAdditionCount = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpumeasure\"->\"additioncnt\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/divisioncnt[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgDivisionCount = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpumeasure\"->\"divisioncnt\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/transmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgTransmitData = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpumeasure\"->\"transmitdata\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/autofixmultipliers[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nCpuMeasureCfgAutoFixMultipliers = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpumeasure\"->\"autofixmultipliers\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("cpumeasure[1]/fixmultipliersthreshold[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeFloat(strXPath, &dValue);
		if (!nErr){cClientConfigProfile.m_dCpuMeasureCfgFixMultipliersThreshold = dValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"cpumeasure\"->\"fixmultipliersthreshold\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		// =================== memory settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/enable[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bMemCfgEnable = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"memory\"->\"enable\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/randomize[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bMemCfgRandomize = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"memory\"->\"randomize\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}
		
		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/minmem[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nMemCfgMinMem = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"memory\"->\"minmem\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/maxmem[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nMemCfgMaxMem = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"memory\"->\"maxmem\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("memory[1]/transmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bMemCfgTransmitData = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"memory\"->\"transmitdata\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}

		// =================== network settings ===================
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/enableping[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgEnablePingTest = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"enableping\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/enabletcpconn[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgEnableTcpConnTest = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"enabletcpconn\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pinginterval[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgPingInterval = (BOOL)nValue;}  
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"pinginterval\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pinghost[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeString(strXPath, &strValue);
		if (!nErr){cClientConfigProfile.m_szNetCfgPingHost = strValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"pinghost\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingttl[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgPingTTL = (BOOL)nValue;}  
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"pingttl\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingpayload[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgPingPayloadSize = (BOOL)nValue;}  
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"pingpayload\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/pingtransmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgPingTransmitData = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"pingtransmitdata\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconinterval[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgTcpConnInterval = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"tcpconinterval\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconhost[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeString(strXPath, &strValue);
		if (!nErr){cClientConfigProfile.m_szNetCfgTcpConnHost = strValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"tcpconhost\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	

		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconport[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgTcpConnPort = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"tcpconport\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	
		
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpconttl[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_nNetCfgTcpConnTTL = nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"tcpconttl\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	
		
		strXPath.Format(strXPathFormat,nProfileID,_T("network[1]/tcpcontransmitdata[1]"));
		nErr = m_xmlProfileConfigFile.GetNodeInt(strXPath, &nValue);
		if (!nErr){cClientConfigProfile.m_bNetCfgTcpConnTransmitData = (BOOL)nValue;} 
		else {
			{ //logging
				CString strLog;
				strLog.Format( _T("ERROR:\n     CSolidSBCClientList::InitProfiles(): Could not find \"network\"->\"tcpcontransmitdata\" for profile id %d"),nProfileID);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}
			break;}	


		m_vClientConfProfiles.push_back(cClientConfigProfile);
		nProfileID++;
	}
	m_cClientConfProfilesMutex.Unlock();

	return nProfileID;
}

SSBC_PROFILE_REPLY_PACKET CSolidSBCClientList::GetProfileReplyPacket(UINT nProfileID)
{
	SSBC_PROFILE_REPLY_PACKET packet;
	ZeroMemory(&packet,sizeof(SSBC_PROFILE_REPLY_PACKET));

	m_cClientConfProfilesMutex.Lock();
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = m_vClientConfProfiles.begin(); i < m_vClientConfProfiles.end(); i++)
	{
		if ( (*i).m_nProfileID == nProfileID ){
			//profile settings
			packet.nProfileID = (*i).m_nProfileID;
			_stprintf_s(packet.szProfileName, SSBC_PROFILE_MAX_PROFILE_NAME, (*i).m_strName);

			//harddrive settings
			packet.bHDCfgEnable			= (*i).m_bHDCfgEnable;
			packet.bHDCfgRandomRead		= (*i).m_bHDCfgRandomRead;
			packet.bHDCfgRandomWrite	= (*i).m_bHDCfgRandomWrite;
			packet.nHDCfgMaxRead		= (*i).m_nHDCfgReadMax;
			packet.nHDCfgMaxWrite		= (*i).m_nHDCfgWriteMax;
			packet.nHDCfgReadWriteDelay = (*i).m_nHDCfgReadWriteDelay;
			packet.bHDCfgTransmitData   = (*i).m_bHDCfgTransmitData;
			
			//cpu settings
			packet.bCpuCfgEnable    = (*i).m_bCpuCfgEnable;
			packet.bCpuCfgRandom    = (*i).m_bCpuCfgRandom;
			packet.nCpuCfgMaxRand   = (*i).m_nCpuCfgMaxRand;
			packet.fCpuCfgSleepMS   = (*i).m_fCpuCfgSleepMS;
			packet.nCpuCfgThreadCnt = (*i).m_nCpuCfgThreadCnt;

			//cpumeasure settings
			packet.bCpuMeasureCfgEnable        = (*i).m_bCpuMeasureCfgEnable;
			packet.nCpuMeasureCfgAdditionCount = (*i).m_nCpuMeasureCfgAdditionCount;
			packet.nCpuMeasureCfgDivisionCount = (*i).m_nCpuMeasureCfgDivisionCount;
			packet.nCpuMeasureCfgTransmitData  = (*i).m_nCpuMeasureCfgTransmitData;
			packet.nCpuMeasureCfgAutoFixMultipliers			= (*i).m_nCpuMeasureCfgAutoFixMultipliers;
			packet.dCpuMeasureCfgFixMultipliersThreshold	= (*i).m_dCpuMeasureCfgFixMultipliersThreshold;

			//memory settings
			packet.bMemCfgEnable		= (*i).m_bMemCfgEnable;
			packet.bMemCfgRandomize		= (*i).m_bMemCfgRandomize;
			packet.nMemCfgMinMem		= (*i).m_nMemCfgMinMem;
			packet.nMemCfgMaxMem		= (*i).m_nMemCfgMaxMem;
			packet.bMemCfgTransmitData	= (*i).m_bMemCfgTransmitData;

			//network settings
			packet.bNetCfgEnablePingTest	= (*i).m_bNetCfgEnablePingTest;
			packet.bNetCfgEnableTcpConnTest = (*i).m_bNetCfgEnableTcpConnTest;
			packet.nNetCfgPingInterval		= (*i).m_nNetCfgPingInterval;
			packet.nNetCfgPingTTL			= (*i).m_nNetCfgPingTTL;
			packet.nNetCfgPingPayloadSize	= (*i).m_nNetCfgPingPayloadSize;
			packet.bNetCfgPingTransmitData	= (*i).m_bNetCfgPingTransmitData;
			packet.nNetCfgTcpConnInterval	= (*i).m_nNetCfgTcpConnInterval;
			packet.nNetCfgTcpConnPort		= (*i).m_nNetCfgTcpConnPort;
			packet.nNetCfgTcpConnTTL		= (*i).m_nNetCfgTcpConnTTL;
			packet.bNetCfgTcpConnTransmitData = (*i).m_bNetCfgTcpConnTransmitData;
			_stprintf_s(packet.szNetCfgTcpConnHost, SSBC_PROFILE_MAX_SERVER_NAME, (*i).m_szNetCfgTcpConnHost);
			_stprintf_s(packet.szNetCfgPingHost, SSBC_PROFILE_MAX_SERVER_NAME, (*i).m_szNetCfgPingHost);

			break;
		}
	}
	m_cClientConfProfilesMutex.Unlock();

	return packet;
}

CString CSolidSBCClientList::GetProfileNameByID(UINT nProfileID)
{
	CString strReturn = _T("");

	m_cClientConfProfilesMutex.Lock();
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = m_vClientConfProfiles.begin(); i < m_vClientConfProfiles.end(); i++){
		if ( (*i).m_nProfileID == nProfileID ){
			strReturn = (*i).m_strName;
			break;
		}
	}
	m_cClientConfProfilesMutex.Unlock();

	return strReturn;
}

int CSolidSBCClientList::AddResultRequest(SSBC_CONN_RES_REQUEST_PACKET packet)
{
	BOOL bFound = FALSE;
	CString strTmp;
	m_cClientConfResultMutex.Lock();
	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) {
		if ( ((*i).GetSockAddr().sin_addr.s_addr == packet.client.sin_addr.s_addr) && 
			 ((*i).GetSockAddr().sin_port == packet.client.sin_port) ){

			(*i).SetClientName(packet.szClientName);
			(*i).SetProfileID (packet.nProfileID);
			(*i).SetClientUUID(packet.szClientUUID);

			{	//logging
			USES_CONVERSION;CString strLog;bFound = TRUE;
			strLog.Format( _T("Client connected, waiting for results: client id: %d, client uuid: %s, name: %s, ip:port %s:%d, profile: %s")
				, (*i).GetClientID()
				, (*i).GetClientUUID()
			    , (*i).GetClientName()
				, A2T(inet_ntoa((*i).GetSockAddr().sin_addr))
				, htons((*i).GetSockAddr().sin_port)
				, GetProfileNameByID((*i).GetProfileID()));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
			}
		}
	}

	//commiting status changes to database
	if ( m_pResultDBConnector )	{
		CSolidSBCClientResultInfo* pInfo = NULL;
		int nErr = g_cClientList.GetClientResultInfoBySockAddr( packet.client, &pInfo );
		if ( nErr && pInfo ){
			m_pResultDBConnector->AddClientHistoryResult( 
				pInfo
				, SSBC_CLIENT_ACTION_ADD_RESULT_CLIENT
				, packet.nProfileID );
			delete pInfo;
			pInfo = NULL;
		}
		else {
			//client unknown
		}
	}

	m_cClientConfResultMutex.Unlock();

	{	//logging
		if (!bFound){
			USES_CONVERSION; CString strLog; bFound = TRUE;
			strLog.Format( _T("invalid CONN_REQUEST_PACKET received,  client is not known, ignoring...: name: %s, ip:port %s:%d, profile: %s")
				, packet.szClientName
				, A2T(inet_ntoa(packet.client.sin_addr))
				, htons(packet.client.sin_port)
				, GetProfileNameByID(packet.nProfileID));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
		}
	}

	return 0;
}

int CSolidSBCClientList::DeleteResultClient(int nClientID)
{
	CString strLog;
	BOOL bFound = FALSE;

	m_cClientConfResultMutex.Lock();

	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) {
		if ( (*i).GetClientID() == nClientID ){

			if ( m_pResultDBConnector )	
				m_pResultDBConnector->AddClientHistoryResult( &(*i),SSBC_CLIENT_ACTION_DELETE_RESULT_CLIENT,0);
	

			if ( (*i).GetResultClientHandlerSocket() ){
				(*i).GetResultClientHandlerSocket()->Close();
				delete (*i).GetResultClientHandlerSocket();}
			USES_CONVERSION; bFound = TRUE;
			strLog.Format( 
				_T("result/control connection has been closed: deleting result connection: client id: %d, client uuid: %s, name: %s, ip:port %s:%d,  profile: %s")
				, (*i).GetClientID(), (*i).GetClientUUID(), (*i).GetClientName(), A2T(inet_ntoa((*i).GetSockAddr().sin_addr))
				, htons((*i).GetSockAddr().sin_port), GetProfileNameByID((*i).GetProfileID()) );
			CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);

			m_vClientResultInfos.erase( i );
			break;
		}
	}

	m_cClientConfResultMutex.Unlock();

	if ( !bFound ){
		strLog.Format( _T("unknown result/control connection has been closed: client id: %d"), nClientID );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);}

	return 0;
}

void CSolidSBCClientList::ChangeClientProfile(UINT nClientID, UINT nNewProfileID)
{
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
	BOOL bFound = FALSE;
#endif

	m_cClientConfResultMutex.Lock();

	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) {
		if ( (*i).GetClientID() == nClientID ){
			if ( (*i).GetResultClientHandlerSocket() ){
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
				{
				USES_CONVERSION; bFound = TRUE;CString strLog;
				strLog.Format( 
					_T("INFO:\n     client id: %d\n      client uuid: %s\n     name: %s\n     ip:port %s:%d\n     profile: %s -> %s\n     action: changing client profile")
					, (*i).GetClientID(), (*i).GetClientUUID(), (*i).GetClientName(), A2T(inet_ntoa((*i).GetSockAddr().sin_addr))
					, htons((*i).GetSockAddr().sin_port), GetProfileNameByID((*i).GetProfileID()), GetProfileNameByID(nNewProfileID)	);
				theApp.WriteToVMPerfSrvLog( strLog );
				}
#endif
				(*i).GetResultClientHandlerSocket()->SendProfileChangeRequest(nNewProfileID);}
		}
	}

	m_cClientConfResultMutex.Unlock();
		
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
	{
	if ( !bFound ){
		CString strLog;
		strLog.Format( _T("ERROR: could not change client profile:\n     client id: %d"), nClientID );
		theApp.WriteToVMPerfSrvLog( strLog );}
	}
#endif
}

int CSolidSBCClientList::GetProfileIDByName(CString strProfileName, UINT* pnProfileID)
{
	int nReturn = 1;

	m_cClientConfProfilesMutex.Lock();
	for ( std::vector<CSolidSBCClientConfigProfile>::iterator i = m_vClientConfProfiles.begin(); i < m_vClientConfProfiles.end(); i++){
		if ( (*i).m_strName == strProfileName ){
			*pnProfileID = (*i).m_nProfileID;
			nReturn = 0;
			break;}
	}
	m_cClientConfProfilesMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientList::GetClientResultInfoByClientID(UINT nClientID, CSolidSBCClientResultInfo** ppInfo)
{
	int nReturn = -1;
	m_cClientConfResultMutex.Lock();
	for (std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++){
		if ( (*i).GetClientID() == nClientID ){
			(*ppInfo) = new CSolidSBCClientResultInfo((*i));
			nReturn = 1;
			break;}
	}
	m_cClientConfResultMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientList::GetClientResultInfoBySockAddr(struct sockaddr_in nClientAddr, CSolidSBCClientResultInfo** ppInfo)
{
	int nReturn = -1;
	m_cClientConfResultMutex.Lock();
	for (std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++){
		if (   ((*i).GetSockAddr().sin_addr.s_addr == nClientAddr.sin_addr.s_addr)
			&& ((*i).GetSockAddr().sin_port == nClientAddr.sin_port)
		   ){
			(*ppInfo) = new CSolidSBCClientResultInfo((*i));
			nReturn = 1;
			break;}
	}
	m_cClientConfResultMutex.Unlock();

	return nReturn;
}
	
int CSolidSBCClientList::AddHDResult(PSSBC_HD_TESTRESULT pResult)
{
	if ( m_pResultDBConnector )	
		m_pResultDBConnector->AddHDResult( pResult );

	return 0;
}

int CSolidSBCClientList::AddCPUMeasureResult(PSSBC_CPUMEASURE_TESTRESULT pResult)
{
	if ( m_pResultDBConnector )	
		m_pResultDBConnector->AddCPUMeasureResult( pResult );

	return 0;
}

int CSolidSBCClientList::AddMemResult(PSSBC_MEM_TESTRESULT pResult)
{
	if ( m_pResultDBConnector )	
		m_pResultDBConnector->AddMemResult( pResult );

	return 0;
}

int CSolidSBCClientList::AddNetPingResult(PSSBC_NET_PING_TESTRESULT pResult)
{
	if ( m_pResultDBConnector )	
		m_pResultDBConnector->AddNetPingResult( pResult );

	return 0;
}

int CSolidSBCClientList::AddNetTCPConResult(PSSBC_NET_TCPCON_TESTRESULT pResult)
{
	if ( m_pResultDBConnector )	
		m_pResultDBConnector->AddNetTCPConResult( pResult );

	return 0;
}
