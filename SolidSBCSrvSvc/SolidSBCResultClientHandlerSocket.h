#pragma once

class CSolidSBCResultClientHandlerSocket
{
public:
	CSolidSBCResultClientHandlerSocket(SOCKET hSocket);
	~CSolidSBCResultClientHandlerSocket(void);

	void SendProfileChangeRequest(UINT nNewProfileID);

	void Close(void);
	bool WaitForPacket(void);
	bool OnRead(int nClientID);

	int GetPeerName(SOCKADDR_IN* pClient){int nClientLen = sizeof(SOCKADDR_IN);return getpeername(m_hSocket,(SOCKADDR*)pClient,&nClientLen);};

private:
	int  ReceiveResultConnRequest     (PSSBC_CONN_RES_REQUEST_PACKET pPacket         , int nClientID);
	int  ReceiveHDResultPacket        (PSSBC_HD_TESTRESULT_PACKET pPacket            , int nClientID);
	int  ReceiveCPUMeasureResultPacket(PSSBC_CPUMEASURE_TESTRESULT_PACKET pPacket    , int nClientID);
	int  ReceiveMemResultPacket       (PSSBC_MEMORY_TESTRESULT_PACKET pPacket        , int nClientID);
	int  ReceiveNetPingResultPacket   (PSSBC_NETWORK_PING_TESTRESULT_PACKET pPacket  , int nClientID);
	int  ReceiveNetTCPConResultPacket (PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET pPacket, int nClientID);

	SOCKET m_hSocket;
};
