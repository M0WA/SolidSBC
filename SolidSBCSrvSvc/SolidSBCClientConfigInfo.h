#pragma once

class CSolidSBCClientConfigInfo
{
public:
	CSolidSBCClientConfigInfo(void);
	CSolidSBCClientConfigInfo( struct sockaddr_in client, CSolidSBCConfigClientSocket* pConfigClientSock );
	~CSolidSBCClientConfigInfo(void);

	inline void               SetSockAddr (struct sockaddr_in client) { m_Client = client; };
	inline struct sockaddr_in GetSockAddr (void)                      { return m_Client; };

	inline void                 SetConfigClientSocket( CSolidSBCConfigClientSocket* p_ConfigClientSock ){ m_pConfigClientSock = p_ConfigClientSock; };
	inline CSolidSBCConfigClientSocket* GetConfigClientSocket(void)                                     { return m_pConfigClientSock; };

private:
	struct sockaddr_in   m_Client;
	CSolidSBCConfigClientSocket* m_pConfigClientSock;
};
