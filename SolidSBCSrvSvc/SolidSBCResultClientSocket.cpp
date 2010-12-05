// ResultClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultClientSocket.h"

UINT SolidSBCResultClientAcceptThread(LPVOID lpParam)
{
	CSolidSBCResultClientSocket* pSocket = (CSolidSBCResultClientSocket*)lpParam;
	SOCKADDR_IN client;
	while ( pSocket->WaitForClient(&client) )
	{
		{
			USES_CONVERSION;
			CString strMsg;
			strMsg.Format(_T("SolidSBCResultClientAcceptThread: client (%s:%d) connected"),A2T(inet_ntoa(client.sin_addr)),ntohs(client.sin_port));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
	}
	return 0;
}

// CSolidSBCResultClientSocket
CSolidSBCResultClientSocket::CSolidSBCResultClientSocket()
: m_hResListenSocket(NULL)
{
}

CSolidSBCResultClientSocket::~CSolidSBCResultClientSocket()
{
}

void CSolidSBCResultClientSocket::StartListen(USHORT nListenPort)
{
	if ( m_hResListenSocket )
		CloseListenSocket();

	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_port   = htons(nListenPort);
	server.sin_addr.s_addr   = INADDR_ANY;

	m_hResListenSocket = socket(AF_INET,SOCK_STREAM,0);
	int nBind   = bind(m_hResListenSocket,(SOCKADDR*)&server,sizeof(SOCKADDR_IN));
	int nListen = listen(m_hResListenSocket,12);

	AfxBeginThread(SolidSBCResultClientAcceptThread,this);
	
	{
		CString strMsg;
		strMsg.Format(_T("Started listening for client result connections on port %d..."),nListenPort);
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO);

		strMsg.Format(_T("CSolidSBCResultClientSocket::StartListen(nListenPort = %d), nBind = %d, nListen = %d"),nListenPort,nListen,nBind);
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
}

void CSolidSBCResultClientSocket::CloseListenSocket(bool bLog)
{
	if (bLog){
		CString strMsg;
		strMsg.Format(_T("CSolidSBCResultClientSocket::CloseListenSocket(): Closing listening socket for result connections..."));
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}

	closesocket(m_hResListenSocket);
	m_hResListenSocket = NULL;
}

bool CSolidSBCResultClientSocket::WaitForClient(SOCKADDR_IN* pClient)
{
	int nClientSize = sizeof(SOCKADDR_IN);
	ZeroMemory(pClient,nClientSize);

	SOCKET hResCliSocket = accept(m_hResListenSocket,(SOCKADDR*)pClient,&nClientSize);
	if ( hResCliSocket == INVALID_SOCKET ){
		{
			CString strMsg;
			strMsg.Format(_T("Error while accepting new client (%d, m_hCfgListenSocket = 0x%X)"),WSAGetLastError(),m_hResListenSocket);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
		}
		CloseListenSocket();
		return false;
	} else {
		OnAccept(hResCliSocket);
		return true;
	}
}

bool CSolidSBCResultClientSocket::OnAccept(SOCKET hResCliSocket)
{
	CSolidSBCResultClientHandlerSocket* pHandlerSocket = new CSolidSBCResultClientHandlerSocket(hResCliSocket);

	int nClientLen = sizeof(SOCKADDR_IN);
	SOCKADDR_IN client;	
	ZeroMemory(&client,nClientLen);
	getpeername(hResCliSocket,(SOCKADDR*)&client,&nClientLen);

	{
		USES_CONVERSION;
		CString strMsg;
		strMsg.Format(_T("CSolidSBCResultClientSocket::OnAccept(): Accepting new client (%s:%d)"),A2T(inet_ntoa(client.sin_addr)),ntohs(client.sin_port));
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
	return true;
}
