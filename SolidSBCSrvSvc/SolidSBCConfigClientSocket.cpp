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
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	int nRead = recv(m_hSocket,(char*)&header,nHeaderSize,0);
	if ( nRead != nHeaderSize )
		return NULL;

	int nPayloadSize = header.nPacketSize - nHeaderSize;
	PBYTE pPayload   = new BYTE[nPayloadSize+1];
	memset(pPayload,0,nPayloadSize+1);
	nRead = recv(m_hSocket,(char*)pPayload,nPayloadSize,0);
	if ( nRead != nPayloadSize ){
		delete [] pPayload;
		return NULL;}

	iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);
	
	CStringW strwPayload = (wchar_t*)pPayload;
	CSolidSBCPacketConfigRequest* pRequest = NULL;

	if( (strwPayload.Find(L"<ConfigRequest>") != -1) && (strwPayload.Find(L"</ConfigRequest>") != -1) )
		pRequest = new CSolidSBCPacketConfigRequest(pPayload);
	
	delete [] pPayload;
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