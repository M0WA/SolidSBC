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

int CSolidSBCResultDBConnector::GetHDSQLString(PSSBC_HD_TESTRESULT pResult, CStringArray* parSQLCmds)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO `%s`.`hdResults` ( `extresult_id`, `extclient_id`, `resulttype`, `duration`, `amountbytes`, `wait`, `resultdatetime` ) VALUES ( %d, %d, %d, %f, %d, %d, NOW() )")
		, m_strDatabase
		, pResult->nResultID
		, pResult->nClientID
		, pResult->hdResult.nType
		, pResult->hdResult.dSeconds
		, pResult->hdResult.ulBytes
		, pResult->hdResult.ulWait
		);
	parSQLCmds->Add(strSQL);

	return 0;
}

int CSolidSBCResultDBConnector::GetCPUMeasureSQLString(PSSBC_CPUMEASURE_TESTRESULT pResult, CStringArray* parSQLCmds)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO `%s`.`cpumeasureResults` ( `extresult_id`, `extclient_id`, `addduration`, `divisionduration`, `overallduration`, `addmultiplier`, `divisionmultiplier`, `resultdatetime` ) VALUES ( %d, %d, %f, %f, %f, %I64u, %I64u, NOW() )")
		, m_strDatabase
		, pResult->nResultID
		, pResult->nClientID
		, pResult->cpuMeasure.dAddDuration
		, pResult->cpuMeasure.dDivDuration
		, pResult->cpuMeasure.dOverallDuration
		, pResult->cpuMeasure.ullAddMultiplier
		, pResult->cpuMeasure.ullDivMultiplier
		);
	parSQLCmds->Add(strSQL);
	return 0;
}

int CSolidSBCResultDBConnector::GetMemSQLString(PSSBC_MEM_TESTRESULT	pResult, CStringArray* parSQLCmds)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO `%s`.`memResults` ( `extresult_id`, `extclient_id`, `malloczeroduration`, `bytes`, `resultdatetime` ) VALUES ( %d, %d, %f, %d, NOW() )")
		, m_strDatabase
		, pResult->nResultID
		, pResult->nClientID
		, pResult->memResult.dMallocZeroDuration
		, pResult->memResult.ulBytes
		);
	parSQLCmds->Add(strSQL);
	return 0;
}

int CSolidSBCResultDBConnector::GetNetPingSQLString(PSSBC_NET_PING_TESTRESULT	pResult, CStringArray* parSQLCmds)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO `%s`.`pingResults` (`extresult_id`, `extclient_id`, `rtt_duration`, `icmp_opts`, `resultdatetime` ) VALUES ( %d, %d, %d, %d,  NOW() )")
		, m_strDatabase
		, pResult->nResultID
		, pResult->nClientID
		, pResult->netPingResult.icmpReply.RoundTripTime
		, pResult->netPingResult.icmpReply.Status
		);
	parSQLCmds->Add(strSQL);
	return 0;
}

int CSolidSBCResultDBConnector::GetNetTCPConSQLString(PSSBC_NET_TCPCON_TESTRESULT	pResult, CStringArray* parSQLCmds)
{
	CString strSQL;
	strSQL.Format(_T("INSERT INTO `%s`.`tcpconResults` (`extresult_id`, `extclient_id`, `handshake_duration`, `resultdatetime` ) VALUES ( %d, %d, %f,  NOW() )")
		, m_strDatabase
		, pResult->nResultID
		, pResult->nClientID
		, pResult->netTCPConResult.dDuration
		);
	parSQLCmds->Add(strSQL);
	return 0;
}

int CSolidSBCResultDBConnector::GetClientHistorySQLString(CSolidSBCClientResultInfo* pResult, SSBC_CLIENT_HISTORY_ACTION nAction, UINT nActionParam, CStringArray* parSQLCmds)
{
	USES_CONVERSION;
	CString strSQL;
	strSQL.Format(_T("INSERT INTO `%s`.`client_history` ( `ext_clientid`, `ext_clientuuid`, `ext_clientname`, `ext_clientip`, `ext_clientport`, `action`, `actionparam`, `actiondatetime` ) VALUES ( %d, \"%s\", \"%s\", \"%s\", %d, %d, %d, NOW() )")
		, m_strDatabase
		, pResult->GetClientID()
		, pResult->GetClientUUID()
		, pResult->GetClientName()
		, A2T(inet_ntoa(pResult->GetSockAddr().sin_addr))
		, ntohs(pResult->GetSockAddr().sin_port)
		, nAction
		, nActionParam
		);
	parSQLCmds->Add(strSQL);
	return 0;
}
