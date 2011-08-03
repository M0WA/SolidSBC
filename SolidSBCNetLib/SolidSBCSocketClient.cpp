#include "StdAfx.h"
#include "SolidSBCNetLib.h"


CSolidSBCSocketClient::CSolidSBCSocketClient(void)
: CSolidSBCSocket()
, m_nConnectTimeout(5000)
{
}

CSolidSBCSocketClient::~CSolidSBCSocketClient(void)
{
	Close();
}

void CSolidSBCSocketClient::Connect(const struct sockaddr_in& target, const int& nTimeoutMS)
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
		AfxBeginThread(CSolidSBCSocketClient::ConnectThread,(LPVOID)this);
	} else if ( !nConnectErr ) {
		OnConnect(true);
	}
	else {
		OnConnect(false);
	}
}

void CSolidSBCSocketClient::GetNextPacket(void)
{
	AfxBeginThread(CSolidSBCSocketClient::WaitForPacketThread,(LPVOID)this);
}

bool CSolidSBCSocketClient::WaitForConnect()
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

bool CSolidSBCSocketClient::WaitForPacket()
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

void CSolidSBCSocketClient::Close(void)
{
	CSolidSBCSocket::Close();
}

UINT CSolidSBCSocketClient::ConnectThread(LPVOID pParam)
{
	CSolidSBCSocketClient* pSocketClass = (CSolidSBCSocketClient*)pParam;
	bool bSuccess = pSocketClass->WaitForConnect();
	pSocketClass->OnConnect( bSuccess );
	return 0;
}

UINT CSolidSBCSocketClient::WaitForPacketThread(LPVOID pParam)
{
	CSolidSBCSocketClient* pSocketClass = (CSolidSBCSocketClient*)pParam;
	if ( pSocketClass->WaitForPacket() )
		pSocketClass->OnRead();
	else
		pSocketClass->Close();

	return 0;
}