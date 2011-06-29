#include "StdAfx.h"
#include "SolidSBCNetLib.h"

UINT SolidSBCServerSocketAcceptThread(LPVOID lpParam)
{
	CSolidSBCServerSocket* pSocket = (CSolidSBCServerSocket*)lpParam;
	SOCKADDR_IN client;
	while ( pSocket->WaitForClient(&client) )
	{
	}
	return 0;
}

CSolidSBCServerSocket::CSolidSBCServerSocket(void)
: m_hSocket(INVALID_SOCKET)
{
}

CSolidSBCServerSocket::CSolidSBCServerSocket(const unsigned short& nListenPort)
: m_hSocket(INVALID_SOCKET)
{
	StartListen(nListenPort);
}

CSolidSBCServerSocket::~CSolidSBCServerSocket(void)
{
	Close();
}

bool CSolidSBCServerSocket::StartListen(const unsigned short& nListenPort)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port   = htons(nListenPort);
	server.sin_addr.s_addr   = INADDR_ANY;

	return StartListen(server);
}

bool CSolidSBCServerSocket::StartListen(const struct sockaddr_in& server)
{
	Close();

	m_hSocket = socket(AF_INET,SOCK_STREAM,0);
	int nBind   = bind(m_hSocket,(SOCKADDR*)&server,sizeof(SOCKADDR_IN));
	int nListen = listen(m_hSocket,12);

	AfxBeginThread(SolidSBCServerSocketAcceptThread,this);
	return false;
}

void CSolidSBCServerSocket::Close(void)
{
	if ( m_hSocket != INVALID_SOCKET )
		closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}

bool CSolidSBCServerSocket::WaitForClient(struct sockaddr_in* pClient)
{
	int nClientSize = sizeof(SOCKADDR_IN);
	ZeroMemory(pClient,nClientSize);

	SOCKET hCfgCliSocket = accept(m_hSocket,(SOCKADDR*)pClient,&nClientSize);
	if ( hCfgCliSocket == INVALID_SOCKET ){
		Close();
		return false;
	} else {
		OnAccept(hCfgCliSocket);
		return true;
	}
}