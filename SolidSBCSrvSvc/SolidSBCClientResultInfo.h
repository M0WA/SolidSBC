#pragma once

class CSolidSBCClientResultInfo
{
public:
	CSolidSBCClientResultInfo( struct sockaddr_in client, CSolidSBCResultClientHandlerSocket* pResultClientHandlerSock, UINT nClientID );
	~CSolidSBCClientResultInfo(void);

	inline void SetResultClientHandlerSocket( CSolidSBCResultClientHandlerSocket* pResultClientHandlerSock ) { m_pResultClientHandlerSock = pResultClientHandlerSock; }
	inline CSolidSBCResultClientHandlerSocket* GetResultClientHandlerSocket(void) { return m_pResultClientHandlerSock; }

	inline void               SetSockAddr (const struct sockaddr_in& client)  { m_Client = client; }
	inline struct sockaddr_in GetSockAddr (void) const                        { return m_Client;   }

	inline UINT    GetClientID(void) const {return m_nClientID;}
	
	inline CString GetClientUUID(void) const                   {return m_strClientUUID;}
	inline void    SetClientUUID(const CString& strClientUUID) {m_strClientUUID = strClientUUID;}
	
	inline CString GetClientName(void) const                   {return m_strClientName;}
	inline void    SetClientName(const CString& strClientName ){m_strClientName = strClientName;}

private:
	CSolidSBCResultClientHandlerSocket* m_pResultClientHandlerSock;
	UINT				 m_nClientID;
	struct sockaddr_in   m_Client;
	CString              m_strClientName;
	CString				 m_strClientUUID;
};
