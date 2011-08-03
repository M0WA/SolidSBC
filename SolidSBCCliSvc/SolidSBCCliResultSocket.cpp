// ServerResultSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliResultSocket.h"

CSolidSBCCliResultSocket::CSolidSBCCliResultSocket()
: CSolidSBCSocketClient()
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
	return false;

	/*
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
	*/
}

int CSolidSBCCliResultSocket::SendTestResultPacket(CSolidSBCTestResult* pResult)
{
	int nPacketSize = 0;
	CSolidSBCPacketTestResult resultPacket(pResult);
	PBYTE pPacketBytes = resultPacket.GetPacketBytes(nPacketSize);

	int nReturn = send( m_hSocket,(char*)pPacketBytes, nPacketSize,0 );

	delete [] pPacketBytes;
	return nReturn;
}

int CSolidSBCCliResultSocket::SendResultConnRequest(void)
{
	USES_CONVERSION;
	DWORD nNameSize = 1024;
	TCHAR szComputerName[1024] = {0};
	GetComputerName(szComputerName,&nNameSize);

	CSolidSBCPacketResultRequest requestPacket(szComputerName,A2T(m_pszClientUUID));
	int nPacketSize = 0;
	PBYTE pPacketBytes = requestPacket.GetPacketBytes(nPacketSize);
	
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::SendResultConnRequest(): sending result connection request."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
	int nSent = send(m_hSocket,(char*)pPacketBytes,nPacketSize,0);

	delete [] pPacketBytes;
	return nSent;
}