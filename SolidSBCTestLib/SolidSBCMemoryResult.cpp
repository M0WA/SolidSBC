#include "StdAfx.h"
#include "SolidSBCMemoryResult.h"

DOUBLE CSolidSBCMemoryResult::GetMallocZeroDuration(void)
{
	std::string sKey = "malloczeroduration";

	DOUBLE dValue = 0.0;
	GetKeyValue(sKey, dValue, (DOUBLE) 0.0);
	return dValue;
}

ULONG CSolidSBCMemoryResult::GetByteCount(void)
{
	std::string sKey = "bytes";

	ULONG ulValue = 0;
	GetKeyValue(sKey, ulValue, (ULONG) 0);
	return ulValue;
}