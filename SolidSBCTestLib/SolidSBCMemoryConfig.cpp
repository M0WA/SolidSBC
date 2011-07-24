#include "StdAfx.h"
#include "SolidSBCMemoryConfig.h"


CSolidSBCMemoryConfig::CSolidSBCMemoryConfig(void)
: CSolidSBCTestConfig(_T(SSBC_MEMORY_TEST_NAME),_T("memory.xml"))
{
}

CSolidSBCMemoryConfig::~CSolidSBCMemoryConfig(void)
{
}

void CSolidSBCMemoryConfig::RegisterAttributes(void)
{
	RegisterXPathByAttributeName(_T("Randomize"));
	RegisterXPathByAttributeName(_T("MinMem"));
	RegisterXPathByAttributeName(_T("MaxMem"));
	RegisterXPathByAttributeName(_T("TransmitData"));
}

BOOL CSolidSBCMemoryConfig::GetRandomize(void)
{
	BOOL bRandomize = false;
	GetAttributeByName<BOOL>(_T("Randomize"), bRandomize);
	return bRandomize;
}

UINT CSolidSBCMemoryConfig::GetMinMem(void)
{
	UINT nMinMem = 0;
	GetAttributeByName<UINT>(_T("MinMem"), nMinMem);
	return nMinMem;
}

UINT CSolidSBCMemoryConfig::GetMaxMem(void)
{
	UINT nMaxMem = 0;
	GetAttributeByName<UINT>(_T("MaxMem"), nMaxMem);
	return nMaxMem;
}

BOOL CSolidSBCMemoryConfig::GetTransmitData(void)
{
	BOOL bTransmitData = false;
	GetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData);
	return bTransmitData;
}