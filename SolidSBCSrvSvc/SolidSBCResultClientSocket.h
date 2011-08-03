#pragma once

class CSolidSBCResultClientSocket : public CSolidSBCSocketServer
{
public:
	CSolidSBCResultClientSocket();
	~CSolidSBCResultClientSocket();

	virtual bool OnAccept(SOCKET hResCliSocket);
};
