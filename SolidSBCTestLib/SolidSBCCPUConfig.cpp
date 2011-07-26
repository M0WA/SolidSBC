#include "StdAfx.h"
#include "SolidSBCCPUConfig.h"

CSolidSBCCPUConfig::CSolidSBCCPUConfig(const CString& strXml)
: CSolidSBCTestConfig(_T(SSBC_CPU_TEST_NAME),strXml)
{
	RegisterAttributes();
	Init();
}

CSolidSBCCPUConfig::~CSolidSBCCPUConfig(void)
{
}

void CSolidSBCCPUConfig::RegisterAttributes(void)
{
	RegisterAttribute(_T("Randomize"), _T("1"));
	RegisterAttribute(_T("MaxRand")  , _T("20"));
	RegisterAttribute(_T("SleepMS")  , _T("5.000000"));
	RegisterAttribute(_T("ThreadCnt"), _T("2"));
}

BOOL CSolidSBCCPUConfig::GetRandomize(void)
{
	BOOL bRandomize = false;
	GetAttributeByName<BOOL>(_T("Randomize"), bRandomize);
	return bRandomize;
}

UINT CSolidSBCCPUConfig::GetMaxRand(void)
{
	UINT nMaxRand = 0;
	GetAttributeByName<UINT>(_T("MaxRand"), nMaxRand);
	return nMaxRand;
}

double CSolidSBCCPUConfig::GetSleepMS(void)
{
	double dSleepMS = 0.0;
	GetAttributeByName<double>(_T("SleepMS"), dSleepMS);
	return dSleepMS;
}

UINT CSolidSBCCPUConfig::GetThreadCnt(void)
{
	UINT nThreadCnt = 0;
	GetAttributeByName<UINT>(_T("ThreadCnt"), nThreadCnt);
	return nThreadCnt;
}