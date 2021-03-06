#include "StdAfx.h"
#include "SolidSBCHarddriveResult.h"

CSolidSBCHarddriveResult::CSolidSBCHarddriveResult(void) 
: CSolidSBCTestResult("testLib_HDResults") 
{
	RegisterAttributeDataType("duration",DB_DOUBLE);
	RegisterAttributeDataType("amountbytes",DB_INT32);
	RegisterAttributeDataType("wait",DB_INT32);
	RegisterAttributeDataType("resulttype",DB_INT16);
}

DOUBLE CSolidSBCHarddriveResult::GetDuration(void)
{
	DOUBLE dValue = 0.0;
	GetKeyValue( "duration", dValue , (DOUBLE) 0.0 );
	return dValue;
}

ULONG CSolidSBCHarddriveResult::GetByteCount(void)
{
	ULONG ulValue = 0;
	GetKeyValue( "amountbytes", ulValue , (ULONG) 0 );
	return ulValue;
}

ULONG CSolidSBCHarddriveResult::GetWaitCount(void)
{
	ULONG ulValue = 0;
	GetKeyValue( "wait", ulValue , (ULONG) 0 );
	return ulValue;
}

SSBC_TEST_HARDDRIVE_RESULT_TYPE CSolidSBCHarddriveResult::GetResultType(void)
{
	int nValue = 0;
	GetKeyValue( "resulttype", nValue , (int) 0 );
	return (SSBC_TEST_HARDDRIVE_RESULT_TYPE)nValue;
}
