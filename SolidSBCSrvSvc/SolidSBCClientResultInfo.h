#pragma once

class CSolidSBCClientResultInfo
{
public:
	CSolidSBCClientResultInfo( struct sockaddr_in client, CSolidSBCResultClientHandlerSocket* pResultClientHandlerSock, UINT nClientID );
	~CSolidSBCClientResultInfo(void);

	inline void SetResultClientHandlerSocket( CSolidSBCResultClientHandlerSocket* pResultClientHandlerSock ) { m_pResultClientHandlerSock = pResultClientHandlerSock; };
	inline CSolidSBCResultClientHandlerSocket* GetResultClientHandlerSocket(void) { return m_pResultClientHandlerSock; };

	inline void               SetSockAddr (struct sockaddr_in client) { m_Client = client; };
	inline struct sockaddr_in GetSockAddr (void)                      { return m_Client; };

	inline UINT GetClientID(void) {return m_nClientID;};
	
	inline UINT GetProfileID(void)            {return m_nProfileID;};
	inline void SetProfileID(UINT nProfileID) {m_nProfileID = nProfileID;};
	
	inline CString GetClientUUID(void)                  {return m_strClientUUID;};
	inline void    SetClientUUID(CString strClientUUID) {m_strClientUUID = strClientUUID;};
	
	inline CString GetClientName(void)                  {return m_strClientName;};
	inline void    SetClientName(CString strClientName ){m_strClientName = strClientName;};

private:
	UINT				 m_nClientID;
	CSolidSBCResultClientHandlerSocket* m_pResultClientHandlerSock;
	struct sockaddr_in   m_Client;
	CString              m_strClientName;
	CString				 m_strClientUUID;
	UINT                 m_nProfileID;
};
