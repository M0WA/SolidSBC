#include "StdAfx.h"
#include "SolidSBCNetLib.h"

CSolidSBCSocketServer::CSolidSBCSocketServer(void)
: CSolidSBCSocket()
{
}

CSolidSBCSocketServer::CSolidSBCSocketServer(const unsigned short& nListenPort)
: CSolidSBCSocket()
{
	StartListen(nListenPort);
}

CSolidSBCSocketServer::~CSolidSBCSocketServer(void)
{
	Close();
}

bool CSolidSBCSocketServer::StartListen(const unsigned short& nListenPort)
{
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port   = htons(nListenPort);
	server.sin_addr.s_addr   = INADDR_ANY;

	return StartListen(server);
}

bool CSolidSBCSocketServer::StartListen(const struct sockaddr_in& server)
{
	Close();

	m_hSocket = socket(AF_INET,SOCK_STREAM,0);
	int nBind   = bind(m_hSocket,(SOCKADDR*)&server,sizeof(SOCKADDR_IN));
	int nListen = listen(m_hSocket,12);

	AfxBeginThread(CSolidSBCSocketServer::AcceptThread,this);
	return false;
}

bool CSolidSBCSocketServer::WaitForClient(struct sockaddr_in* pClient)
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

void CSolidSBCSocketServer::Close(void)
{
	CSolidSBCSocket::Close();
}

UINT CSolidSBCSocketServer::AcceptThread(LPVOID lpParam)
{
	CSolidSBCSocketServer* pSocket = (CSolidSBCSocketServer*)lpParam;
	SOCKADDR_IN client;
	while ( pSocket->WaitForClient(&client) )
	{
	}
	return 0;
}