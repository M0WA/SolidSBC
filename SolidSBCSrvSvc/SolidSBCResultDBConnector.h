#pragma once

//message lparam
typedef struct {
	ULONG                       nClientID;
	ULONG						nResultID;
	SSBC_HD_TESTRESULT_PACKET hdResult;
} SSBC_HD_TESTRESULT,*PSSBC_HD_TESTRESULT;

typedef struct {
	ULONG  nClientID;
	ULONG  nResultID;
	SSBC_CPUMEASURE_TESTRESULT_PACKET cpuMeasure;
} SSBC_CPUMEASURE_TESTRESULT, *PSSBC_CPUMEASURE_TESTRESULT;

typedef struct {
	ULONG  nClientID;
	ULONG  nResultID;
	SSBC_MEMORY_TESTRESULT_PACKET memResult;
} SSBC_MEM_TESTRESULT, *PSSBC_MEM_TESTRESULT;

typedef struct {
	ULONG  nClientID;
	ULONG  nResultID;
	SSBC_NETWORK_PING_TESTRESULT_PACKET netPingResult;
} SSBC_NET_PING_TESTRESULT, *PSSBC_NET_PING_TESTRESULT;

typedef struct {
	ULONG  nClientID;
	ULONG  nResultID;
	SSBC_NETWORK_TCPCON_TESTRESULT_PACKET netTCPConResult;
} SSBC_NET_TCPCON_TESTRESULT, *PSSBC_NET_TCPCON_TESTRESULT;

//database types
typedef enum SSBC_DB_TYPE {
	SSBC_DB_TYPE_NONE		= 0,
	SSBC_DB_TYPE_MYSQL		= 1
};

//enum for client_history table action types
typedef enum SSBC_CLIENT_HISTORY_ACTION{
	SSBC_CLIENT_ACTION_ADD_RESULT_CLIENT    = 1,
	SSBC_CLIENT_ACTION_DELETE_RESULT_CLIENT = 2,
};

class CSolidSBCResultDBConnector : public CObject
{
public:
	CSolidSBCResultDBConnector(void);
	~CSolidSBCResultDBConnector(void);
	void SetConfig(CString strServer, DWORD nPort, CString strDatabase, CString strUser, CString strPassword);

	//pure virtual functions (need to be implemented by derived child classes)
	virtual int Connect					(void)=0;
	virtual int Disconnect				(void)=0;
	virtual int AddHDResult				(PSSBC_HD_TESTRESULT pResult)=0;
	virtual int AddCPUMeasureResult		(PSSBC_CPUMEASURE_TESTRESULT pResult)=0;
	virtual int AddMemResult			(PSSBC_MEM_TESTRESULT pResult)=0;
	virtual int AddNetPingResult		(PSSBC_NET_PING_TESTRESULT pResult)=0;
	virtual int AddNetTCPConResult		(PSSBC_NET_TCPCON_TESTRESULT pResult)=0;
	virtual int AddClientHistoryResult	(CSolidSBCClientResultInfo* pResult, SSBC_CLIENT_HISTORY_ACTION nAction, UINT nActionParam)=0;

protected:
	int GetHDSQLString				(PSSBC_HD_TESTRESULT			pResult, CStringArray* parSQLCmds);
	int GetCPUMeasureSQLString		(PSSBC_CPUMEASURE_TESTRESULT	pResult, CStringArray* parSQLCmds);
	int GetMemSQLString				(PSSBC_MEM_TESTRESULT			pResult, CStringArray* parSQLCmds);
	int GetNetPingSQLString			(PSSBC_NET_PING_TESTRESULT		pResult, CStringArray* parSQLCmds);
	int GetNetTCPConSQLString		(PSSBC_NET_TCPCON_TESTRESULT	pResult, CStringArray* parSQLCmds);
	int GetClientHistorySQLString	(CSolidSBCClientResultInfo* pResult, SSBC_CLIENT_HISTORY_ACTION nAction, UINT nActionParam, CStringArray* parSQLCmds);

	CMutex			m_DBConnMutex;
	CString			m_strServer;
	DWORD			m_nPort;
	CString			m_strDatabase;
	CString			m_strUser;
	CString			m_strPassword;
	SSBC_DB_TYPE	m_Type;
};