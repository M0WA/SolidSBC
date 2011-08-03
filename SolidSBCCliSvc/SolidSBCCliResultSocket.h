#pragma once

class CSolidSBCCliResultSocket : public CSolidSBCSocketClient
{
public:
	CSolidSBCCliResultSocket();
	~CSolidSBCCliResultSocket();

	virtual bool OnConnect(bool bSuccess);
	virtual bool OnRead();

	int		SendResultConnRequest(void);
	int     SendTestResultPacket (CSolidSBCTestResult* pResult);

	void	SetClientUUID(char* pszClientUUID){m_pszClientUUID = pszClientUUID;}

private:
	char*  m_pszClientUUID;
};
