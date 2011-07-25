#include "StdAfx.h"
#include "SolidSBCHarddriveConfig.h"


CSolidSBCHarddriveConfig::CSolidSBCHarddriveConfig(void)
: CSolidSBCTestConfig(_T(SSBC_HARDDRIVE_TEST_NAME),_T("harddrive.xml"))
{
	RegisterAttributes();
}


CSolidSBCHarddriveConfig::~CSolidSBCHarddriveConfig(void)
{
}

void CSolidSBCHarddriveConfig::RegisterAttributes(void)
{
	RegisterXPathByAttributeName(_T("RandomRead"));
	RegisterXPathByAttributeName(_T("RandomWrite"));
	RegisterXPathByAttributeName(_T("ReadMax"));
	RegisterXPathByAttributeName(_T("WriteMax"));
	RegisterXPathByAttributeName(_T("ReadWriteDelay"));
	RegisterXPathByAttributeName(_T("TransmitData"));
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
