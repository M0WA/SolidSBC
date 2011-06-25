// ServerResultSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliResultSocket.h"

UINT SolidSBCCliResultConnectThread(LPVOID pParam)
{
	CSolidSBCCliResultSocket* pSocketClass = (CSolidSBCCliResultSocket*)pParam;
	bool bSuccess = pSocketClass->WaitForConnect();
	pSocketClass->OnConnect( bSuccess );

	return 0;
}

UINT SolidSBCCliResultWaitForPacketThread(LPVOID pParam)
{
	CSolidSBCCliResultSocket* pSocketClass = (CSolidSBCCliResultSocket*)pParam;
	if ( pSocketClass->WaitForPacket() )
		pSocketClass->OnRead();		

	return 0;
}

CSolidSBCCliResultSocket::CSolidSBCCliResultSocket()
: m_hCliResSocket(NULL)
{
}

CSolidSBCCliResultSocket::~CSolidSBCCliResultSocket()
{
	Close(false);
}

bool CSolidSBCCliResultSocket::Connect(SOCKADDR_IN target)
{
	if (m_hCliResSocket)
		Close();

	m_hCliResSocket = socket(AF_INET,SOCK_STREAM,0);

	u_long iMode = 1;
	ioctlsocket(m_hCliResSocket, FIONBIO, &iMode);

	int nErr = connect(m_hCliResSocket,(SOCKADDR*)&target,sizeof(SOCKADDR_IN));
	if ( (nErr == SOCKET_ERROR) && (WSAGetLastError() == WSAEWOULDBLOCK ) ){
		AfxBeginThread(SolidSBCCliResultConnectThread,(LPVOID)this);
		return true;
	} else 
		return false;
}

bool CSolidSBCCliResultSocket::WaitForConnect()
{
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::WaitForConnect() waiting for connection..."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	//writeable socket means successful connect()
	fd_set writefds; FD_ZERO(&writefds); FD_SET(m_hCliResSocket,&writefds);
	fd_set exceptfds; FD_ZERO(&exceptfds); FD_SET(m_hCliResSocket,&exceptfds);
	
	
	u_long iMode = 1;
	ioctlsocket(m_hCliResSocket, FIONBIO, &iMode);

	bool bReturn = true;
	struct timeval tv; 
	tv.tv_sec = 10; 
    tv.tv_usec = 0; 

	int nErr = select((int)m_hCliResSocket+1,NULL,&writefds,NULL,&tv);
	if ( FD_ISSET(m_hCliResSocket,&writefds) ){
	}else {
		bReturn = false;
	}

	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::WaitForConnect() returns %d"),bReturn);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	iMode = 0;
	ioctlsocket(m_hCliResSocket, FIONBIO, &iMode);

	return bReturn;
}

bool CSolidSBCCliResultSocket::OnConnect(bool bSuccess)
{
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::OnConnect(bool bSuccess = %d)"), bSuccess);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	if (bSuccess){
		SendResultConnRequest();
		AfxBeginThread(SolidSBCCliResultWaitForPacketThread,(LPVOID)this);
	} else{
		{
			CString strMsg;
			strMsg.Format(_T("Could not connect to result server."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);

			g_cClientService.ConnectionClosed();
		}
	}
	return true;
}

bool CSolidSBCCliResultSocket::WaitForPacket()
{
	//readable socket means packet recieved
	fd_set readfds; FD_ZERO(&readfds); FD_SET(m_hCliResSocket,&readfds);
	
	int nErr = select((int)m_hCliResSocket+1,&readfds,NULL,NULL,NULL);

	//check if socket is in set
	if ( ( nErr == 1) && FD_ISSET(m_hCliResSocket,&readfds) ){
		return true;}
	else {
		g_cClientService.ConnectionClosed();
		return false;}
}

bool CSolidSBCCliResultSocket::OnRead()
{
	int nRead = 0, nTotal = 0;
	int nBufferSize = sizeof(BYTE) * 1024;
	PBYTE pBytes = (PBYTE)malloc( nBufferSize );
	ZeroMemory(pBytes,1024);

	do{
		//read from socket
		nRead = recv(m_hCliResSocket,(char*)&(pBytes[nTotal]),nBufferSize,0);

		//read even more
		if (nRead == nBufferSize){
			nTotal += nRead;
			pBytes = (PBYTE)realloc(pBytes, nTotal + nBufferSize );
			ZeroMemory( &pBytes[nTotal], nBufferSize);
		} else if( nRead > 0 ){
			nTotal += nRead; }
	} while (nRead == nBufferSize);

	bool bReturn = true;
	if (nTotal > 0){
		PSSBC_BASE_PACKET_HEADER pHdr = (PSSBC_BASE_PACKET_HEADER)pBytes;
		switch (pHdr->type)
		{
		case SSBC_PROFILE_CHANGE_REQUEST_PACKET_TYPE:
			ReceiveChangeProfileRequest((PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET)pBytes,nTotal);
			break;
		default:
			bReturn = false;
			g_cClientService.ConnectionClosed();
			break;
		}
	} else {
		bReturn = false;
		g_cClientService.ConnectionClosed();
	}
	free(pBytes);
	pBytes = NULL;

	return bReturn;
}

int CSolidSBCCliResultSocket::ReceiveChangeProfileRequest(PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET pPacket, int nSize)
{	
	g_cClientService.ChangeProfile(pPacket);
	return 0;
}

bool CSolidSBCCliResultSocket::Close(bool bLog)
{
	if (bLog){
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::Close()"));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	if (m_hCliResSocket){
		closesocket(m_hCliResSocket);}

	m_hCliResSocket = NULL;

	return true;
}

int CSolidSBCCliResultSocket::SendResultConnRequest(void)
{
	USES_CONVERSION;
	DWORD nNameSize = 1024;
	TCHAR szComputerName[1024] = {0};
	GetComputerName(szComputerName,&nNameSize);

	int nNewSize = sizeof(SSBC_TEST_RESULT_PACKET) + sizeof(SSBC_CONN_RES_REQUEST_PACKET);
	PBYTE pSendPacket = new BYTE[nNewSize];
	ZeroMemory(pSendPacket,nNewSize);

	PSSBC_TEST_RESULT_PACKET pPacketHeader = (PSSBC_TEST_RESULT_PACKET)pSendPacket;
	pPacketHeader->hdr.type = SSBC_CONN_RES_REQUEST_PACKET_TYPE;
	pPacketHeader->nParamSize = sizeof(SSBC_CONN_RES_REQUEST_PACKET);

	PSSBC_CONN_RES_REQUEST_PACKET pPacket = (PSSBC_CONN_RES_REQUEST_PACKET)(pSendPacket+sizeof(SSBC_TEST_RESULT_PACKET));
	pPacket->hdr.type = SSBC_CONN_RES_REQUEST_PACKET_TYPE;
	pPacket->nProfileID = m_nProfileID;
	_stprintf_s(pPacket->szClientName, SSBC_PROFILE_MAX_CLIENT_NAME, szComputerName);
	_stprintf_s(pPacket->szClientUUID, SSBC_CLIENTUUID_SIZE        , A2T(m_pszClientUUID) );

	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::SendResultConnRequest(): sending result connection request"));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	//TODO: packet.client (sockaddr_in) is filled out and used and needed on server side only, 
	//      make this stuff less dirty here, unneeded bytes go over the wire...but its just an once
	//		per session packet
	return send( m_hCliResSocket,(char*)pSendPacket, nNewSize,0 );
}

int CSolidSBCCliResultSocket::SendHdTestResult(PSSBC_HD_TESTRESULT_PACKET pPacket)
{
	int nNewSize = sizeof(SSBC_TEST_RESULT_PACKET) + sizeof(SSBC_HD_TESTRESULT_PACKET);
	PBYTE pSendPacket = new BYTE[nNewSize];
	ZeroMemory(pSendPacket,nNewSize);

	SSBC_TEST_RESULT_PACKET resPacket;
	resPacket.hdr.type   = SSBC_HD_TESTRESULT_PACKET_TYPE;
	resPacket.nParamSize = sizeof(SSBC_HD_TESTRESULT_PACKET);
	resPacket.pParam     = NULL;
	memcpy( pSendPacket, &resPacket, sizeof(SSBC_TEST_RESULT_PACKET) );
	memcpy( &pSendPacket[sizeof(SSBC_TEST_RESULT_PACKET)], pPacket, sizeof(SSBC_HD_TESTRESULT_PACKET) );

	send( m_hCliResSocket,(char*)pSendPacket, nNewSize,0 );

	delete pSendPacket;
	pSendPacket = NULL;
	return 0;
}

int CSolidSBCCliResultSocket::SendCpuMeasureTestResult(PSSBC_CPUMEASURE_TESTRESULT_PACKET pResult)
{
	int nNewSize = sizeof(SSBC_TEST_RESULT_PACKET) + sizeof(SSBC_CPUMEASURE_TESTRESULT_PACKET);
	PBYTE pSendPacket = new BYTE[nNewSize];
	ZeroMemory(pSendPacket,nNewSize);

	SSBC_TEST_RESULT_PACKET resPacket;
	resPacket.hdr.type   = SSBC_CPUMEASURE_TESTRESULT_PACKET_TYPE;
	resPacket.nParamSize = sizeof(SSBC_CPUMEASURE_TESTRESULT_PACKET);
	resPacket.pParam     = NULL;
	memcpy( pSendPacket, &resPacket, sizeof(SSBC_TEST_RESULT_PACKET) );
	memcpy( &pSendPacket[sizeof(SSBC_TEST_RESULT_PACKET)], pResult, sizeof(SSBC_CPUMEASURE_TESTRESULT_PACKET) );

	send( m_hCliResSocket,(char*)pSendPacket, nNewSize,0 );

	delete pSendPacket;
	pSendPacket = NULL;
	return 0;
}

int CSolidSBCCliResultSocket::SendMemTestResult(PSSBC_MEMORY_TESTRESULT_PACKET pResult)
{
	int nNewSize = sizeof(SSBC_TEST_RESULT_PACKET) + sizeof(SSBC_MEMORY_TESTRESULT_PACKET);
	PBYTE pSendPacket = new BYTE[nNewSize];
	ZeroMemory(pSendPacket,nNewSize);

	SSBC_TEST_RESULT_PACKET resPacket;
	resPacket.hdr.type   = SSBC_MEMORY_TESTRESULT_PACKET_TYPE;
	resPacket.nParamSize = sizeof(SSBC_MEMORY_TESTRESULT_PACKET);
	resPacket.pParam     = NULL;
	memcpy( pSendPacket, &resPacket, sizeof(SSBC_TEST_RESULT_PACKET) );
	memcpy( &pSendPacket[sizeof(SSBC_TEST_RESULT_PACKET)], pResult, sizeof(SSBC_MEMORY_TESTRESULT_PACKET) );

	send( m_hCliResSocket,(char*)pSendPacket, nNewSize,0 );

	delete pSendPacket;
	pSendPacket = NULL;
	return 0;
}

int CSolidSBCCliResultSocket::SendNetPingTestResult(PSSBC_NETWORK_PING_TESTRESULT_PACKET pResult)
{
	int nNewSize = sizeof(SSBC_TEST_RESULT_PACKET) + sizeof(SSBC_NETWORK_PING_TESTRESULT_PACKET);
	PBYTE pSendPacket = new BYTE[nNewSize];
	ZeroMemory(pSendPacket,nNewSize);

	SSBC_TEST_RESULT_PACKET resPacket;
	resPacket.hdr.type   = SSBC_NETWORK_PING_TESTRESULT_PACKET_TYPE;
	resPacket.nParamSize = sizeof(SSBC_NETWORK_PING_TESTRESULT_PACKET);
	resPacket.pParam     = NULL;
	memcpy( pSendPacket, &resPacket, sizeof(SSBC_TEST_RESULT_PACKET) );
	memcpy( &pSendPacket[sizeof(SSBC_TEST_RESULT_PACKET)], pResult, sizeof(SSBC_NETWORK_PING_TESTRESULT_PACKET) );

	send( m_hCliResSocket,(char*)pSendPacket, nNewSize,0 );

	delete pSendPacket;
	pSendPacket = NULL;

	return 0;
}

int CSolidSBCCliResultSocket::SendNetTCPConTestResult(PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET pResult)
{
	int nNewSize = sizeof(SSBC_TEST_RESULT_PACKET) + sizeof(SSBC_NETWORK_TCPCON_TESTRESULT_PACKET);
	PBYTE pSendPacket = new BYTE[nNewSize];
	ZeroMemory(pSendPacket,nNewSize);

	SSBC_TEST_RESULT_PACKET resPacket;
	resPacket.hdr.type   = SSBC_NETWORK_TCPCON_TESTRESULT_PACKET_TYPE;
	resPacket.nParamSize = sizeof(SSBC_NETWORK_TCPCON_TESTRESULT_PACKET);
	resPacket.pParam     = NULL;
	memcpy( pSendPacket, &resPacket, sizeof(SSBC_TEST_RESULT_PACKET) );
	memcpy( &pSendPacket[sizeof(SSBC_TEST_RESULT_PACKET)], pResult, sizeof(SSBC_NETWORK_TCPCON_TESTRESULT_PACKET) );

	send( m_hCliResSocket,(char*)pSendPacket, nNewSize,0 );

	delete pSendPacket;
	pSendPacket = NULL;

	return 0;
}
