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
	
#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::OnConnect(bool bSuccess = %d)"), bSuccess);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif

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
}

int CSolidSBCCliResultSocket::SendTestResultPacket(CSolidSBCTestResult* pResult)
{
	int nPacketSize = 0;
	CSolidSBCPacketTestResult resultPacket(pResult);
	PBYTE pPacketBytes = resultPacket.GetPacketBytes(nPacketSize);

	int nReturn = send( m_hSocket,(char*)pPacketBytes, nPacketSize,0 );

#ifdef _DEBUG
	{
		USES_CONVERSION;
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::SendTestResultPacket(): Sent result %d byte(-s):\r\n%s"),nReturn,A2T(pResult->ToSQL().c_str()));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif

	delete [] pPacketBytes;
	return nReturn == nPacketSize ? nPacketSize : -1;
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
	
#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliResultSocket::SendResultConnRequest(): sending result connection request."));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif
	int nSent = send(m_hSocket,(char*)pPacketBytes,nPacketSize,0);

	delete [] pPacketBytes;
	return nSent;
}