#pragma once

#include "SolidSBCResultDBConnector.h"

class CSolidSBCResultDBConnectorMySQL : public CSolidSBCResultDBConnector
{
public:
	CSolidSBCResultDBConnectorMySQL				(void);
	virtual ~CSolidSBCResultDBConnectorMySQL	(void);
	virtual int Connect							(void);
	virtual int Disconnect						(void);

	virtual int AddHDResult				(PSSBC_HD_TESTRESULT pResult);
	virtual int AddCPUMeasureResult		(PSSBC_CPUMEASURE_TESTRESULT pResult);
	virtual int AddMemResult			(PSSBC_MEM_TESTRESULT pResult);
	virtual int AddNetPingResult		(PSSBC_NET_PING_TESTRESULT pResult);
	virtual int AddNetTCPConResult		(PSSBC_NET_TCPCON_TESTRESULT pResult);
	virtual int AddClientHistoryResult	(CSolidSBCClientResultInfo* pResult, SSBC_CLIENT_HISTORY_ACTION nAction, UINT nActionParam);

private:	
	MYSQL *m_conn;
};
