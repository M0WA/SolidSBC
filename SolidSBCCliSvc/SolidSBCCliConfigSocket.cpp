// ServerConfigSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliConfigSocket.h"

UINT SolidSBCCliConfigConnectThread(LPVOID pParam)
{
	CSolidSBCCliConfigSocket* pSocketClass = (CSolidSBCCliConfigSocket*)pParam;
	bool bSuccess = pSocketClass->WaitForConnect();
	pSocketClass->OnConnect( bSuccess );

	return 0;
}

UINT SolidSBCCliConfigWaitForPacketThread(LPVOID pParam)
{
	CSolidSBCCliConfigSocket* pSocketClass = (CSolidSBCCliConfigSocket*)pParam;
	if ( pSocketClass->WaitForPacket() )
		pSocketClass->OnRead();
	else
		pSocketClass->Close();

	return 0;
}

CSolidSBCCliConfigSocket::CSolidSBCCliConfigSocket()
: m_hCliConfSocket(NULL)
, m_nProfileID(0)
{
}

CSolidSBCCliConfigSocket::~CSolidSBCCliConfigSocket()
{
	Close(false);
}

bool CSolidSBCCliConfigSocket::Connect(SOCKADDR_IN target)
{
	if (m_hCliConfSocket)
		Close();

	m_hCliConfSocket = socket(AF_INET,SOCK_STREAM,0);

	u_long iMode = 1;
	ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode);

	int nErr = connect(m_hCliConfSocket,(SOCKADDR*)&target,sizeof(SOCKADDR_IN));
	if ( (nErr == SOCKET_ERROR) && (WSAGetLastError() == WSAEWOULDBLOCK ) ){
		AfxBeginThread(SolidSBCCliConfigConnectThread,(LPVOID)this);
		return true;
	} else 
		return false;
}

bool CSolidSBCCliConfigSocket::WaitForConnect()
{
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect() waiting for connection..."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	//writeable socket means successful connect()
	fd_set writefds; FD_ZERO(&writefds); FD_SET(m_hCliConfSocket,&writefds);
	fd_set exceptfds; FD_ZERO(&exceptfds); FD_SET(m_hCliConfSocket,&exceptfds);
	
	
	u_long iMode = 1;
	ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode);

	bool bReturn = true;
	struct timeval tv; 
	tv.tv_sec = 10; 
    tv.tv_usec = 0; 

	int nErr = select(m_hCliConfSocket+1,NULL,&writefds,NULL,&tv);
	if ( !FD_ISSET(m_hCliConfSocket,&writefds) ){
		bReturn = false;
	} else if ( !FD_ISSET(m_hCliConfSocket,&exceptfds) ){
		bReturn = false;
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): an exception occurred on socket"));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
	}

	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect() returns %d"),bReturn);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	iMode = 0;
	ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode);

	return bReturn;
}

bool CSolidSBCCliConfigSocket::OnConnect(bool bSuccess)
{
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::OnConnect(bool bSuccess = %d)"), bSuccess);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	if (bSuccess){
		SendRequestProfileID();
		AfxBeginThread(SolidSBCCliConfigWaitForPacketThread,(LPVOID)this);
	} else{
		{
			g_cClientService.ConnectionClosed();

			CString strMsg;
			strMsg.Format(_T("Could not connect to profile server."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
	}
	return true;
}

bool CSolidSBCCliConfigSocket::Close(bool bLog)
{
	if (bLog){
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::Close()"));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	if (m_hCliConfSocket){
		closesocket(m_hCliConfSocket);}

	m_hCliConfSocket = NULL;

	return true;
}

bool CSolidSBCCliConfigSocket::WaitForPacket()
{
	//readable socket means packet recieved, 
	fd_set readfds; FD_ZERO(&readfds); FD_SET(m_hCliConfSocket,&readfds);
	
	int nErr = select((int)(m_hCliConfSocket+1),&readfds,NULL,NULL,NULL);
	if ( ( nErr == 1) && FD_ISSET(m_hCliConfSocket,&readfds) ){
		return true;}
	else {
		return false;}
}

bool CSolidSBCCliConfigSocket::OnRead()
{
	if ( !ReceiveReplyProfileID() ){
		{
			CString strMsg;
			strMsg.Format(_T("Fetched profile (id: %d) successfully from server."),m_nProfileID);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}
		return true;}
	else{
		{
			CString strMsg;
			strMsg.Format(_T("Error while fetching profile from server."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		return false;}
}

int CSolidSBCCliConfigSocket::SendRequestProfileID(void)
{
	DWORD nNameSize = 1024;
	TCHAR szComputerName[1024] = {0};
	GetComputerName(szComputerName,&nNameSize);
	
	SSBC_PROFILE_REQUEST_PACKET packet;
	packet.nProfileID = m_nProfileID;
	_stprintf_s(packet.szClient, SSBC_PROFILE_MAX_CLIENT_NAME, szComputerName);

	u_long iMode = 0;
	ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode);

	int nSent = send(m_hCliConfSocket,(char *)&packet,sizeof(packet),0);

	iMode = 1;
	ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode);

	return nSent;
}

int CSolidSBCCliConfigSocket::ReceiveReplyProfileID(void)
{
	int nRead = 0, nTotal = 0;
	int nBufferSize = sizeof(BYTE) * 1024;
	PBYTE pBytes = (PBYTE)malloc( nBufferSize );
	ZeroMemory(pBytes,1024);

	do{
		nRead = recv(m_hCliConfSocket,(char*)&(pBytes[nTotal]),nBufferSize,0);
		if (nRead == nBufferSize){
			nTotal += nRead;
			pBytes = (PBYTE)realloc(pBytes, nTotal + nBufferSize );
			ZeroMemory( &pBytes[nTotal], nBufferSize);
		} else if( nRead > 0 ){
			nTotal += nRead; }
	} while (nRead == nBufferSize);

	int nReturn = 0;
	if ( nTotal != sizeof(SSBC_PROFILE_REPLY_PACKET) ) { 
		//connection closed
		//PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_REQPROFIDERR, (WPARAM)nTotal );
		{
			CString strMsg;
			strMsg.Format( _T("ReceiveReplyProfileID: Error while fetching profile from server (invalid packet size %d bytes)."), nTotal );
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		nReturn = 1;
	}
	else {
		{
			CString strMsg;
			strMsg.Format(_T("ReceiveReplyProfileID: Starting Result Connection"));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}

		g_cClientService.StartResultConnection();
		g_cClientService.StartTestFromProfilePacket((PSSBC_PROFILE_REPLY_PACKET)pBytes);
	}

	free(pBytes);
	pBytes = NULL;

	Close();
	return nReturn;
}

/*

// CServerConfigSocket

CServerConfigSocket::CServerConfigSocket()
: m_hMessageWnd(NULL)
, m_nStatus(VMPERF_SERVER_CONFIG_SOCKET_STATUS_IDLE)
, m_nProfileID(0)
{
}

CServerConfigSocket::~CServerConfigSocket()
{
}


// CServerConfigSocket member functions

void CServerConfigSocket::OnConnect(int nErrorCode)
{
	CSolidSBCCliServiceWnd::LogServiceMessage(_T("CServerConfigSocket::OnConnect()"));

	if ( nErrorCode != 0 ){
		PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_CONNERR, (WPARAM)nErrorCode);
	}		
	else {
		PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_CONN   , (WPARAM)nErrorCode);
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

void CServerConfigSocket::OnReceive(int nErrorCode)
{
	CSolidSBCCliServiceWnd::LogServiceMessage(_T("CServerConfigSocket::OnReceive()"));

	if ( nErrorCode != 0 ){
		PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_RECVERR, (WPARAM)nErrorCode);}	

	switch(m_nStatus)
	{
	case VMPERF_SERVER_CONFIG_SOCKET_STATUS_PROFILEID:
		ReceiveReplyProfileID();
		m_nStatus = VMPERF_SERVER_CONFIG_SOCKET_STATUS_IDLE;
		break;
	default:
		break;
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

void CServerConfigSocket::OnSend(int nErrorCode)
{
	CSolidSBCCliServiceWnd::LogServiceMessage(_T("CServerConfigSocket::OnSend()"));

	if ( nErrorCode != 0 ){
		PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_SENDERR, (WPARAM)nErrorCode);}	

	switch(m_nStatus)
	{
	case VMPERF_SERVER_CONFIG_SOCKET_STATUS_IDLE:
		m_nStatus = VMPERF_SERVER_CONFIG_SOCKET_STATUS_PROFILEID;
		SendRequestProfileID();
		break;
#ifdef _DEBUG
	case VMPERF_SERVER_CONFIG_SOCKET_STATUS_PROFILEID:
		TRACE(_T("CServerConfigSocket::OnSend(): m_nStatus == VMPERF_SERVER_CONFIG_SOCKET_STATUS_PROFILEID (should not happen)\n"));
		break;
#endif
	default:
		break;
	}

	CAsyncSocket::OnSend(nErrorCode);
}

void CServerConfigSocket::OnClose(int nErrorCode)
{
	CSolidSBCCliServiceWnd::LogServiceMessage(_T("CServerConfigSocket::OnClose()"));

	PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_CONNRST, (WPARAM)nErrorCode);
	
	CAsyncSocket::OnClose(nErrorCode);

	m_nStatus = VMPERF_SERVER_CONFIG_SOCKET_STATUS_IDLE;
}

int CServerConfigSocket::SendRequestProfileID(void)
{
	DWORD nNameSize = 1024;
	TCHAR szComputerName[1024] = {0};
	GetComputerName(szComputerName,&nNameSize);
	
	SSBC_PROFILE_REQUEST_PACKET packet;
	packet.nProfileID = m_nProfileID;
	_stprintf_s(packet.szClient, SSBC_PROFILE_MAX_CLIENT_NAME, szComputerName);

	return Send(&packet,sizeof(packet));
}

int CServerConfigSocket::ReceiveReplyProfileID(void)
{
	int nRead = 0, nTotal = 0;
	int nBufferSize = sizeof(BYTE) * 1024;
	PBYTE pBytes = (PBYTE)malloc( nBufferSize );
	ZeroMemory(pBytes,1024);

	do{
		nRead = Receive(&(pBytes[nTotal]),nBufferSize);
		if (nRead == nBufferSize){
			nTotal += nRead;
			pBytes = (PBYTE)realloc(pBytes, nTotal + nBufferSize );
			ZeroMemory( &pBytes[nTotal], nBufferSize);
		} else if( nRead > 0 ){
			nTotal += nRead; }
	} while (nRead == nBufferSize);

	if ( nTotal != sizeof(SSBC_PROFILE_REPLY_PACKET) ) { 
		//connection closed
		PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_REQPROFIDERR, (WPARAM)nTotal );
	}
	else {
		PSSBC_PROFILE_REPLY_PACKET pPacket = new SSBC_PROFILE_REPLY_PACKET;
		(*pPacket) = (*(PSSBC_PROFILE_REPLY_PACKET)(pBytes));

		PostMessage( m_hMessageWnd, WM_SSBC_ERR, (LPARAM)SSBC_ERR_SRVCFG_REQPROFIDSUC, (WPARAM)pPacket );
	}

	free(pBytes);
	pBytes = NULL;

	Close();
	return 0;
}

*/