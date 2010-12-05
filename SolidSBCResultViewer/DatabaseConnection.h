#pragma once

class CDatabaseConnection
{
public:
	CDatabaseConnection(void);
	~CDatabaseConnection(void);

	//database connection related
	int Connect(CString strServer, CString strUser, CString strPassword, CString strDatabase, int nPort);
	int Disconnect(void);
	bool IsConnected();

	//get counts
	int GetHDResultCount(CString strClientUUID, int nIdentity = -1);
	int GetCPUMeasureResultCount(CString strClientUUID, int nIdentity = -1);
	int GetMemResultCount(CString strClientUUID, int nIdentity = -1);
	int GetPingResultCount(CString strClientUUID, int nIdentity = -1);
	int GetTCPResultCount(CString strClientUUID, int nIdentity = -1);

	//get results
	std::vector<SSBC_CLIENT_HD_RESULT>		   GetClientHDResults(CString strClientUUID, int nIdentity = -1);
	std::vector<SSBC_CLIENT_MEM_RESULT>        GetClientMemResults(CString strClientUUID, int nIdentity = -1);
	std::vector<SSBC_CLIENT_CPUMEASURE_RESULT> GetClientCPUMeasureResults(CString strClientUUID, int nIdentity = -1);
	std::vector<SSBC_CLIENT_PING_RESULT>       GetClientPingResults(CString strClientUUID, int nIdentity = -1);
	std::vector<SSBC_CLIENT_TCPCON_RESULT>     GetClientTCPConResults(CString strClientUUID, int nIdentity = -1);

	//get other stuff
	int                               GetUniqueClients(std::vector<CString>* pvClientUUIDs);
	std::vector<SSBC_CLIENT_HISTORY>  GetClientHistory(CString strClientUUID);
	std::vector<SSBC_CLIENT_IDENTITY> GetClientIdentities(CString strClientUUID);
	std::vector<CString>              GetClientNames(CString strClientUUID);

	//database connection variables
	CString m_strServer;
	CString m_strUser;
	CString m_strPassword;
	CString m_strDatabase;
	int     m_nPort;

private:
	int     RefreshClientIdentities(void);
	CString GetClientEndTime(CString strClientUUID, int nExtClientID, CString strStartTime);
	
	MYSQL *m_conn;
	CMutex m_cDatabaseLock;
	std::vector<SSBC_CLIENT_IDENTITY> m_vClientIdentities;
};

extern CDatabaseConnection g_cDatabaseConnection;