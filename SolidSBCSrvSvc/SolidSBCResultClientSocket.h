#pragma once

class CSolidSBCResultClientSocket : public CSolidSBCServerSocket
{
public:
	CSolidSBCResultClientSocket();
	~CSolidSBCResultClientSocket();
	void SendProfileChangeRequest(UINT nNewProfileID);
	virtual bool OnAccept(SOCKET hResCliSocket);
};
