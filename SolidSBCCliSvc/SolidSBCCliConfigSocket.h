#pragma once

class CSolidSBCCliConfigSocket
{
public:
	CSolidSBCCliConfigSocket();
	~CSolidSBCCliConfigSocket();

	bool Connect(SOCKADDR_IN target);
	bool WaitForConnect();
	bool OnConnect(bool bSuccess);

	bool WaitForPacket();
	bool OnRead();

	bool Close(bool bLog = true);

	void SetProfileID(UINT nProfileID){m_nProfileID = nProfileID;};

private:
	int SendRequestProfileID(void);
	int ReceiveReplyProfileID(void);

	SOCKET m_hCliConfSocket;
	UINT   m_nProfileID;
};

/*
// CServerConfigSocket command target
typedef enum VMPERF_SERVER_CONFIG_SOCKET_STATUS {
	VMPERF_SERVER_CONFIG_SOCKET_STATUS_IDLE = 0,
	VMPERF_SERVER_CONFIG_SOCKET_STATUS_PROFILEID = 1
};

class CServerConfigSocket : public CAsyncSocket
{
public:
	CServerConfigSocket();
	virtual ~CServerConfigSocket();
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void    SetMessageWindow(HWND hMessageWnd){ m_hMessageWnd = hMessageWnd; };
	void    SetProfileID(UINT nProfileID){m_nProfileID = nProfileID;};

private:
	int SendRequestProfileID(void);
	int ReceiveReplyProfileID(void);

	HWND m_hMessageWnd;
	VMPERF_SERVER_CONFIG_SOCKET_STATUS m_nStatus;
	UINT m_nProfileID;
};

*/


