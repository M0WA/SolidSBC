#include "StdAfx.h"
#include "SolidSBCCPUMeasureConfig.h"

CSolidSBCCPUMeasureConfig::CSolidSBCCPUMeasureConfig(void)
: CSolidSBCTestConfig(_T("CPU-Measure-Test"),_T("cpumeasure.xml"))
{
}

CSolidSBCCPUMeasureConfig::~CSolidSBCCPUMeasureConfig(void)
{
}

void CSolidSBCCPUMeasureConfig::RegisterAttributes(std::map<CString,CString>& mapAttributeXPaths)
{
	mapAttributeXPaths[_T("DivisionCnt")]           = GetXPathFromNodeName(_T("DivisionCnt"));
	mapAttributeXPaths[_T("AdditionCnt")]           = GetXPathFromNodeName(_T("AdditionCnt"));
	mapAttributeXPaths[_T("TransmitData")]          = GetXPathFromNodeName(_T("TransmitData"));
	mapAttributeXPaths[_T("AutoFixMultiplier")]     = GetXPathFromNodeName(_T("AutoFixMultiplier"));
	mapAttributeXPaths[_T("FixMultiplierTreshold")] = GetXPathFromNodeName(_T("FixMultiplierTreshold"));
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