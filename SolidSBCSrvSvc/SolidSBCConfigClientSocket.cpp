// ConfigClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCConfigClientSocket.h"

typedef struct {
	CSolidSBCConfigClientSocket* pSocket;
	SOCKET hClientSocket;
} SolidSBCConfigClientHandlerThreadParam, *PSolidSBCConfigClientHandlerThreadParam;

UINT SolidSBCConfigClientHandlerThread(LPVOID lpParam)
{
	PSolidSBCConfigClientHandlerThreadParam pParams = (PSolidSBCConfigClientHandlerThreadParam)lpParam;

	CString strClientName = _T("");
	int nResponseProfileID = pParams->pSocket->RecieveRequestProfileID(pParams->hClientSocket,&strClientName);
	if ( nResponseProfileID > -1 ){
		{
			USES_CONVERSION;
			SOCKADDR_IN client;
			int nClientSize = sizeof(SOCKADDR_IN);
			ZeroMemory(&client,sizeof(SOCKADDR_IN));
			getpeername(pParams->hClientSocket,(SOCKADDR*)&client,&nClientSize);
			CString strMsg;
			strMsg.Format(_T("Client (%s at %s:%d) requested Profile ID %d")
				, strClientName
				, A2T(inet_ntoa(client.sin_addr))
				, ntohs(client.sin_port)
				, nResponseProfileID);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
		}
		pParams->pSocket->SendReplyProfileID(pParams->hClientSocket,nResponseProfileID);
	} else {
		{
			CString strMsg;
			strMsg.Format(_T("Error while waiting for client profile request."));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
		}
	}

	closesocket(pParams->hClientSocket);
	pParams->hClientSocket = NULL;

	delete pParams;
	pParams = NULL;

	return 0;
}

// CSolidSBCConfigClientSocket
CSolidSBCConfigClientSocket::CSolidSBCConfigClientSocket()
: CSolidSBCServerSocket()
{
}

CSolidSBCConfigClientSocket::~CSolidSBCConfigClientSocket()
{
	Close();
}

bool CSolidSBCConfigClientSocket::OnAccept(SOCKET hCfgCliSocket)
{
	PSolidSBCConfigClientHandlerThreadParam pParams = new SolidSBCConfigClientHandlerThreadParam;
	pParams->hClientSocket = hCfgCliSocket;
	pParams->pSocket       = this;

	AfxBeginThread(SolidSBCConfigClientHandlerThread,pParams);
	return true;
}

int CSolidSBCConfigClientSocket::RecieveRequestProfileID(SOCKET hCfgListenSocket, CString* pstrClientName)
{
	int nReturn = -1;
	int nRead = 0, nTotal = 0;
	int nBufferSize = sizeof(BYTE) * 1024;
	PBYTE pBytes = (PBYTE)malloc( nBufferSize );
	ZeroMemory(pBytes,1024);

	do{
		nRead = recv(hCfgListenSocket,(char*)&(pBytes[nTotal]),nBufferSize,0);
		if (nRead == nBufferSize){
			nTotal += nRead;
			pBytes = (PBYTE)realloc(pBytes, nTotal + nBufferSize );
			ZeroMemory( &pBytes[nTotal], nBufferSize);
		} else if( nRead > 0 ){
			nTotal += nRead; }
	} while (nRead == nBufferSize);

	if ( (nTotal != sizeof(SSBC_PROFILE_REQUEST_PACKET)) ){
		{
			CString strMsg;
			strMsg.Format(_T("ReceiveReplyProfileID: Error while receiving profile request from client (invalid packet size %d bytes)."), nTotal);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_ERROR);
		}
		nReturn = -1; }
	else {
		PSSBC_PROFILE_REQUEST_PACKET pPacket = (PSSBC_PROFILE_REQUEST_PACKET)pBytes;
		nReturn = pPacket->nProfileID; 
		pstrClientName->Format(_T("%s"),pPacket->szClient);
	}

	free(pBytes);
	pBytes = NULL;

	return nReturn;
}

int CSolidSBCConfigClientSocket::SendReplyProfileID(SOCKET hCfgCliSocket, int nProfileID)
{
	SSBC_PROFILE_REPLY_PACKET packet = g_cClientList.GetProfileReplyPacket(nProfileID);
	return send(hCfgCliSocket,(char*)&packet,sizeof(SSBC_PROFILE_REPLY_PACKET),0);
}
