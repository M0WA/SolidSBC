#include "StdAfx.h"
#include "SolidSBCNetTcpConfig.h"


CSolidSBCNetTcpConfig::CSolidSBCNetTcpConfig(const CString& strXml)
: CSolidSBCTestConfig(_T(SSBC_NETTCP_TEST_NAME),strXml)
{
	RegisterAttributes();
	Init();
}


CSolidSBCNetTcpConfig::~CSolidSBCNetTcpConfig(void)
{
}

void CSolidSBCNetTcpConfig::RegisterAttributes(void)
{
	RegisterAttribute(_T("Interval")    , _T("60000")    );
	RegisterAttribute(_T("Host")        , _T("127.0.0.1"));
	RegisterAttribute(_T("Port")        , _T("80")       );
	RegisterAttribute(_T("TTL")         , _T("1000")     );
	RegisterAttribute(_T("TransmitData"), _T("1")        );
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
