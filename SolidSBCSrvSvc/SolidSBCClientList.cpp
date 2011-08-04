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

void CSolidSBCClientList::Empty(void)
{
	m_cClientResultInfoMutex.Lock();

	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) 
	{
		if ( ((*i).GetResultClientHandlerSocket()) ) 
		{
			{
				USES_CONVERSION;
				CString strMsg;
				strMsg.Format( 
					_T("CSolidSBCClientList::Empty(): Deleting and closing result/control connection: client id: %d, name: %s, ip:port %s:%d")
					, (*i).GetClientID(), (*i).GetClientName(), A2T(inet_ntoa((*i).GetSockAddr().sin_addr))
					, htons((*i).GetSockAddr().sin_port));
				CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
			}

			((*i).GetResultClientHandlerSocket())->Close();
			delete ((*i).GetResultClientHandlerSocket()); 
			(*i).SetResultClientHandlerSocket(NULL);
		}
	}
	m_vClientResultInfos.clear();

	m_cClientResultInfoMutex.Unlock();
}

int CSolidSBCClientList::Init(void)
{
	if ( !m_cConfigFile.Init() )
		return 0;

	if ( !InitDBConnection() )
		return 0;

	return 1;
}

int CSolidSBCClientList::InitDBConnection(void)
{
	CString strType = _T("")
		, strDbHost = _T("")
		, strDbName = _T("")
		, strDbUser = _T("")
		, strDbPass = _T("");
	int nPort = -1;

	if ( m_cConfigFile.GetParamStr( SSBC_DATABASE_TYPE, &strType   ) != 0 ){
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
		InitDatabase(SSBC_DB_TYPE_MYSQL);
		m_pResultDBConnector = GetDatabaseConnection();}
	else if ( strType.MakeLower() == _T("mssql") ){	//TODO: mssql (not implemented)
		return 0;}
	//no database type found
	else {		
		return 0;}

	m_pResultDBConnector->SetConfig(strDbHost,nPort,strDbName, strDbUser, strDbPass);
	return m_pResultDBConnector->Connect();
}

int CSolidSBCClientList::GetClientResultInfoByClientID(UINT nClientID, CSolidSBCClientResultInfo** ppInfo)
{
	int nReturn = -1;
	m_cClientResultInfoMutex.Lock();
	for (std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++){
		if ( (*i).GetClientID() == nClientID ){
			(*ppInfo) = new CSolidSBCClientResultInfo((*i));
			nReturn = 1;
			break;}
	}
	m_cClientResultInfoMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientList::GetClientResultInfoBySockAddr(struct sockaddr_in nClientAddr, CSolidSBCClientResultInfo** ppInfo)
{
	int nReturn = -1;
	m_cClientResultInfoMutex.Lock();
	for (std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++){
		if (   ((*i).GetSockAddr().sin_addr.s_addr == nClientAddr.sin_addr.s_addr)
			&& ((*i).GetSockAddr().sin_port == nClientAddr.sin_port)
		   ){
			(*ppInfo) = new CSolidSBCClientResultInfo((*i));
			nReturn = 1;
			break;}
	}
	m_cClientResultInfoMutex.Unlock();

	return nReturn;
}

int CSolidSBCClientList::AddResultClient(const CSolidSBCClientResultInfo& clientInfo)
{
	BOOL bFound = FALSE;
	m_cClientResultInfoMutex.Lock();

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
	{
		m_vClientResultInfos.push_back(clientInfo);
		
		if ( m_pResultDBConnector )	
			m_pResultDBConnector->AddClientHistory( clientInfo.GetClientUUID(), SSBC_CLIENT_ACTION_ADD_RESULT_CLIENT);
	}

	m_cClientResultInfoMutex.Unlock();

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

int CSolidSBCClientList::DeleteResultClient(int nClientID)
{
	CString strLog;
	BOOL bFound = FALSE;

	m_cClientResultInfoMutex.Lock();

	for( std::vector<CSolidSBCClientResultInfo>::iterator i = m_vClientResultInfos.begin(); i < m_vClientResultInfos.end(); i++) {
		if ( (*i).GetClientID() == nClientID ){

			if ( m_pResultDBConnector )	
				m_pResultDBConnector->AddClientHistory( (*i).GetClientUUID(), SSBC_CLIENT_ACTION_DELETE_RESULT_CLIENT);
	

			if ( (*i).GetResultClientHandlerSocket() ){
				(*i).GetResultClientHandlerSocket()->Close();
				delete (*i).GetResultClientHandlerSocket();}
			USES_CONVERSION; bFound = TRUE;
			strLog.Format( 
				_T("result/control connection has been closed: deleting result connection: client id: %d, client uuid: %s, name: %s, ip:port %s:%d")
				, (*i).GetClientID(), (*i).GetClientUUID(), (*i).GetClientName(), A2T(inet_ntoa((*i).GetSockAddr().sin_addr))
				, htons((*i).GetSockAddr().sin_port) );
			CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);

			m_vClientResultInfos.erase( i );
			break;
		}
	}

	m_cClientResultInfoMutex.Unlock();

	if ( !bFound ){
		strLog.Format( _T("unknown result/control connection has been closed: client id: %d"), nClientID );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_WARN);}

	return 0;
}

int CSolidSBCClientList::AddTestResult(const CSolidSBCClientResultInfo& clientInfo, const CString& strTestSQL)
{
	if ( m_pResultDBConnector )	{
		m_pResultDBConnector->AddTestResult(clientInfo.GetClientUUID() ,strTestSQL);
		return 0; }
	else 
		return 1;
}

int CSolidSBCClientList::GetConfigsForClient(const CString& strClientUUID, std::vector<CString>& vecXmlConfigs)
{
	if ( m_pResultDBConnector )	{
		m_pResultDBConnector->GetConfigsForClient(strClientUUID,vecXmlConfigs);
		return 0; } 	
	else 
		return 1;
}