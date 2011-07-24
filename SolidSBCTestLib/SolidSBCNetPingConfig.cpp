#include "StdAfx.h"
#include "SolidSBCNetPingConfig.h"


CSolidSBCNetPingConfig::CSolidSBCNetPingConfig(void)
: CSolidSBCTestConfig(_T(SSBC_NETPING_TEST_NAME),_T("netping.xml"))
{
}


CSolidSBCNetPingConfig::~CSolidSBCNetPingConfig(void)
{
}

void CSolidSBCNetPingConfig::RegisterAttributes(void)
{
	RegisterXPathByAttributeName(_T("Interval"));
	RegisterXPathByAttributeName(_T("Host"));
	RegisterXPathByAttributeName(_T("TTL"));
	RegisterXPathByAttributeName(_T("PayloadSize"));
	RegisterXPathByAttributeName(_T("TransmitData"));
}

UINT  CSolidSBCNetPingConfig::GetInterval(void)
{
	UINT nInterval = 0;
	GetAttributeByName<UINT>(_T("Interval"), nInterval);
	return nInterval;
}

CString CSolidSBCNetPingConfig::GetHost(void)
{
	std::string strHost;
	GetAttributeByName<std::string>(_T("Host"), strHost);
	return CString(strHost.c_str());
}

UINT CSolidSBCNetPingConfig::GetTTL(void)
{
	UINT nTTL = 0;
	GetAttributeByName<UINT>(_T("TTL"), nTTL);
	return nTTL;
}

UINT CSolidSBCNetPingConfig::GetPayloadSize(void)
{
	UINT nPayloadSize = 0;
	GetAttributeByName<UINT>(_T("PayloadSize"), nPayloadSize);
	return nPayloadSize;
}

BOOL CSolidSBCNetPingConfig::GetTransmitData(void)
{
	BOOL bTransmitData = false;
	GetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData);
	return bTransmitData;
}
