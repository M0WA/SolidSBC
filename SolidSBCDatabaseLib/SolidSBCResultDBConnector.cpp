#include "StdAfx.h"
#include "SolidSBCResultDBConnector.h"

CSolidSBCResultDBConnector::CSolidSBCResultDBConnector(void)
: m_strServer	(_T(""))
, m_nPort		(0)
, m_strDatabase	(_T(""))
, m_strUser		(_T(""))
, m_strPassword	(_T(""))
, m_Type		(SSBC_DB_TYPE_NONE)
{
}

CSolidSBCResultDBConnector::~CSolidSBCResultDBConnector(void)
{
}

void CSolidSBCResultDBConnector::SetConfig(CString strServer, DWORD nPort, CString strDatabase, CString strUser, CString strPassword)
{
	m_strServer	  = strServer;
	m_nPort		  = nPort;
	m_strDatabase = strDatabase;
	m_strUser	  = strUser;
	m_strPassword = strPassword;
}

int CSolidSBCResultDBConnector::GetConfigsForClientSQLString(const CString& strClientUUID, CStringArray& arSQLCmds)
{
	CString strSQL;
	strSQL.Format(
		_T("SELECT `config`.`xmlConfig` ")
		_T("FROM `%s`.`Clients` AS `client` ")
		_T("INNER JOIN `%s`.`ClientsConfigs` AS `clientConfig` ON ( `clientConfig`.`CLIENT_ID` = `client`.`ID` ) ")
		_T("INNER JOIN `%s`.`Configs`        AS `config`       ON ( `clientConfig`.`CONFIG_ID` = `config`.`ID` ) ")
		_T("WHERE `client`.`uuid` = '%s' ")
		, m_strDatabase
		, m_strDatabase
		, m_strDatabase
		, strClientUUID
		);
	arSQLCmds.Add(strSQL);

	/*
	{
		CString strLog;
		strLog.Format( _T("CSolidSBCResultDBConnector: Getting configs: \n %s"), strSQL );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
	*/
	return 0;
}

int CSolidSBCResultDBConnector::GetAddClientHistorySQLString(const CString& strClientUUID, SSBC_CLIENT_HISTORY_ACTION nAction, CStringArray& arSQLCmds)
{	
	CString strSQL;
	strSQL.Format(
		_T("INSERT INTO `%s`.`ClientHistory` (`CLIENT_ID`,`action`,`actiontime`) ")
		_T("(")
			_T(" SELECT `client`.`ID`, '%d', NOW() ")
			_T(" FROM `%s`.`Clients` AS `client`   ")
			_T(" WHERE `client`.`uuid` = '%s'      ")
		_T(")")
		, m_strDatabase
		, nAction
		, m_strDatabase
		, strClientUUID
		);
	arSQLCmds.Add(strSQL);

	/*
	{
		CString strLog;
		strLog.Format( _T("CSolidSBCResultDBConnector: Inserting client history: \n%s"), strSQL );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
	*/
	return 0;
}

int CSolidSBCResultDBConnector::GetAddTestResultSQLString(const CString& strClientUUID, const CString& strTestSQL, CStringArray& arSQLCmds)
{
	CString strClientUuidSubSelectSQL;
	strClientUuidSubSelectSQL.Format(
		_T(" (SELECT `Clients`.`ID` ")
		_T("FROM `%s`.`Clients` ")
		_T("WHERE `Clients`.`uuid` = '%s') ")
		, m_strDatabase
		, strClientUUID
		);
		
	CString strSQL;
	strSQL.Format(
		strTestSQL
		, strClientUuidSubSelectSQL
		);
	arSQLCmds.Add(strSQL);

	/*
	{
		CString strLog;
		strLog.Format( _T("Inserting test result: \n%s"), strSQL );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
	*/
	return 0;
}

int CSolidSBCResultDBConnector::GetNameFromUuidSQLString(const CString& strUuid, CStringArray& arSQLCmds)
{
	CString strSQL;
	strSQL.Format(
		_T("SELECT `client`.`name` ")
		_T("FROM `%s`.`Clients` AS `client` ")
		_T("WHERE `client`.`uuid` = '%s' ")
		, m_strDatabase
		, strUuid
		);
	arSQLCmds.Add(strSQL);

	/*
	{
		CString strLog;
		strLog.Format( _T("Getting name for client: \n%s"), strSQL );
		CSolidSBCSrvServiceWnd::LogServiceMessage(strLog,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
	*/
	return 0;
}