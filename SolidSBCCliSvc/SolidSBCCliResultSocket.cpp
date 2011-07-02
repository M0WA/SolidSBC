// ServerResultSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliResultSocket.h"

CSolidSBCCliResultSocket::CSolidSBCCliResultSocket()
: CSolidSBCClientSocket()
{
}

CSolidSBCCliResultSocket::~CSolidSBCCliResultSocket()
{
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
		GetNextPacket();
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

bool CSolidSBCCliResultSocket::OnRead()
{
	int nRead = 0, nTotal = 0;
	int nBufferSize = sizeof(BYTE) * 1024;
	PBYTE pBytes = (PBYTE)malloc( nBufferSize );
	ZeroMemory(pBytes,1024);

	do{
		//read from socket
		nRead = recv(m_hSocket,(char*)&(pBytes[nTotal]),nBufferSize,0);

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

int CSolidSBCCliResultSocket::SendPacket(PSSBC_BASE_PACKET_HEADER pPacket)
{
	return send( m_hSocket,(char*)pPacket, pPacket->nPacketSize,0 );
}

int CSolidSBCCliResultSocket::SendResultConnRequest(void)
{
	USES_CONVERSION;
	DWORD nNameSize = 1024;
	TCHAR szComputerName[1024] = {0};
	GetComputerName(szComputerName,&nNameSize);

	int nPacketSize = sizeof(SSBC_BASE_PACKET_HEADER) + sizeof(SSBC_CONN_RES_REQUEST_PACKET);
	PBYTE pSendPacket = new BYTE[nPacketSize];
	ZeroMemory(pSendPacket,nPacketSize);

	((PSSBC_BASE_PACKET_HEADER)pSendPacket)->type = SSBC_CONN_RES_REQUEST_PACKET_TYPE;
	((PSSBC_BASE_PACKET_HEADER)pSendPacket)->nPacketSize = nPacketSize;

	PSSBC_CONN_RES_REQUEST_PACKET pPacket = (PSSBC_CONN_RES_REQUEST_PACKET)(pSendPacket+sizeof(SSBC_BASE_PACKET_HEADER));
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
	int nSent = SendPacket((PSSBC_BASE_PACKET_HEADER)pSendPacket);
	delete [] pSendPacket;
	return nSent;
}