#include "StdAfx.h"
#include "SolidSBCCPUMeasureConfig.h"

CSolidSBCCPUMeasureConfig::CSolidSBCCPUMeasureConfig(void)
: CSolidSBCTestConfig(_T(SSBC_CPUMEASURE_TEST_NAME),_T("cpumeasure.xml"))
{
	RegisterAttributes();
}

CSolidSBCCPUMeasureConfig::~CSolidSBCCPUMeasureConfig(void)
{
}

void CSolidSBCCPUMeasureConfig::RegisterAttributes(void)
{
	RegisterXPathByAttributeName(_T("DivisionCnt"));
	RegisterXPathByAttributeName(_T("AdditionCnt"));
	RegisterXPathByAttributeName(_T("AutoFixMultiplier"));
	RegisterXPathByAttributeName(_T("FixMultiplierTreshold"));
	RegisterXPathByAttributeName(_T("TransmitData"));
}

UINT CSolidSBCCPUMeasureConfig::GetDivisionCnt(void)
{
	UINT nDivisionCnt = 0;
	GetAttributeByName<UINT>(_T("DivisionCnt"), nDivisionCnt);
	return nDivisionCnt;
}

UINT CSolidSBCCPUMeasureConfig::GetAdditionCnt(void)
{
	UINT nAdditionCnt = 0;
	GetAttributeByName<UINT>(_T("AdditionCnt"), nAdditionCnt);
	return nAdditionCnt;
}

BOOL CSolidSBCCPUMeasureConfig::GetTransmitData(void)
{
	BOOL bTransmitData = false;
	GetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData);
	return bTransmitData;
}

UINT CSolidSBCCPUMeasureConfig::GetAutoFixMultiplier(void)
{
	UINT nAutoFixMultiplier = 0;
	GetAttributeByName<UINT>(_T("AutoFixMultiplier"), nAutoFixMultiplier);
	return nAutoFixMultiplier;
}

double CSolidSBCCPUMeasureConfig::GetFixMultiplierTreshold(void)
{
	double dFixMultiplierTreshold = 0.0;
	GetAttributeByName<double>(_T("FixMultiplierTreshold"), dFixMultiplierTreshold);
	return dFixMultiplierTreshold;
}