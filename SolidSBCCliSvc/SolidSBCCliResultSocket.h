#pragma once

class CSolidSBCCliResultSocket : public CSolidSBCClientSocket
{
public:
	CSolidSBCCliResultSocket();
	~CSolidSBCCliResultSocket();

	virtual bool OnConnect(bool bSuccess);
	virtual bool OnRead();

	int SendPacket(PSSBC_BASE_PACKET_HEADER pPacket);

	int		SendResultConnRequest	(void);
	void    SetProfileID            (UINT nProfileID) { m_nProfileID = nProfileID;   };
	void	SetClientUUID			(char* pszClientUUID){m_pszClientUUID = pszClientUUID;};

private:
	int ReceiveChangeProfileRequest(PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET pPacket, int nSize);
	UINT   m_nProfileID;
	char*  m_pszClientUUID;
};
