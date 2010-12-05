#include "StdAfx.h"
#include "SolidSBCResultDBConnectorMySQL.h"


CSolidSBCResultDBConnectorMySQL::CSolidSBCResultDBConnectorMySQL(void)
: CSolidSBCResultDBConnector()
, m_conn(NULL)
{
	m_Type = SSBC_DB_TYPE_MYSQL;
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
			CString strLog;
			strLog.Format( _T("ERROR: Could not connect to MySQL database: Error: %s"), A2T(mysql_error(m_conn)) );
			CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
			nReturn = 0;
		}
		else
			bCompression = FALSE;
	}

	if ( nReturn ) {
		CString strLog;
		strLog.Format( _T("INFO: Successfully connected to MySQL database. Compression: %d"), bCompression );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);}

	m_DBConnMutex.Unlock();
	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::Disconnect(void)
{

	CString strLog;
	strLog.Format( _T("INFO: Closing connection to MySQL database.") );
	CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_INFO);

	m_DBConnMutex.Lock();
	if ( m_conn )
		 mysql_close(m_conn);
	m_conn = NULL;
	m_DBConnMutex.Unlock();

	return 0;
}

int CSolidSBCResultDBConnectorMySQL::AddHDResult(PSSBC_HD_TESTRESULT pResult)
{
	USES_CONVERSION;
	CStringArray arSQLCmds;
	if ( CSolidSBCResultDBConnector::GetHDSQLString(pResult, &arSQLCmds) != 0 ){
		return 1;}

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {

#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
			int nDebug = 0;
			if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
				nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

			if (nDebug){
				CString strLog;
				strLog.Format( _T("ERROR:\n     Harddrive result could not be added to database\n     Error: %s\n     !!! result will be lost !!!"), A2T(mysql_error(m_conn)) );
				theApp.WriteToVMPerfSrvLog( strLog );
			}
#endif

			nReturn = 1;
			break;
		}
	}

	m_DBConnMutex.Unlock();
	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::AddCPUMeasureResult(PSSBC_CPUMEASURE_TESTRESULT pResult)
{

	USES_CONVERSION;
	CStringArray arSQLCmds;

	if ( CSolidSBCResultDBConnector::GetCPUMeasureSQLString(pResult, &arSQLCmds) != 0 ){
		return 1;}

	int nReturn = 0;
	m_DBConnMutex.Lock();
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
			int nDebug = 0;
			if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
				nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

			if (nDebug){
				CString strLog;
				strLog.Format( _T("ERROR:\n     CPUMeasure result could not be added to database\n     Error: %s\n     !!! result will be lost !!!"), A2T(mysql_error(m_conn)) );
				theApp.WriteToVMPerfSrvLog( strLog );
			}
#endif
			nReturn = 1;
			break;
		}
	}

	m_DBConnMutex.Unlock();
	return nReturn;
}

int  CSolidSBCResultDBConnectorMySQL::AddMemResult(PSSBC_MEM_TESTRESULT pResult)
{
	USES_CONVERSION;
	CStringArray arSQLCmds;

	if ( CSolidSBCResultDBConnector::GetMemSQLString(pResult, &arSQLCmds) != 0 ){
		return 1;}

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
			int nDebug = 0;
			if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
				nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

			if (nDebug){
				CString strLog;
				strLog.Format( _T("ERROR:\n     Memory result could not be added to database\n     Error: %s\n     !!! result will be lost !!!"), A2T(mysql_error(m_conn)) );
				theApp.WriteToVMPerfSrvLog( strLog );
			}
#endif
			nReturn = 1;
			break;
		}
	}

	m_DBConnMutex.Unlock();
	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::AddNetPingResult(PSSBC_NET_PING_TESTRESULT pResult)
{	
	USES_CONVERSION;
	CStringArray arSQLCmds;

	if ( CSolidSBCResultDBConnector::GetNetPingSQLString(pResult, &arSQLCmds) != 0 ){
		return 1;}

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
			int nDebug = 0;
			if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
				nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

			if (nDebug){
				CString strLog;
				strLog.Format( _T("ERROR:\n     Network ping result could not be added to database\n     Error: %s\n     !!! result will be lost !!!"), A2T(mysql_error(m_conn)) );
				theApp.WriteToVMPerfSrvLog( strLog );
			}
#endif
			nReturn = 1;
			break;
		}
	}
	m_DBConnMutex.Unlock();
	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::AddNetTCPConResult(PSSBC_NET_TCPCON_TESTRESULT pResult)
{	
	USES_CONVERSION;
	CStringArray arSQLCmds;

	if ( CSolidSBCResultDBConnector::GetNetTCPConSQLString(pResult, &arSQLCmds) != 0 ){
		return 1;}

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
			int nDebug = 0;
			if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
				nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

			if (nDebug){
				CString strLog;
				strLog.Format( _T("ERROR:\n     Network ping result could not be added to database\n     Error: %s\n     !!! result will be lost !!!"), A2T(mysql_error(m_conn)) );
				theApp.WriteToVMPerfSrvLog( strLog );
			}
#endif
			nReturn = 1;
			break;
		}
	}

	m_DBConnMutex.Unlock();
	return nReturn;
}

int CSolidSBCResultDBConnectorMySQL::AddClientHistoryResult(CSolidSBCClientResultInfo* pResult, SSBC_CLIENT_HISTORY_ACTION nAction, UINT nActionParam)
{
	USES_CONVERSION;
	CStringArray arSQLCmds;

	if ( CSolidSBCResultDBConnector::GetClientHistorySQLString(pResult, nAction, nActionParam, &arSQLCmds) != 0 )
		return 1;

	m_DBConnMutex.Lock();
	int nReturn = 0;
	for (int i = 0; i < arSQLCmds.GetSize(); i++){
		if ( mysql_query( m_conn, T2A(arSQLCmds.GetAt(i)) ) != 0 )
        {
#ifdef  VMPERFORMER_SRV_ENABLE_LOGGING
			int nDebug = 0;
			if (theApp.m_cConfigFile.GetParamInt(VMPERFORMER_CFGFILE_SRV_LOG_DEBUG,&nDebug) != 0)
				nDebug = VMPERFORMER_CFGFILE_SRV_LOG_DEBUG_DEFAULT;

			if (nDebug){
				CString strLog;
				strLog.Format( _T("ERROR:\n     Client history could not be updated in the database\n     Error: %s\n     !!! result will be lost !!!"), A2T(mysql_error(m_conn)) );
				theApp.WriteToVMPerfSrvLog( strLog );
			}
#endif
			nReturn = 1;
			break;
		}
	}

	m_DBConnMutex.Unlock();
	return nReturn;
}