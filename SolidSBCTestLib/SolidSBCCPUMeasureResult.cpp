#include "StdAfx.h"
#include "SolidSBCCPUMeasureResult.h"

DOUBLE CSolidSBCCPUMeasureResult::GetAddDuration(void)
{
	std::string sKey = "addduration";
	DOUBLE dValue = 0.0;
	GetKeyValue( sKey, dValue, (DOUBLE) 0.0 );
	return dValue;
}

DOUBLE CSolidSBCCPUMeasureResult::GetDivDuration(void)
{
	std::string sKey = "divduration";
	DOUBLE dValue = 0.0;
	GetKeyValue( sKey, dValue, (DOUBLE) 0.0 );
	return dValue;
}

DOUBLE CSolidSBCCPUMeasureResult::GetOverallDuration(void)
{
	std::string sKey = "overallduration";
	DOUBLE dValue = 0.0;
	GetKeyValue( sKey, dValue, (DOUBLE) 0.0 );
	return dValue;
}

ULONGLONG CSolidSBCCPUMeasureResult::GetAddMultiplier(void)
{
	std::string sKey = "addmultiplier";
	ULONGLONG ullValue = 0;
	GetKeyValue( sKey, ullValue, (ULONGLONG) 0 );
	return ullValue;
}

ULONGLONG CSolidSBCCPUMeasureResult::GetDivMultiplier(void)
{
	std::string sKey = "divmultiplier";
	ULONGLONG ullValue = 0;
	GetKeyValue( sKey, ullValue, (ULONGLONG) 0 );
	return ullValue;
}

