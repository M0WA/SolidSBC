#include "StdAfx.h"
#include "SolidSBCHarddriveConfig.h"


CSolidSBCHarddriveConfig::CSolidSBCHarddriveConfig(const CString& strXml)
: CSolidSBCTestConfig(_T(SSBC_HARDDRIVE_TEST_NAME),strXml)
{
	RegisterAttributes();
	Init();
}


CSolidSBCHarddriveConfig::~CSolidSBCHarddriveConfig(void)
{
}

void CSolidSBCHarddriveConfig::RegisterAttributes(void)
{
	RegisterAttribute(_T("RandomRead")    , _T("1")       );
	RegisterAttribute(_T("RandomWrite")   , _T("1")       );
	RegisterAttribute(_T("ReadMax")       , _T("10485760"));
	RegisterAttribute(_T("WriteMax")      , _T("10485760"));
	RegisterAttribute(_T("ReadWriteDelay"), _T("20")      );
	RegisterAttribute(_T("TransmitData")  , _T("1")       );
}

BOOL CSolidSBCHarddriveConfig::GetRandomRead(void)
{
	BOOL bRandomRead = false;
	GetAttributeByName<BOOL>(_T("RandomRead"), bRandomRead);
	return bRandomRead;
}

BOOL CSolidSBCHarddriveConfig::GetRandomWrite(void)
{
	BOOL bRandomWrite = false;
	GetAttributeByName<BOOL>(_T("RandomWrite"), bRandomWrite);
	return bRandomWrite;
}

ULONG CSolidSBCHarddriveConfig::GetReadMax(void)
{
	ULONG ulReadMax = 0;
	GetAttributeByName<ULONG>(_T("ReadMax"), ulReadMax);
	return ulReadMax;
}

ULONG CSolidSBCHarddriveConfig::GetWriteMax(void)
{
	ULONG ulWriteMax = 0;
	GetAttributeByName<ULONG>(_T("WriteMax"), ulWriteMax);
	return ulWriteMax;
}

UINT CSolidSBCHarddriveConfig::GetReadWriteDelay(void)
{
	UINT nReadWriteDelay = 0;
	GetAttributeByName<UINT>(_T("ReadWriteDelay"), nReadWriteDelay);
	return nReadWriteDelay;
}

BOOL CSolidSBCHarddriveConfig::GetTransmitData(void)
{
	BOOL bTransmitData = false;
	GetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData);
	return bTransmitData;
}
