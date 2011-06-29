#pragma once

class CSolidSBCCliConfigSocket : public CSolidSBCClientSocket
{
public:
	CSolidSBCCliConfigSocket();
	~CSolidSBCCliConfigSocket();
	virtual bool OnConnect(bool bSuccess);
	virtual bool OnRead();
	void SetProfileID(UINT nProfileID){m_nProfileID = nProfileID;};

private:
	int SendRequestProfileID(void);
	int ReceiveReplyProfileID(void);
	UINT   m_nProfileID;
};


