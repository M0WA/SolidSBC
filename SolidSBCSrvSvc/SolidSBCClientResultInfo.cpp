#include "StdAfx.h"
#include "SolidSBCClientResultInfo.h"

CSolidSBCClientResultInfo::CSolidSBCClientResultInfo( struct sockaddr_in client, CSolidSBCResultClientHandlerSocket* pResultClientHandlerSock, UINT nClientID )
: m_strClientName(_T("na"))
, m_nProfileID(-1)
{
	m_pResultClientHandlerSock = pResultClientHandlerSock;
	m_Client    = client;
	m_nClientID = nClientID;
}

CSolidSBCClientResultInfo::~CSolidSBCClientResultInfo(void)
{
	/*
	if (m_pResultClientSock){
		m_pResultClientSock->Close();
		delete m_pResultClientSock;
		m_pResultClientSock = NULL;}
	*/
}
