#include "StdAfx.h"
#include "SolidSBCNetLib.h"

UINT SolidSBCClientSocket_ConnectThread(LPVOID pParam)
{
	CSolidSBCClientSocket* pSocketClass = (CSolidSBCClientSocket*)pParam;
	bool bSuccess = pSocketClass->WaitForConnect();
	pSocketClass->OnConnect( bSuccess );
	return 0;
}

UINT SolidSBCClientSocket_WaitForPacketThread(LPVOID pParam)
{
	CSolidSBCClientSocket* pSocketClass = (CSolidSBCClientSocket*)pParam;
	if ( pSocketClass->WaitForPacket() )
		pSocketClass->OnRead();
	else
		pSocketClass->Close();

	return 0;
}

CSolidSBCClientSocket::CSolidSBCClientSocket(void)
: m_hSocket(INVALID_SOCKET)
, m_nConnectTimeout(5000)
{
}

CSolidSBCClientSocket::~CSolidSBCClientSocket(void)
{
	Close();
}

void CSolidSBCClientSocket::Connect(const struct sockaddr_in& target, const int& nTimeoutMS)
{
	if (m_hSocket != INVALID_SOCKET)
		Close();

	if ( (m_hSocket = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET )
		OnConnect(false);

	//set socket to non blocking...
	u_long iMode = 1;
	int   nError = 0;
	if ( (nError = ioctlsocket(m_hSocket, FIONBIO, &iMode)) == SOCKET_ERROR )
		OnConnect(false);

	int nConnectErr = connect(m_hSocket,(SOCKADDR*)&target,sizeof(SOCKADDR_IN));
	int nConnectGetLastError = WSAGetLastError();
	
	//set socket to blocking mode...
	iMode  = 0;
	nError = 0;
	if ( (nError = ioctlsocket(m_hSocket, FIONBIO, &iMode)) == SOCKET_ERROR )
		OnConnect(false);

	//check if connect was successful and act accordingly
	if ( (nConnectErr == SOCKET_ERROR) && (nConnectGetLastError == WSAEWOULDBLOCK ) ){
		AfxBeginThread(SolidSBCClientSocket_ConnectThread,(LPVOID)this);
	} else if ( !nConnectErr ) {
		OnConnect(true);
	}
	else {
		OnConnect(false);
	}
}

void CSolidSBCClientSocket::Close(void)
{
	if(m_hSocket != INVALID_SOCKET)
		closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}

void CSolidSBCClientSocket::GetNextPacket(void)
{
	AfxBeginThread(SolidSBCClientSocket_WaitForPacketThread,(LPVOID)this);
}

bool CSolidSBCClientSocket::WaitForConnect()
{
	//writeable socket means successful connect()
	HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	int nError = 0;
	if ( (nError = WSAEventSelect(m_hSocket, hEvent, FD_WRITE|FD_CLOSE)) == SOCKET_ERROR )
		return false;

	//wait if we get signaled...
	DWORD dwTimeoutMS = m_nConnectTimeout;
	DWORD dwWait = WaitForSingleObject(hEvent,dwTimeoutMS);
	CloseHandle(hEvent);
	switch(dwWait)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	default:
		return false;
	}
}

bool CSolidSBCClientSocket::WaitForPacket()
{
	//readable socket means packet recieved
	HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	int nError = 0;
	if ( (nError = WSAEventSelect(m_hSocket, hEvent, FD_READ|FD_CLOSE)) == SOCKET_ERROR )
	{
		return false;
	}
	
	//wait if we get signaled...
	DWORD dwTimeoutMS = INFINITE;
	DWORD dwWait = WaitForSingleObject(hEvent,dwTimeoutMS);
	CloseHandle(hEvent);
	switch(dwWait)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_TIMEOUT:
		return false;
	default:
		return false;
	}
}