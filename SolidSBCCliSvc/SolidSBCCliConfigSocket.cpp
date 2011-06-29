// ServerConfigSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliConfigSocket.h"

CSolidSBCCliConfigSocket::CSolidSBCCliConfigSocket()
: CSolidSBCClientSocket()
, m_nProfileID(0)
{
}

CSolidSBCCliConfigSocket::~CSolidSBCCliConfigSocket()
{
	Close();
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
		GetNextPacket();
	} else{
		{
			CString strMsg;
			strMsg.Format(_T("Could not connect to profile server."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		g_cClientService.ConnectionClosed();
	}
	return bSuccess;
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
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	int nSent = send(m_hSocket,(char *)&packet,sizeof(packet),0);

	iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	return nSent;
}

int CSolidSBCCliConfigSocket::ReceiveReplyProfileID(void)
{
	int nRead = 0, nTotal = 0;
	int nBufferSize = sizeof(BYTE) * 1024;
	PBYTE pBytes = (PBYTE)malloc( nBufferSize );
	ZeroMemory(pBytes,1024);

	do{
		nRead = recv(m_hSocket,(char*)&(pBytes[nTotal]),nBufferSize,0);
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
