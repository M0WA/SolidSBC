#include "StdAfx.h"
#include "SolidSBCResultClientHandlerSocket.h"

CMutex g_cNextClientIDMutex;
UINT   g_nNextClientID = 0;

CSolidSBCResultClientHandlerSocket::CSolidSBCResultClientHandlerSocket(SOCKET hSocket)
{
	m_hSocket = hSocket;
	AfxBeginThread(CSolidSBCResultClientHandlerSocket::WaitForPacketThread,this);
}

CSolidSBCResultClientHandlerSocket::~CSolidSBCResultClientHandlerSocket(void)
{
}

bool CSolidSBCResultClientHandlerSocket::WaitForPacket(void)
{
	//readable socket means packet recieved
	HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	int nError = 0;
	if ( (nError = WSAEventSelect(m_hSocket, hEvent, FD_READ|FD_CLOSE)) == SOCKET_ERROR )
	{
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCResultClientHandlerSocket::WaitForPacket(): WSAEventSelect() returned %d, GetLastError() = %d, socket = %d "),nError,WSAGetLastError(),m_hSocket);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
			return false;
		}
	}
	
	//wait if we get signaled...
	DWORD dwTimeoutMS = INFINITE;
	DWORD dwWait = WaitForSingleObject(hEvent,dwTimeoutMS);
	CloseHandle(hEvent);
	switch(dwWait)
	{
	case WAIT_OBJECT_0:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCResultClientHandlerSocket::WaitForPacket(): returns 1, socket = %d "),m_hSocket);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
		return true;
	case WAIT_TIMEOUT:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCResultClientHandlerSocket::WaitForPacket(): WaitForSingleObject() timed out, GetLastError() = %d, socket = %d "),WSAGetLastError(),m_hSocket);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	default:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCResultClientHandlerSocket::WaitForPacket(): WaitForSingleObject() returned %d, GetLastError() = %d, socket = %d "),dwWait,WSAGetLastError(),m_hSocket);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	}
}

bool CSolidSBCResultClientHandlerSocket::OnRead(CSolidSBCClientResultInfo& clientInfo)
{
	int nHeaderSize = sizeof(SSBC_PACKET_HEADER);
	SSBC_PACKET_HEADER header;
	memset(&header,0,nHeaderSize);
	
	u_long iMode = 0;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	//receive header
	int nRead = recv(m_hSocket,(char*)&header,nHeaderSize,0);
	if ( nRead != nHeaderSize )
		return false;

	//allocate memory for packet
	PBYTE pPacket = new BYTE[header.nPacketSize+1];
	ZeroMemory(pPacket,header.nPacketSize+1);
	memcpy(pPacket,&header,header.nPacketSize);

	//receive payload
	int nPayloadSize = header.nPacketSize - nHeaderSize;
	nRead = recv(m_hSocket,(char*)&pPacket[nHeaderSize],nPayloadSize,0);
	if ( nRead != nPayloadSize ){
		delete pPacket;
		return false;}

	iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);
	
	bool bSuccess = false;
	switch(header.nType)
	{
	case SSBC_PACKET_TYPE_RESULT:
		bSuccess = OnTestResultPacket( clientInfo, pPacket );
		break;
	case SSBC_PACKET_TYPE_RESULT_REQUEST:
		bSuccess = OnResultRequestPacket( clientInfo, pPacket );
		break;
	default:
		bSuccess = false;
	}

	delete pPacket;
	return bSuccess;
}

bool CSolidSBCResultClientHandlerSocket::OnResultRequestPacket(CSolidSBCClientResultInfo& clientInfo, PBYTE pPacket)
{
	bool bReturn = false;
	CSolidSBCPacketResultRequest* pRequestPacket = new CSolidSBCPacketResultRequest(pPacket);

	CString strClientUUID, strClientName;
	pRequestPacket->GetClientUUID(strClientUUID);
	pRequestPacket->GetClientName(strClientName);
	delete pRequestPacket;

	clientInfo.SetClientUUID(strClientUUID);
	clientInfo.SetClientName(strClientName);
	
	return !g_cClientList.AddResultClient(clientInfo);
}

bool CSolidSBCResultClientHandlerSocket::OnTestResultPacket(CSolidSBCClientResultInfo& clientInfo, PBYTE pPacket)
{
	CSolidSBCPacketTestResult* pResultPacket = new CSolidSBCPacketTestResult(pPacket);

	CString strResultSQL;
	pResultPacket->GetResultSQL(strResultSQL);
	delete pResultPacket;

	return !g_cClientList.AddTestResult( clientInfo, strResultSQL );
}

void CSolidSBCResultClientHandlerSocket::Close(void)
{
	if (m_hSocket)
		closesocket(m_hSocket);

	m_hSocket = NULL;
}

UINT CSolidSBCResultClientHandlerSocket::WaitForPacketThread(LPVOID lpParam)
{
	g_cNextClientIDMutex.Lock();
	int nClientID = g_nNextClientID;
	g_nNextClientID++;
	g_cNextClientIDMutex.Unlock();

	CSolidSBCResultClientHandlerSocket* pSocket = (CSolidSBCResultClientHandlerSocket*)lpParam;
	
	SOCKADDR_IN client;
	ZeroMemory(&client,sizeof(SOCKADDR_IN));
	pSocket->GetPeerName(&client);

	CSolidSBCClientResultInfo clientResultInfo( client, pSocket, nClientID );
	
	//wait for packet till error occurs
	BOOL bLoop = TRUE;
	while ( bLoop )
	{
		if ( pSocket->WaitForPacket() ){
			if ( !pSocket->OnRead(clientResultInfo) ){
				bLoop = FALSE;
			} else {
#ifdef _DEBUG
				{
					CString strMsg;
					strMsg.Format(_T("SolidSBCResultClientHandlerWaitForPacketThread: packet from client id: %d received..."), nClientID);
					CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
				}
#endif
			}
		}else {
			bLoop = FALSE;
		}
	}

	{
		CString strMsg;
		strMsg.Format(_T("SolidSBCResultClientHandlerWaitForPacketThread: Closing result connection for client id: %d"), nClientID);
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
	pSocket->Close();

	g_cClientList.DeleteResultClient(nClientID);
	return 0;
}