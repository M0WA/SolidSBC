#pragma once

#include "SolidSBCNetLib.h"
#include "SolidSBCSocket.h"

// This class is exported from the SolidSBCNetLib.dll
class SOLIDSBCNETLIB_API CSolidSBCSocketServer : public CSolidSBCSocket {
public:
	CSolidSBCSocketServer(void);
	CSolidSBCSocketServer(const unsigned short& nListenPort);
	~CSolidSBCSocketServer(void);

	bool StartListen(const unsigned short& nListenPort);
	bool StartListen(const struct sockaddr_in& server);

	virtual void Close(void);
	
private:
	virtual bool OnAccept(SOCKET hNewClientSock)=0;

	bool WaitForClient(struct sockaddr_in* pClient);
	static UINT AcceptThread(LPVOID lpParam);
};