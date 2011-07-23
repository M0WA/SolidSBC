#include "StdAfx.h"
#include "SolidSBCNetPingResult.h"

ULONG CSolidSBCNetPingResult::GetDuration(void)
{
	ULONG ullValue = 0;
	GetKeyValue( "rtt_duration", ullValue , (ULONG) 0 );
	return ullValue;
}

ULONGLONG CSolidSBCNetPingResult::GetICMPOpts(void)
{
	ULONGLONG ullValue = 0;
	GetKeyValue( "icmp_opts", ullValue , (ULONGLONG) 0.0 );
	return ullValue;
}
