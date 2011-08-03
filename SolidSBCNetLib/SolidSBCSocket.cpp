#include "StdAfx.h"
#include "SolidSBCSocket.h"


CSolidSBCSocket::CSolidSBCSocket(void)
: m_hSocket(INVALID_SOCKET)
{
}


CSolidSBCSocket::~CSolidSBCSocket(void)
{
}

void CSolidSBCSocket::Close(void)
{
	if(m_hSocket != INVALID_SOCKET)
		closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}