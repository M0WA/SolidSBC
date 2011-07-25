#include "StdAfx.h"
#include "SolidSBCNetTcpConfig.h"


CSolidSBCNetTcpConfig::CSolidSBCNetTcpConfig(void)
: CSolidSBCTestConfig(_T(SSBC_NETTCP_TEST_NAME),_T("nettcp.xml"))
{
	RegisterAttributes();
}


CSolidSBCNetTcpConfig::~CSolidSBCNetTcpConfig(void)
{
}

void CSolidSBCNetTcpConfig::RegisterAttributes(void)
{
	RegisterXPathByAttributeName(_T("Interval"));
	RegisterXPathByAttributeName(_T("Host"));
	RegisterXPathByAttributeName(_T("Port"));
	RegisterXPathByAttributeName(_T("TTL"));
	RegisterXPathByAttributeName(_T("TransmitData"));
}

UINT CSolidSBCNetTcpConfig::GetInterval(void)
{
	UINT nInterval = false;
	GetAttributeByName<UINT>(_T("Interval"), nInterval);
	return nInterval;
}

CString CSolidSBCNetTcpConfig::GetHost(void)
{
	std::string strHost;
	GetAttributeByName<std::string>(_T("Host"), strHost);
	return CString(strHost.c_str());
}

USHORT CSolidSBCNetTcpConfig::GetPort(void)
{
	USHORT nPort = 0;
	GetAttributeByName<USHORT>(_T("Port"), nPort);
	return nPort;
}

UINT CSolidSBCNetTcpConfig::GetTTL(void)
{
	UINT nTTL = false;
	GetAttributeByName<UINT>(_T("TTL"), nTTL);
	return nTTL;
}

BOOL CSolidSBCNetTcpConfig::GetTransmitData(void)
{
	BOOL bTransmitData = false;
	GetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData);
	return bTransmitData;
}
