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
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::Connect(): socket() created: %d"),m_hCliConfSocket);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	//set socket to non blocking...
	u_long iMode = 1;
	int   nError = 0;
	if ( (nError = ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode)) == SOCKET_ERROR )
	{
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): ioctlsocket(non-block) returned %d, GetLastError() = %d, socket = %d "),nError,WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
	}

	int nErr = connect(m_hCliConfSocket,(SOCKADDR*)&target,sizeof(SOCKADDR_IN));
	int nConnectGetLastError = WSAGetLastError();
	
	//set socket to blocking mode...
	iMode  = 0;
	nError = 0;
	if ( (nError = ioctlsocket(m_hCliConfSocket, FIONBIO, &iMode)) == SOCKET_ERROR )
	{
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): ioctlsocket(block) returned %d, GetLastError() = %d, socket = %d "),nError,nConnectGetLastError,m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
	}

	//check if connect was successful and act accordingly
	if ( (nErr == SOCKET_ERROR) && (nConnectGetLastError == WSAEWOULDBLOCK ) ){
		AfxBeginThread(SolidSBCCliConfigConnectThread,(LPVOID)this);
		return true;
	} else if ( !nErr )
		return true;
	else {
		
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): connect() returned %d, GetLastError() = %d, socket = %d "),nErr,nConnectGetLastError,m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	}
}

bool CSolidSBCCliConfigSocket::WaitForConnect()
{
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect() waiting for connection..."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

	//writeable socket means successful connect()
	HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	int nError = 0;
	if ( (nError = WSAEventSelect(m_hCliConfSocket, hEvent, FD_WRITE)) == SOCKET_ERROR )
	{
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): WSAEventSelect() returned %d, GetLastError() = %d, socket = %d "),nError,WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
			return false;
		}
	}

	//wait if we get signaled...
	DWORD dwTimeoutMS = 10000;
	DWORD dwWait = WaitForSingleObject(hEvent,dwTimeoutMS);
	CloseHandle(hEvent);
	switch(dwWait)
	{
	case WAIT_OBJECT_0:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): returns 1, socket = %d "),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return true;
	case WAIT_TIMEOUT:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): WaitForSingleObject() timed out, GetLastError() = %d, socket = %d "),WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	default:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForConnect(): WaitForSingleObject() returned %d, GetLastError() = %d, socket = %d "),dwWait,WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	}
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
	//readable socket means packet recieved
	HANDLE hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	int nError = 0;
	if ( (nError = WSAEventSelect(m_hCliConfSocket, hEvent, FD_READ)) == SOCKET_ERROR )
	{
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForPacket(): WSAEventSelect() returned %d, GetLastError() = %d, socket = %d "),nError,WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
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
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForPacket(): returns 1, socket = %d "),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return true;
	case WAIT_TIMEOUT:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForPacket(): WaitForSingleObject() timed out, GetLastError() = %d, socket = %d "),WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	default:
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::WaitForPacket(): WaitForSingleObject() returned %d, GetLastError() = %d, socket = %d "),dwWait,WSAGetLastError(),m_hCliConfSocket);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
		return false;
	}
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
	{
		CString strMsg;
		strMsg.Format( _T("SendRequestProfileID: Sending profile request packet") );
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}

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
