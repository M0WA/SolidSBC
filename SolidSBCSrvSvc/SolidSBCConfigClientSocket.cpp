// ConfigClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCConfigClientSocket.h"

typedef struct {
	CSolidSBCConfigClientSocket* pSocket;
	SOCKET hClientSocket;
} SSBC_CONFIG_CLIENTHANDLER_THREAD_PARAM, *PSSBC_CONFIG_CLIENTHANDLER_THREAD_PARAM;

// CSolidSBCConfigClientSocket
CSolidSBCConfigClientSocket::CSolidSBCConfigClientSocket()
: CSolidSBCSocketServer()
{
}

CSolidSBCConfigClientSocket::~CSolidSBCConfigClientSocket()
{
	Close();
}

bool CSolidSBCConfigClientSocket::OnAccept(SOCKET hCfgCliSocket)
{
	PSSBC_CONFIG_CLIENTHANDLER_THREAD_PARAM pParams = new SSBC_CONFIG_CLIENTHANDLER_THREAD_PARAM;
	pParams->hClientSocket = hCfgCliSocket;
	pParams->pSocket       = this;

	AfxBeginThread(ConfigClientHandlerThread,pParams);
	return true;
}

CSolidSBCPacketConfigRequest* CSolidSBCConfigClientSocket::ReceiveConfigRequest(SOCKET hCfgListenSocket)
{
	int nHeaderSize = sizeof(SSBC_PACKET_HEADER);
	SSBC_PACKET_HEADER header;
	memset(&header,0,nHeaderSize);
	
	u_long iMode = 0;
	ioctlsocket(hCfgListenSocket, FIONBIO, &iMode);

	int nRead = recv(hCfgListenSocket,(char*)&header,nHeaderSize,0);
	DWORD dwError = GetLastError();
	if ( nRead != nHeaderSize )
		return NULL;
	
	int nPayloadSize = header.nPacketSize - nHeaderSize;
	int nPacketSize  = header.nPacketSize+10;
	PBYTE pPacket    = new BYTE[nPacketSize];
	memset(pPacket,0,nPacketSize);
	memcpy(pPacket,&header,nHeaderSize);
	nRead = recv(hCfgListenSocket,(char*)&pPacket[nHeaderSize],nPayloadSize,0);
	if ( nRead != nPayloadSize ){
		delete [] pPacket;
		return NULL;}

	iMode = 1;
	ioctlsocket(hCfgListenSocket, FIONBIO, &iMode);
	
	CSolidSBCPacketConfigRequest* pRequest = NULL;
		
	CStringW strwPayload = (wchar_t*)(&pPacket[nHeaderSize]);
	if( (strwPayload.Find(L"<ConfigRequest>") != -1) && (strwPayload.Find(L"</ConfigRequest>") != -1) )
		pRequest = new CSolidSBCPacketConfigRequest(pPacket);
	
	delete [] pPacket;
	return pRequest;
}

int CSolidSBCConfigClientSocket::SendConfigResponses(SOCKET hCfgCliSocket, CSolidSBCPacketConfigRequest* pRequest)
{
	CString strClientUUID;
	pRequest->GetClientUUID(strClientUUID);

	std::vector<CString> vecXmlConfigs;
	g_cClientList.GetConfigsForClient(strClientUUID,vecXmlConfigs);

	int nBytesSent = 0;
	std::vector<CString>::iterator iXmlConfig;
	for( iXmlConfig = vecXmlConfigs.begin(); iXmlConfig != vecXmlConfigs.end(); iXmlConfig++ )
	{
		CSolidSBCPacketConfigResponse response( *iXmlConfig );

		int nPacketSize = 0;
		PBYTE pPacket = response.GetPacketBytes(nPacketSize);
		
		nBytesSent += send(hCfgCliSocket,(char*)pPacket,nPacketSize,0);
	}
	return nBytesSent;
}

UINT CSolidSBCConfigClientSocket::ConfigClientHandlerThread(LPVOID lpParam)
{
	PSSBC_CONFIG_CLIENTHANDLER_THREAD_PARAM pParams = (PSSBC_CONFIG_CLIENTHANDLER_THREAD_PARAM)lpParam;
	
	CSolidSBCPacketConfigRequest* pRequest = pParams->pSocket->ReceiveConfigRequest(pParams->hClientSocket);
	if ( pRequest ){
		{
			USES_CONVERSION;
			SOCKADDR_IN client;
			int nClientSize = sizeof(SOCKADDR_IN);
			ZeroMemory(&client,sizeof(SOCKADDR_IN));
			getpeername(pParams->hClientSocket,(SOCKADDR*)&client,&nClientSize);

			CString sClientName;
			pRequest->GetClientName(sClientName);
			CString strMsg;
			strMsg.Format(_T("Client (%s at %s:%d) requested config.")
				, sClientName
				, A2T(inet_ntoa(client.sin_addr))
				, ntohs(client.sin_port)
				);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
		}

		pParams->pSocket->SendConfigResponses(pParams->hClientSocket,pRequest);
		delete pRequest;
	} else {
		{
			CString strMsg;
			strMsg.Format(_T("Error while waiting for client config request."));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
		}
	}

	closesocket(pParams->hClientSocket);
	pParams->hClientSocket = NULL;
		
	delete pParams;
	pParams = NULL;
	return 0;
}