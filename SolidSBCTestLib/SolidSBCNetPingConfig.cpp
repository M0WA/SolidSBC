#include "StdAfx.h"
#include "SolidSBCNetPingConfig.h"


CSolidSBCNetPingConfig::CSolidSBCNetPingConfig(const CString& strXml)
: CSolidSBCTestConfig(_T(SSBC_NETPING_TEST_NAME),strXml)
{
	RegisterAttributes();
	Init();
}


CSolidSBCNetPingConfig::~CSolidSBCNetPingConfig(void)
{
}

void CSolidSBCNetPingConfig::RegisterAttributes(void)
{
	RegisterAttribute(_T("Host")        , _T("127.0.0.1"));
	RegisterAttribute(_T("Interval")    , _T("60000")    );
	RegisterAttribute(_T("TTL")         , _T("1000")     );
	RegisterAttribute(_T("PayloadSize") , _T("32")       );
	RegisterAttribute(_T("TransmitData"), _T("1")        );
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
