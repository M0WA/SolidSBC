#pragma once

// CSolidSBCConfigClientSocket command target
class CSolidSBCConfigClientSocket : public CSolidSBCServerSocket
{
public:
	CSolidSBCConfigClientSocket(void);
	~CSolidSBCConfigClientSocket(void);
		
	virtual bool OnAccept(SOCKET hCfgCliSocket);

	int RecieveRequestProfileID(SOCKET hCfgListenSocket, CString* pstrClientName);
	int SendReplyProfileID(SOCKET hCfgCliSocket, int nProfileID);

private:	
	UINT   m_nProfileID;
};
