#pragma once

// CSolidSBCConfigClientSocket command target
class CSolidSBCConfigClientSocket : public CSolidSBCSocketServer
{
public:
	CSolidSBCConfigClientSocket(void);
	~CSolidSBCConfigClientSocket(void);
		
	virtual bool OnAccept(SOCKET hCfgCliSocket);

	CSolidSBCPacketConfigRequest* ReceiveConfigRequest(SOCKET hCfgListenSocket);
	int SendConfigResponses (SOCKET hCfgCliSocket, CSolidSBCPacketConfigRequest* pRequest);

private:
	static UINT ConfigClientHandlerThread(LPVOID lpParam);

	UINT   m_nProfileID;
};
