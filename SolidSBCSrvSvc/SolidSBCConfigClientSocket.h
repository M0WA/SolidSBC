#pragma once

// CSolidSBCConfigClientSocket command target
class CSolidSBCConfigClientSocket
{
public:
	CSolidSBCConfigClientSocket(void);
	~CSolidSBCConfigClientSocket(void);

	bool StartListen(USHORT nListenPort = SSBC_CFG_SERVER_PORT);
	bool WaitForClient(SOCKADDR_IN* pClient);
	bool OnAccept(SOCKET hCfgCliSocket);

	bool CloseListenSocket(bool bLog = true);

	int RecieveRequestProfileID(SOCKET hCfgListenSocket, CString* pstrClientName);
	int SendReplyProfileID(SOCKET hCfgCliSocket, int nProfileID);

private:
	SOCKET m_hCfgListenSocket;
	UINT   m_nProfileID;
};
