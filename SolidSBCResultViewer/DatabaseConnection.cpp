#include "StdAfx.h"
#include "DatabaseConnection.h"

CDatabaseConnection g_cDatabaseConnection;

CDatabaseConnection::CDatabaseConnection(void)
: m_conn(NULL)
, m_strServer(_T(""))
, m_strUser(_T(""))
, m_strPassword(_T(""))
, m_strDatabase(_T(""))
, m_nPort(0)
{
}

CDatabaseConnection::~CDatabaseConnection(void)
{
	Disconnect();
}

int CDatabaseConnection::Connect(CString strServer, CString strUser, CString strPassword, CString strDatabase, int nPort)
{
	m_strServer   = strServer;
	m_strUser     = strUser;
	m_strPassword = strPassword;
	m_strDatabase = strDatabase;
	m_nPort       = nPort;

	m_cDatabaseLock.Lock();

	USES_CONVERSION;
	if ( m_conn )
		 mysql_close(m_conn);

    m_conn = mysql_init(NULL);

	int nReturn = 1;
	BOOL bCompression = TRUE;
	//try to connect with compression
	if ( mysql_real_connect(m_conn, T2A(strServer), T2A(strUser), T2A(strPassword), T2A(strDatabase), nPort, NULL, CLIENT_COMPRESS) == NULL ){
		//if failed, try to connect w/o compression
		if ( mysql_real_connect(m_conn, T2A(strServer), T2A(strUser), T2A(strPassword), T2A(strDatabase), nPort, NULL, 0) == NULL )
		{
			m_conn = NULL;
			Disconnect();
			nReturn = 0;
		}
		else
			bCompression = FALSE;
	}

	if ( nReturn ) {

		RefreshClientIdentities();

	}

	m_cDatabaseLock.Unlock();

	return nReturn;
}

int CDatabaseConnection::Disconnect(void)
{
	m_cDatabaseLock.Lock();

	if ( m_conn )
		 mysql_close(m_conn);
	m_conn = NULL;

	m_cDatabaseLock.Unlock();
	
	m_strServer   = _T("");
	m_strUser     = _T("");
	m_strPassword = _T("");
	m_strDatabase = _T("");
	m_nPort       = 0;

	m_vClientIdentities.clear();

	return 0;
}

bool CDatabaseConnection::IsConnected()
{
	bool bReturn = false;

	m_cDatabaseLock.Lock();
	if ( m_conn != NULL )
		bReturn = true;
	m_cDatabaseLock.Unlock();

	return bReturn;
}

int CDatabaseConnection::GetUniqueClients(std::vector<CString>* pvClientUUIDs)
{
	if ( !IsConnected())
		return 0;

	USES_CONVERSION;
	CString strSqlCmd =_T("SELECT DISTINCT ext_clientuuid FROM client_history");

	m_cDatabaseLock.Lock();
	mysql_query(m_conn,T2A(strSqlCmd));
	MYSQL_RES *result = mysql_store_result(m_conn);
	m_cDatabaseLock.Unlock();	
	
	my_ulonglong nReturnedClients = mysql_num_rows(result);

	MYSQL_ROW row;
	while ( (row = mysql_fetch_row(result)) )
		pvClientUUIDs->push_back( A2T( row[0] ) );

	mysql_free_result(result);
	return (int)nReturnedClients;
}

int CDatabaseConnection::GetHDResultCount( CString strClientUUID, int nIdentity )
{
	USES_CONVERSION;

	if ( !IsConnected())
		return 0;

	int nResults = 0;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT COUNT(id_hdResults) FROM hdResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			if ( result && mysql_num_rows(result) ){
				MYSQL_ROW row = mysql_fetch_row(result);
				nResults += atoi(row[0]);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}
	
	return nResults;
}

std::vector<SSBC_CLIENT_HD_RESULT> CDatabaseConnection::GetClientHDResults(CString strClientUUID, int nIdentity)
{
	std::vector<SSBC_CLIENT_HD_RESULT> vReturn;
	
	USES_CONVERSION;

	if ( !IsConnected())
		return vReturn;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT resulttype,duration,amountbytes,wait,resultdatetime FROM hdResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();
		
			MYSQL_ROW row = mysql_fetch_row(result);
			while ( result && mysql_num_rows(result) && row){	
	
				SSBC_CLIENT_HD_RESULT hdResult;
				hdResult.strType = A2T(row[0]);
				hdResult.strDuration = A2T(row[1]);
				hdResult.strBytes = A2T(row[2]);
				hdResult.strWait = A2T(row[3]);
				hdResult.strTime = A2T(row[4]);

				vReturn.push_back(hdResult);

				row = mysql_fetch_row(result);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}

	return vReturn;
}

int CDatabaseConnection::GetCPUMeasureResultCount( CString strClientUUID, int nIdentity )
{
	USES_CONVERSION;

	if ( !IsConnected())
		return 0;

	int nResults = 0;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT COUNT(id_cpumeasureResults) FROM cpumeasureResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			if ( result && mysql_num_rows(result) ){
				MYSQL_ROW row = mysql_fetch_row(result);
				nResults += atoi(row[0]);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}
	
	return nResults;
}

std::vector<SSBC_CLIENT_CPUMEASURE_RESULT> CDatabaseConnection::GetClientCPUMeasureResults(CString strClientUUID, int nIdentity)
{
	std::vector<SSBC_CLIENT_CPUMEASURE_RESULT> vReturn;
	
	USES_CONVERSION;

	if ( !IsConnected())
		return vReturn;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT addduration,divisionduration,overallduration,addmultiplier,divisionmultiplier,resultdatetime FROM cpumeasureResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();
		
			MYSQL_ROW row = mysql_fetch_row(result);
			while ( result && mysql_num_rows(result) && row){	
			
				SSBC_CLIENT_CPUMEASURE_RESULT cpuResult;
				cpuResult.strAddDuration   = A2T(row[0]);
				cpuResult.strDivDuration   = A2T(row[1]);
				cpuResult.strAllDuration   = A2T(row[2]);
				cpuResult.strAddMultiplier = A2T(row[3]);
				cpuResult.strDivMultiplier = A2T(row[4]);
				cpuResult.strTime          = A2T(row[5]);

				vReturn.push_back(cpuResult);

				row = mysql_fetch_row(result);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}

	return vReturn;
}

int CDatabaseConnection::GetMemResultCount( CString strClientUUID, int nIdentity )
{
	USES_CONVERSION;

	if ( !IsConnected())
		return 0;

	int nResults = 0;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT COUNT(idmemoryResults) FROM memResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			if ( result && mysql_num_rows(result)){
				MYSQL_ROW row = mysql_fetch_row(result);
				nResults += atoi(row[0]);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}
	
	return nResults;
}

std::vector<SSBC_CLIENT_MEM_RESULT> CDatabaseConnection::GetClientMemResults( CString strClientUUID, int nIdentity )
{
	std::vector<SSBC_CLIENT_MEM_RESULT> vReturn;
	
	USES_CONVERSION;

	if ( !IsConnected())
		return vReturn;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT extclient_id,malloczeroduration,bytes,resultdatetime FROM memResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			MYSQL_ROW row = mysql_fetch_row(result);
			while ( result && mysql_num_rows(result) && row){			
			
				SSBC_CLIENT_MEM_RESULT memResult;
				memResult.strClientID           = A2T(row[0]);
				memResult.strMallocZeroDuration = A2T(row[1]);
				memResult.strBytes              = A2T(row[2]); 
				memResult.strTime               = A2T(row[3]); 

				vReturn.push_back(memResult);

				row = mysql_fetch_row(result);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}

	return vReturn;
}

int CDatabaseConnection::GetPingResultCount( CString strClientUUID, int nIdentity )
{
	USES_CONVERSION;

	if ( !IsConnected())
		return 0;

	int nResults = 0;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT COUNT(idpingResults) FROM pingResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			if ( result && mysql_num_rows(result) ){
				MYSQL_ROW row = mysql_fetch_row(result);
				nResults += atoi(row[0]);
			}	

			mysql_free_result(result);
		}

		nIdentCount++;
	}
	
	return nResults;
}

std::vector<SSBC_CLIENT_PING_RESULT> CDatabaseConnection::GetClientPingResults(CString strClientUUID, int nIdentity)
{
	std::vector<SSBC_CLIENT_PING_RESULT> vReturn;

	USES_CONVERSION;

	if ( !IsConnected())
		return vReturn;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT rtt_duration,icmp_opts,resultdatetime FROM pingResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			MYSQL_ROW row = mysql_fetch_row(result);
			while ( result && mysql_num_rows(result) && row){			
			
				SSBC_CLIENT_PING_RESULT pingResult;
				pingResult.strRTTDuration = A2T(row[0]);
				pingResult.strICMPOpts    = A2T(row[1]);
				pingResult.strTime        = A2T(row[2]);

				vReturn.push_back(pingResult);

				row = mysql_fetch_row(result);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}

	return vReturn;
}

int CDatabaseConnection::GetTCPResultCount( CString strClientUUID, int nIdentity )
{
	USES_CONVERSION;

	if ( !IsConnected())
		return 0;

	int nResults = 0;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT COUNT(idtcpconResults) FROM tcpconResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			if ( result && mysql_num_rows(result) ){
				MYSQL_ROW row = mysql_fetch_row(result);
				nResults += atoi(row[0]);
			}	

			mysql_free_result(result);
		}

		nIdentCount++;
	}
	
	return nResults;
}

std::vector<SSBC_CLIENT_TCPCON_RESULT> CDatabaseConnection::GetClientTCPConResults(CString strClientUUID, int nIdentity)
{
	std::vector<SSBC_CLIENT_TCPCON_RESULT> vReturn;

	USES_CONVERSION;

	if ( !IsConnected())
		return vReturn;

	std::vector<SSBC_CLIENT_IDENTITY> vIdentities = GetClientIdentities(strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = vIdentities.begin();
	int nIdentCount = 0;
	for ( iCliIter = vIdentities.begin(); iCliIter < vIdentities.end(); iCliIter++)
	{
		if ( ( nIdentity == -1 ) || (nIdentity == nIdentCount) ){
			CString strSqlCmd =_T("");
			strSqlCmd.Format(_T("SELECT handshake_duration,resultdatetime FROM tcpconResults WHERE extclient_id=%d AND resultdatetime >= \"%s\" AND resultdatetime <= \"%s\""),(*iCliIter).nClientID, (*iCliIter).strBeginTime, (*iCliIter).strEndTime );

			m_cDatabaseLock.Lock();
			mysql_query(m_conn,T2A(strSqlCmd));
			MYSQL_RES *result = mysql_store_result(m_conn);
			m_cDatabaseLock.Unlock();

			MYSQL_ROW row = mysql_fetch_row(result);
			while ( result && mysql_num_rows(result) && row){			
			
				SSBC_CLIENT_TCPCON_RESULT tcpResult;
				tcpResult.strHandshakeDuration = A2T(row[0]);
				tcpResult.strTime              = A2T(row[1]);

				vReturn.push_back(tcpResult);

				row = mysql_fetch_row(result);
			}	

			mysql_free_result(result);
		}
		nIdentCount++;
	}

	return vReturn;
}

int CDatabaseConnection::RefreshClientIdentities(void)
{
	USES_CONVERSION;

	//get all unique clients
	std::vector<CString> vClientUUIDs;
	GetUniqueClients(&vClientUUIDs);

	m_vClientIdentities.clear();
	std::vector<CString>::iterator iCliUUIDIter = vClientUUIDs.begin();
	for ( iCliUUIDIter = vClientUUIDs.begin(); iCliUUIDIter < vClientUUIDs.end(); iCliUUIDIter++)
	{
		CString strSqlCmd = _T("");
		strSqlCmd.Format(_T("SELECT ext_clientid,actiondatetime,ext_clientname FROM client_history WHERE ext_clientuuid=\"%s\" AND action=1"),(*iCliUUIDIter));

		m_cDatabaseLock.Lock();
		mysql_query(m_conn,T2A(strSqlCmd));
		MYSQL_RES *result = mysql_store_result(m_conn);
		m_cDatabaseLock.Unlock();
		
		int nReturnedIdentities = (int)mysql_num_rows(result);

		MYSQL_ROW row;
		while ( (row = mysql_fetch_row(result)) )
		{
			SSBC_CLIENT_IDENTITY identity;
			identity.strClientUUID = (*iCliUUIDIter);
			identity.nClientID     = atoi(row[0]);
			identity.strBeginTime  = A2T(row[1]);
			identity.strClientName = A2T(row[2]);
			CString strEndTime     = GetClientEndTime(identity.strClientUUID,identity.nClientID,identity.strBeginTime);
			identity.strEndTime    = strEndTime;
			if ( strEndTime == _T("") ){
					CString strMsg;
					strMsg.Format(_T("Could not find clients end time:\r\n%s ( UUID: %s )\r\nid: %d start:%s")
						,identity.strClientName,identity.strClientUUID,identity.nClientID,identity.strBeginTime);
					AfxMessageBox(strMsg, MB_OK | MB_ICONEXCLAMATION);
			}

			m_vClientIdentities.push_back(identity);
		}

		mysql_free_result(result);
	}

	return 0;
}

CString CDatabaseConnection::GetClientEndTime(CString strClientUUID, int nExtClientID, CString strStartTime)
{
	USES_CONVERSION;
	CString strSqlCmd = _T("");
	strSqlCmd.Format(_T("SELECT actiondatetime FROM client_history WHERE ext_clientuuid=\"%s\" AND ext_clientid=%d AND actiondatetime > \"%s\" AND action=2 LIMIT 1"),strClientUUID,nExtClientID,strStartTime);
	
	m_cDatabaseLock.Lock();
	mysql_query(m_conn,T2A(strSqlCmd));
	MYSQL_RES *result = mysql_store_result(m_conn);
	m_cDatabaseLock.Unlock();

	if ( !result || !mysql_num_rows(result) ){
		mysql_free_result(result);
		return _T("");}
	
	MYSQL_ROW row = mysql_fetch_row(result);
	CString strReturn = A2T(row[0]);
	mysql_free_result(result);

	return strReturn;
}

std::vector<SSBC_CLIENT_IDENTITY> CDatabaseConnection::GetClientIdentities(CString strClientUUID)
{
	std::vector<SSBC_CLIENT_IDENTITY> vReturn;
	
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = m_vClientIdentities.begin();
	for ( iCliIter = m_vClientIdentities.begin(); iCliIter < m_vClientIdentities.end(); iCliIter++)
	{
		if ( (*iCliIter).strClientUUID == strClientUUID )
		{
			vReturn.push_back( (*iCliIter) );
		}
	}

	return vReturn;
}

std::vector<CString> CDatabaseConnection::GetClientNames(CString strClientUUID)
{
	std::vector<CString> vReturn;
	std::vector<SSBC_CLIENT_IDENTITY>::iterator iCliIter = m_vClientIdentities.begin();
	for ( iCliIter = m_vClientIdentities.begin(); iCliIter < m_vClientIdentities.end(); iCliIter++)
	{
		if ( (*iCliIter).strClientUUID == strClientUUID )
		{
			bool bFound = false;
			std::vector<CString>::iterator iReturnIter = vReturn.begin();
			for ( iReturnIter = vReturn.begin(); iReturnIter < vReturn.end(); iReturnIter++){
				
				if ( (*iReturnIter) == (*iCliIter).strClientName ){
					bFound = true;
					break;}
			}

			if (!bFound)
				vReturn.push_back( (*iCliIter).strClientName );
		}
	}
	return vReturn;
}

std::vector<SSBC_CLIENT_HISTORY> CDatabaseConnection::GetClientHistory(CString strClientUUID)
{
	std::vector<SSBC_CLIENT_HISTORY> vReturn;
	
	USES_CONVERSION;
	CString strSqlCmd = _T("");
	strSqlCmd.Format(_T("SELECT ext_clientid,ext_clientname,ext_clientuuid,action,actionparam,actiondatetime FROM client_history WHERE ext_clientuuid=\"%s\""),strClientUUID);

	m_cDatabaseLock.Lock();
	mysql_query(m_conn,T2A(strSqlCmd));
	MYSQL_RES *result = mysql_store_result(m_conn);
	m_cDatabaseLock.Unlock();

	if ( !result || !mysql_num_rows(result) ){
		mysql_free_result(result);
		return vReturn;}
	
	MYSQL_ROW row;
	while ( (row = mysql_fetch_row(result)) )
	{
		SSBC_CLIENT_HISTORY history;
		history.strClientID   = A2T(row[0]);
		history.strClientName = A2T(row[1]);
		history.strClientUUID = A2T(row[2]);
		history.strAction     = A2T(row[3]);
		history.strParam      = A2T(row[4]);
		history.strTime       = A2T(row[5]);

		vReturn.push_back(history);
	}

	return vReturn;
}
