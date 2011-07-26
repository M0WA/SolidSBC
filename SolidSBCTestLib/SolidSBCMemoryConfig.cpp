#include "StdAfx.h"
#include "SolidSBCMemoryConfig.h"


CSolidSBCMemoryConfig::CSolidSBCMemoryConfig(const CString& strXml)
: CSolidSBCTestConfig(_T(SSBC_MEMORY_TEST_NAME),strXml)
{
	RegisterAttributes();
	Init();
}

CSolidSBCMemoryConfig::~CSolidSBCMemoryConfig(void)
{
}

void CSolidSBCMemoryConfig::RegisterAttributes(void)
{
	RegisterAttribute(_T("Randomize")   , _T("1") );
	RegisterAttribute(_T("MinMem")      , _T("1") );
	RegisterAttribute(_T("MaxMem")      , _T("1048576") );
	RegisterAttribute(_T("TransmitData"), _T("1") );
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