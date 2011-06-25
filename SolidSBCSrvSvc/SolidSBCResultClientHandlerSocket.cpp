#include "StdAfx.h"
#include "SolidSBCResultClientHandlerSocket.h"

CMutex g_cNextClientIDMutex;
UINT   g_nNextClientID = 0;

UINT SolidSBCResultClientHandlerWaitForPacketThread(LPVOID lpParam)
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
	g_cClientList.AddResultClient(clientResultInfo);

	//wait for packet till error occurs
	BOOL bLoop = TRUE;
	while ( bLoop )
	{
		if ( pSocket->WaitForPacket() ){
			if ( !pSocket->OnRead(nClientID) ){
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

CSolidSBCResultClientHandlerSocket::CSolidSBCResultClientHandlerSocket(SOCKET hSocket)
{
	m_hSocket = hSocket;
	AfxBeginThread(SolidSBCResultClientHandlerWaitForPacketThread,this);
}

CSolidSBCResultClientHandlerSocket::~CSolidSBCResultClientHandlerSocket(void)
{
}

bool CSolidSBCResultClientHandlerSocket::WaitForPacket(void)
{
	//readable socket means packet recieved, 
	fd_set readfds; FD_ZERO(&readfds); FD_SET(m_hSocket,&readfds);
	
	int nErr = select((int)m_hSocket+1,&readfds,NULL,NULL,NULL);
	if ( ( nErr == 1) && FD_ISSET(m_hSocket,&readfds) ){
		return true;}
	else {
		return false;}
}

bool CSolidSBCResultClientHandlerSocket::OnRead(int nClientID)
{
	bool bReturn = true;
	int nRequiredSize = -1;
	CString strType = _T("error");

	int	  nRead       = 0;
	int   nHeaderSize = sizeof(SSBC_TEST_RESULT_PACKET);
	PBYTE pHeader     = (PBYTE)malloc( nHeaderSize );
	ZeroMemory(pHeader,nHeaderSize);

	//receive till header is there
	nRead = recv(m_hSocket,(char*)pHeader,nHeaderSize,0);
	if (nRead == nHeaderSize){

		//calculating size for complete packet
		switch( ((PSSBC_TEST_RESULT_PACKET)pHeader)->hdr.type )
		{
		case SSBC_CONN_RES_REQUEST_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_CONN_RES_REQUEST_PACKET);
			strType = _T("SSBC_CONN_RES_REQUEST_PACKET_TYPE");
			break;
		case SSBC_PROFILE_CHANGE_REQUEST_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET);
			strType = _T("SSBC_PROFILE_CHANGE_REQUEST_PACKET_TYPE");
			break;
		case SSBC_HD_TESTRESULT_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_HD_TESTRESULT_PACKET);
			strType = _T("SSBC_HD_TESTRESULT_PACKET_TYPE");
			break;
		case SSBC_CPUMEASURE_TESTRESULT_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_CPUMEASURE_TESTRESULT_PACKET);
			strType = _T("SSBC_CPUMEASURE_TESTRESULT_PACKET_TYPE");
			break;
		case SSBC_MEMORY_TESTRESULT_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_MEMORY_TESTRESULT_PACKET);
			strType = _T("SSBC_MEMORY_TESTRESULT_PACKET_TYPE");
			break;
		case SSBC_NETWORK_PING_TESTRESULT_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_NETWORK_PING_TESTRESULT_PACKET);
			strType = _T("SSBC_NETWORK_PING_TESTRESULT_PACKET_TYPE");
			break;
		case SSBC_NETWORK_TCPCON_TESTRESULT_PACKET_TYPE:
			nRequiredSize = sizeof(SSBC_NETWORK_TCPCON_TESTRESULT_PACKET);
			strType = _T("SSBC_NETWORK_TCPCON_TESTRESULT_PACKET_TYPE");
			break;
		default:
			bReturn = false;
			break;
		}
		
		//request space for complete packet
		int nPacketSize = nHeaderSize + nRequiredSize;
		PBYTE pPacket = new BYTE[nPacketSize];
		ZeroMemory(pPacket,nPacketSize);
		memcpy(pPacket,pHeader,nHeaderSize);

		//receive rest of the packet
		nRead = recv(m_hSocket,(char*)(&(pPacket[nHeaderSize])),nRequiredSize,0);

		if ( nRead == nRequiredSize ){
			//packet received
			//everything is ok with the packet, do appropriate actions...
			switch( ((PSSBC_TEST_RESULT_PACKET)pHeader)->hdr.type )
			{
			case SSBC_CONN_RES_REQUEST_PACKET_TYPE:
				ReceiveResultConnRequest( (PSSBC_CONN_RES_REQUEST_PACKET) (&(pPacket[nHeaderSize])), nClientID );
				break;
			case SSBC_HD_TESTRESULT_PACKET_TYPE:
				ReceiveHDResultPacket( (PSSBC_HD_TESTRESULT_PACKET) (&(pPacket[nHeaderSize])), nClientID );
				break;
			case SSBC_CPUMEASURE_TESTRESULT_PACKET_TYPE:
				ReceiveCPUMeasureResultPacket( (PSSBC_CPUMEASURE_TESTRESULT_PACKET) (&(pPacket[nHeaderSize])), nClientID );
				break;
			case SSBC_MEMORY_TESTRESULT_PACKET_TYPE:
				ReceiveMemResultPacket( (PSSBC_MEMORY_TESTRESULT_PACKET) (&(pPacket[nHeaderSize])), nClientID );
				break;
			case SSBC_NETWORK_PING_TESTRESULT_PACKET_TYPE:
				ReceiveNetPingResultPacket( (PSSBC_NETWORK_PING_TESTRESULT_PACKET) (&(pPacket[nHeaderSize])), nClientID );
				break;
			case SSBC_NETWORK_TCPCON_TESTRESULT_PACKET_TYPE:
				ReceiveNetTCPConResultPacket( (PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET) (&(pPacket[nHeaderSize])), nClientID );
				break;
			default:
				bReturn = false;
				break;
			}

		} else {

			//invalid packet size
			bReturn = false;
			{
				CString strMsg;
				strMsg.Format(_T("Invalid packet size. Closing result connection. received %d bytes, required %d bytes, type: %s."),nRead,nRequiredSize,strType);
				CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
			}

		}

		delete [] pPacket;
		pPacket = NULL;

	} else{

		//packet smaller than header ???
		bReturn = false;
		{
			CString strMsg;
			strMsg.Format(_T("Invalid header received. Closing result connection. received %d bytes"),nRead);
			CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_WARN);
		}
	}

#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCResultClientHandlerSocket::OnRead() returns %d. received %d bytes, required %d bytes, type: %s."),bReturn,nRead,nRequiredSize,strType);
		CSolidSBCSrvServiceWnd::LogServiceMessage(strMsg,SSBC_SRVSVC_LOGMSG_TYPE_DEBUG);
	}
#endif

	free(pHeader);
	pHeader = NULL;

	return bReturn;
}

int CSolidSBCResultClientHandlerSocket::ReceiveResultConnRequest( PSSBC_CONN_RES_REQUEST_PACKET pPacket, int nClientID )
{
	//TODO: this should not be in packet, see client for more info
	struct sockaddr_in client;
	int nClientSize = sizeof(client);
	BOOL bErr = getpeername( m_hSocket, (SOCKADDR*)&client, &nClientSize );
	pPacket->client = client;

	g_cClientList.AddResultRequest(*pPacket);

	return 0;
}

void CSolidSBCResultClientHandlerSocket::SendProfileChangeRequest(UINT nNewProfileID)
{
	//sending profile change request to client
	SSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET packet;
	packet.hdr.type = SSBC_PROFILE_CHANGE_REQUEST_PACKET_TYPE;
	packet.nChangeToProfileID = nNewProfileID;

	send(m_hSocket,(char*)&packet,sizeof(SSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET),0);
}

void CSolidSBCResultClientHandlerSocket::Close(void)
{
	if (m_hSocket)
		closesocket(m_hSocket);

	m_hSocket = NULL;
}

int CSolidSBCResultClientHandlerSocket::ReceiveHDResultPacket(PSSBC_HD_TESTRESULT_PACKET pPacket, int nClientID)
{
	SSBC_HD_TESTRESULT result;
	result.hdResult  = *pPacket;
	result.nResultID = 0;
	result.nClientID = nClientID;

	g_cClientList.AddHDResult(&result);
	return 0;
}

int CSolidSBCResultClientHandlerSocket::ReceiveCPUMeasureResultPacket(PSSBC_CPUMEASURE_TESTRESULT_PACKET pPacket, int nClientID)
{
	SSBC_CPUMEASURE_TESTRESULT result;
	result.cpuMeasure = *pPacket;
	result.nResultID  = 0;
	result.nClientID  = nClientID;

	g_cClientList.AddCPUMeasureResult(&result);
	return 0;
}

int CSolidSBCResultClientHandlerSocket::ReceiveMemResultPacket(PSSBC_MEMORY_TESTRESULT_PACKET pPacket, int nClientID)
{
	SSBC_MEM_TESTRESULT result;
	result.memResult  = *pPacket;
	result.nResultID  = 0;
	result.nClientID  = nClientID;

	g_cClientList.AddMemResult(&result);
	return 0;
}

int CSolidSBCResultClientHandlerSocket::ReceiveNetPingResultPacket(PSSBC_NETWORK_PING_TESTRESULT_PACKET pPacket, int nClientID)
{
	SSBC_NET_PING_TESTRESULT result;
	result.netPingResult = *pPacket;
	result.nResultID     = 0;
	result.nClientID     = nClientID;

	g_cClientList.AddNetPingResult(&result);
	return 0;
}

int CSolidSBCResultClientHandlerSocket::ReceiveNetTCPConResultPacket(PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET pPacket, int nClientID)
{
	SSBC_NET_TCPCON_TESTRESULT result;
	result.netTCPConResult = *pPacket;
	result.nResultID       = 0;
	result.nClientID       = nClientID;

	g_cClientList.AddNetTCPConResult(&result);
	return 0;
}
