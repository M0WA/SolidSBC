#pragma once

class CSolidSBCResultClientSocket
{
public:
	CSolidSBCResultClientSocket();
	~CSolidSBCResultClientSocket();
	void StartListen(USHORT nListenPort);
	void CloseListenSocket(bool bLog = true);
	void SendProfileChangeRequest(UINT nNewProfileID);
	bool WaitForClient(SOCKADDR_IN* pClient);
	bool OnAccept(SOCKET hResCliSocket);

private:
	SOCKET m_hResListenSocket;
};
