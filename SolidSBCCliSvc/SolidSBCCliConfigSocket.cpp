// ServerConfigSocket.cpp : implementation file
//

#include "stdafx.h"
#include "SolidSBCCliConfigSocket.h"

CSolidSBCCliConfigSocket::CSolidSBCCliConfigSocket()
: CSolidSBCSocketClient()
, m_pszClientUUID(NULL)
{
}

CSolidSBCCliConfigSocket::~CSolidSBCCliConfigSocket()
{
	Close();
}

bool CSolidSBCCliConfigSocket::OnConnect(bool bSuccess)
{
#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::OnConnect(bool bSuccess = %d)"), bSuccess);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif
	
	if (bSuccess && SendConfigRequest()){
		GetNextPacket();
	} else {
		{
			CString strMsg;
			strMsg.Format(_T("Could not connect to config server."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		g_cClientService.ConnectionClosed();
	}
	return bSuccess;
}

bool CSolidSBCCliConfigSocket::OnRead()
{	
	if ( ReceiveConfigResponse() ){
		GetNextPacket();
		return true;}
	else if ( m_vecTestConfigs.size() ){
		{
			CString strMsg;
			strMsg.Format(_T("Fetched %d configs from config server."), m_vecTestConfigs.size());
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
		}
		Close();

		g_cClientService.StartResultConnection();
		g_cClientService.StartTests();
		return false;}
	else {
		{
			CString strMsg;
			strMsg.Format(_T("Could not fetch config from server."));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		return false;
	}
}

bool CSolidSBCCliConfigSocket::SendConfigRequest(void)
{
	{
		CString strMsg;
		strMsg.Format( _T("Requesting configs for this client.") );
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_INFO);
	}

	DWORD nNameSize = 1024;
	TCHAR szComputerName[1024] = {0};
	GetComputerName(szComputerName,&nNameSize);

	USES_CONVERSION;
	CSolidSBCPacketConfigRequest configRequest(A2T(m_pszClientUUID),szComputerName);

	int nPacketSize = 0;
	PBYTE pPacketBytes = configRequest.GetPacketBytes(nPacketSize);

	u_long iMode = 0;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	int nSent = send(m_hSocket,(char*)pPacketBytes,nPacketSize,0);
	DWORD dwError = GetLastError();

	iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);
	
#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format( _T("CSolidSBCCliConfigSocket::SendConfigRequest(): Sent config request. %d of %d byte(-s)\r\n %s"), nSent,  nPacketSize, configRequest.GetXml());
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif
	
	if ( (nSent == -1) || ( nSent != nPacketSize ))
	{
		{
			CString strMsg;
			strMsg.Format( _T("Error while sending config request (%d)."), dwError);
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_ERROR);
		}
		nSent = 0;
	}

	delete [] pPacketBytes;
	return nSent ? true : false;
}

bool CSolidSBCCliConfigSocket::ReceiveConfigResponse(void)
{
	PBYTE pPacket = CSolidSBCPacket::ReceivePacket(m_hSocket);
	if(!pPacket)
	{
#ifdef _DEBUG
		{
			CString strMsg;
			strMsg.Format(_T("CSolidSBCCliConfigSocket::ReceiveConfigResponse(): Error while receiving ConfigResponse packet"));
			CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
		}
#endif
		return false;
	}

	/*
	if( ((PSSBC_PACKET_HEADER)pPacket)->nType != SSBC_PACKET_TYPE_CONFIG_RESPONSE)
	{
		return false;
	}
	*/

	CSolidSBCPacketConfigResponse response(pPacket);
	delete [] pPacket;

	CString strConfig = response.GetXml();
	m_vecTestConfigs.push_back(strConfig);

#ifdef _DEBUG
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::ReceiveConfigResponse(): Fetched config successfully from server:\r\n%s"),strConfig);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif

	return true;
}
