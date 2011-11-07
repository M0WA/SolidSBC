#include "StdAfx.h"
#include "SolidSBCMemoryResult.h"

CSolidSBCMemoryResult::CSolidSBCMemoryResult(void)
: CSolidSBCTestResult("testLib_MemoryResults")
{
	RegisterAttributeDataType("malloczeroduration", DB_DOUBLE);
	RegisterAttributeDataType("bytes", DB_UINT32);
}

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