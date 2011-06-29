#pragma once

class CSolidSBCCliResultSocket : public CSolidSBCClientSocket
{
public:
	CSolidSBCCliResultSocket();
	~CSolidSBCCliResultSocket();

	virtual bool OnConnect(bool bSuccess);
	virtual bool OnRead();

	int		SendResultConnRequest	(void);
	int     SendHdTestResult        (PSSBC_HD_TESTRESULT_PACKET pPacket);
	int     SendMemTestResult       (PSSBC_MEMORY_TESTRESULT_PACKET pResult);
	int     SendCpuMeasureTestResult(PSSBC_CPUMEASURE_TESTRESULT_PACKET pResult);
	int     SendNetPingTestResult   (PSSBC_NETWORK_PING_TESTRESULT_PACKET pResult);
	int     SendNetTCPConTestResult (PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET pResult);

	

	void    SetProfileID            (UINT nProfileID) { m_nProfileID = nProfileID;   };
	void	SetClientUUID			(char* pszClientUUID){m_pszClientUUID = pszClientUUID;};

private:
	int ReceiveChangeProfileRequest(PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET pPacket, int nSize);
	UINT   m_nProfileID;
	char*  m_pszClientUUID;
};
