#include "StdAfx.h"
#include "SolidSBCCPUConfig.h"

CSolidSBCCPUConfig::CSolidSBCCPUConfig(void)
: CSolidSBCTestConfig(_T(SSBC_CPU_TEST_NAME),_T("cpu.xml"))
{
}

CSolidSBCCPUConfig::~CSolidSBCCPUConfig(void)
{
}

void CSolidSBCCPUConfig::RegisterAttributes(void)
{
	RegisterXPathByAttributeName(_T("Randomize"));
	RegisterXPathByAttributeName(_T("MaxRand"));
	RegisterXPathByAttributeName(_T("SleepMS"));
	RegisterXPathByAttributeName(_T("ThreadCnt"));
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