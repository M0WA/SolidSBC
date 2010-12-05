#include "StdAfx.h"
#include "SolidSBCClientConfigInfo.h"

CSolidSBCClientConfigInfo::CSolidSBCClientConfigInfo(void)
{
}

CSolidSBCClientConfigInfo::CSolidSBCClientConfigInfo( struct sockaddr_in client, CSolidSBCConfigClientSocket* pConfigClientSock )
{
	m_Client= client;
	m_pConfigClientSock = pConfigClientSock;
}


CSolidSBCClientConfigInfo::~CSolidSBCClientConfigInfo(void)
{
	m_pConfigClientSock = NULL;
}
