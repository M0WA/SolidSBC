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
	{
		CString strMsg;
		strMsg.Format(_T("CSolidSBCCliConfigSocket::OnConnect(bool bSuccess = %d)"), bSuccess);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
	
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
	std::vector<byte> vecPacketBytes;
	configRequest.GetPacketBytes(vecPacketBytes);

	int nPacketSize = (int)vecPacketBytes.size();
	PBYTE pPacketBytes = new BYTE[nPacketSize];
	memset(pPacketBytes,0,nPacketSize);
	for(int i = 0; i < nPacketSize; i++)
		pPacketBytes[i] = vecPacketBytes[i];
	
	u_long iMode = 0;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	int nSent = send(m_hSocket,(char *)&pPacketBytes,sizeof(nPacketSize),0);

	iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	delete pPacketBytes;
	return nSent ? true : false;
}

bool CSolidSBCCliConfigSocket::ReceiveConfigResponse(void)
{
	int nHeaderSize = sizeof(SSBC_PACKET_HEADER);
	SSBC_PACKET_HEADER header;
	memset(&header,0,nHeaderSize);
	
	u_long iMode = 0;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);

	int nRead = recv(m_hSocket,(char*)&header,nHeaderSize,0);
	if ( nRead != nHeaderSize )
		return false;

	int nPayloadSize = header.nPacketSize - nHeaderSize;
	PBYTE pPayload   = new BYTE[nPayloadSize];
	memset(pPayload,0,nPayloadSize);
	nRead = recv(m_hSocket,(char*)pPayload,nPayloadSize,0);
	if ( nRead != nPayloadSize ){
		delete pPayload;
		return false;}

	iMode = 1;
	ioctlsocket(m_hSocket, FIONBIO, &iMode);
	
	wchar_t* pwszPayload = (wchar_t*)pPayload;


#ifdef _UNICODE
	m_vecTestConfigs.push_back(pwszPayload);
	{
		CString strMsg;
		strMsg.Format(_T("Fetched config: %s successfully from server."),pwszPayload);
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#elif
	USES_CONVERSION;
	m_vecTestConfigs.push_back(W2A(pwszPayload));
	{
		CString strMsg;
		strMsg.Format(_T("Fetched config (id: %s) successfully from server."),W2A(pwszPayload));
		CSolidSBCCliServiceWnd::LogServiceMessage(strMsg,SSBC_CLISVC_LOGMSG_TYPE_DEBUG);
	}
#endif

	delete pPayload;
	return true;
}
