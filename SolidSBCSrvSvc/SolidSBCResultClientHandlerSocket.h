#pragma once

class CSolidSBCClientResultInfo;
class CSolidSBCResultClientHandlerSocket
{
public:
	CSolidSBCResultClientHandlerSocket(SOCKET hSocket);
	~CSolidSBCResultClientHandlerSocket(void);

	void Close(void);
	bool WaitForPacket(void);
	bool OnRead(CSolidSBCClientResultInfo& clientInfo);

	int GetPeerName(SOCKADDR_IN* pClient){int nClientLen = sizeof(SOCKADDR_IN);return getpeername(m_hSocket,(SOCKADDR*)pClient,&nClientLen);};

private:
	static UINT WaitForPacketThread(LPVOID lpParam);

	bool OnResultRequestPacket(CSolidSBCClientResultInfo& clientInfo, PBYTE pPacket);
	bool OnTestResultPacket(CSolidSBCClientResultInfo& clientInfo, PBYTE pPacket);

	SOCKET m_hSocket;
};
