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
	PBYTE pPacket = CSolidSBCPacket::ReceivePacket(hCfgListenSocket);

	if(!pPacket)
	{
#ifdef _DEBUG
		{
			CString strMsg;
			strMsg.Format(_T("Error while receiving config request from unknown client."));
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
#endif
		return 0;
	}

	CSolidSBCPacketConfigRequest* pRequest = new CSolidSBCPacketConfigRequest(pPacket);
	delete [] pPacket;
	return pRequest;
}

int CSolidSBCConfigClientSocket::SendConfigResponses(SOCKET hCfgCliSocket, CSolidSBCPacketConfigRequest* pRequest)
{
	CString strClientUUID;
	pRequest->GetClientUUID(strClientUUID);

	std::vector<CString> vecXmlConfigs;
	g_cClientList.GetConfigsForClient(strClientUUID,vecXmlConfigs);

	{
		CString strClientName;
		pRequest->GetClientName(strClientName);
		CString strMsg;
		strMsg.Format(_T("Sending client %s %d configs."),strClientName,vecXmlConfigs.size());
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_INFO);
	}

	int nBytesSent = 0;
	std::vector<CString>::iterator iXmlConfig;
	for( iXmlConfig = vecXmlConfigs.begin(); iXmlConfig != vecXmlConfigs.end(); iXmlConfig++ )
	{
		CSolidSBCPacketConfigResponse response( *iXmlConfig );

		int nPacketSize = 0;
		PBYTE pPacket = response.GetPacketBytes(nPacketSize);
		
		int nSent = send(hCfgCliSocket,(char*)pPacket,nPacketSize,0);
		nBytesSent += nSent;
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCConfigClientSocket::SendConfigResponses(): Sending client config (%d byte(-s)):\r\n%s"), nSent,*iXmlConfig);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
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

			strMsg.Format(_T("CSolidSBCConfigClientSocket::ConfigClientHandlerThread(): Client config request:\r\n%s"),pRequest->GetXml());
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
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