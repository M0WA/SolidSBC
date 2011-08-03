#pragma once

#include "SolidSBCNetLib.h"

// This class is exported from the SolidSBCNetLib.dll
class SOLIDSBCNETLIB_API CSolidSBCSocketClient : public CSolidSBCSocket {

protected:
	CSolidSBCSocketClient(void);
public:
	~CSolidSBCSocketClient(void);

	void Connect(const struct sockaddr_in& target, const int& nTimeoutMS = 5000);
	void GetNextPacket(void);

	virtual void Close(void);

private:
	virtual bool OnConnect(bool bSuccess)=0;
	virtual bool OnRead()=0;

	static UINT ConnectThread(LPVOID pParam);
	bool WaitForConnect();

	static UINT WaitForPacketThread(LPVOID pParam);
	bool WaitForPacket();

	int m_nConnectTimeout;
};