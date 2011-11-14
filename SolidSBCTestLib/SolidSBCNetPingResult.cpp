#include "StdAfx.h"
#include "SolidSBCNetPingResult.h"

CSolidSBCNetPingResult::CSolidSBCNetPingResult(void) 
: CSolidSBCTestResult("testLib_NetPingResults")
{
	RegisterAttributeDataType("rtt_duration", DB_INT32);
	RegisterAttributeDataType("icmp_opts", DB_INT64);
}

ULONG CSolidSBCNetPingResult::GetDuration(void)
{
	ULONG ullValue = 0;
	GetKeyValue<ULONG>( "rtt_duration", ullValue , (ULONG) 0 );
	return ullValue;
}

ULONGLONG CSolidSBCNetPingResult::GetICMPOpts(void)
{
	ULONGLONG ullValue = 0;
	GetKeyValue<ULONGLONG>( "icmp_opts", ullValue , (ULONGLONG) 0 );
	return ullValue;
}
