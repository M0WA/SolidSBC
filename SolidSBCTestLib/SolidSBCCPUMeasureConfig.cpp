#include "StdAfx.h"
#include "SolidSBCCPUMeasureConfig.h"

CSolidSBCCPUMeasureConfig::CSolidSBCCPUMeasureConfig(const CString& strXml)
: CSolidSBCTestConfig(_T(SSBC_CPUMEASURE_TEST_NAME),strXml)
{
	RegisterAttributes();
	Init();
}

CSolidSBCCPUMeasureConfig::~CSolidSBCCPUMeasureConfig(void)
{
}

void CSolidSBCCPUMeasureConfig::RegisterAttributes(void)
{
	RegisterAttribute(_T("DivisionCnt")          , _T("1000"));
	RegisterAttribute(_T("AdditionCnt")          , _T("1000"));
	RegisterAttribute(_T("AutoFixMultiplier")    , _T("3")   );
	RegisterAttribute(_T("FixMultiplierTreshold"), _T("0.5") );
	RegisterAttribute(_T("TransmitData")         , _T("1")   );
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