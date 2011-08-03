// ResultClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCResultClientSocket.h"

// CSolidSBCResultClientSocket
CSolidSBCResultClientSocket::CSolidSBCResultClientSocket()
: CSolidSBCSocketServer()
{
}

CSolidSBCResultClientSocket::~CSolidSBCResultClientSocket()
{
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
