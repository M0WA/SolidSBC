#include "StdAfx.h"
#include "SolidSBCResultDBConnectorMySQL.h"
	
#include <mysql.h>

MYSQL *m_conn = NULL;

CSolidSBCResultDBConnectorMySQL::CSolidSBCResultDBConnectorMySQL(void)
: CSolidSBCResultDBConnector()
{
	m_Type = SSBC_DB_TYPE_MYSQL;
	m_conn = NULL;
}

CSolidSBCResultDBConnectorMySQL::~CSolidSBCResultDBConnectorMySQL(void)
{
	Disconnect();
}

int CSolidSBCResultDBConnectorMySQL::Connect(void)
{
	m_DBConnMutex.Lock();

	USES_CONVERSION;
	if ( m_conn )
		 mysql_close(m_conn);

    m_conn = mysql_init(NULL);

	int nReturn = 1;
	BOOL bCompression = TRUE;
	//try to connect with compression
	if ( mysql_real_connect(m_conn, T2A(m_strServer), T2A(m_strUser), T2A(m_strPassword), T2A(m_strDatabase), m_nPort, NULL, CLIENT_COMPRESS) == NULL ){
		//if failed, try to connect w/o compression
		if ( mysql_real_connect(m_conn, T2A(m_strServer), T2A(m_strUser), T2A(m_strPassword), T2A(m_strDatabase), m_nPort, NULL, 0) == NULL )
		{
			/*
			CString strLog;
			strLog.Format( _T("ERROR: Could not connect to MySQL database: Error: %s"), A2T(mysql_error(m_conn)) );
			CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
			*/
			nReturn = 0;
		}
		else
			bCompression = FALSE;
	}
	
	/*
	if ( nReturn ) {
		CString strLog;
		strLog.Format( _T("INFO: Successfully connected to MySQL database. Compression: %d"), bCompression );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
		}
	*/

	m_DBConnMutex.Unlock();
	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::Disconnect(void)
{
	/*
	{
		CString strLog;
		strLog.Format( _T("INFO: Closing connection to MySQL database.") );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
	}
	*/

	m_DBConnMutex.Lock();
	if ( m_conn )
		 mysql_close(m_conn);
	m_conn = NULL;
	m_DBConnMutex.Unlock();

	return 0;
}

int CSolidSBCResultDBConnectorMySQL::GetConfigsForClient(const CString& strClientUUID, std::vector<CString>& vecXmlConfigs)
{
	USES_CONVERSION;
	CStringArray arSQLCmds;
	if ( CSolidSBCResultDBConnector::GetConfigsForClientSQLString( strClientUUID, arSQLCmds ) != 0 )
		return 1;
	
	int nReturn = 0;
	vecXmlConfigs.clear();

	m_DBConnMutex.Lock();
	for (int i = 0; i < arSQLCmds.GetSize(); i++)
	{
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
			MYSQL_RES* res = mysql_use_result(m_conn);
			if( !res )
				continue;

			MYSQL_ROW row;
			while ( ( row = mysql_fetch_row(res) ) )
				vecXmlConfigs.push_back(A2T(row[0]));

			mysql_free_result(res);
		}
	}
	m_DBConnMutex.Unlock();

	return 0;
}

int CSolidSBCResultDBConnectorMySQL::AddTestResult(const CString& strClientUUID, const CString& strTestSQL)
{
	USES_CONVERSION;
	CStringArray arSQLCmds;
	if ( CSolidSBCResultDBConnector::GetAddTestResultSQLString(strClientUUID, strTestSQL, arSQLCmds) != 0 )
		return 1;

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
			nReturn = 1;
			break;
		}
	}
	m_DBConnMutex.Unlock();

	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::AddClientHistory(const CString& strClientUUID, SSBC_CLIENT_HISTORY_ACTION nAction)
{
	USES_CONVERSION;
	CStringArray arSQLCmds;

	if ( CSolidSBCResultDBConnector::GetAddClientHistorySQLString(strClientUUID, nAction, arSQLCmds) != 0 )
		return 1;

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
			nReturn = 1;
			break;
		}
	}
	m_DBConnMutex.Unlock();

	return nReturn;
}