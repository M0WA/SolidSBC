#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCNetPingResult : CSolidSBCTestResult
{
public:
	CSolidSBCNetPingResult(void);

	ULONG GetDuration(void);
	inline void SetDuration(ULONG ullDuration) { SetKeyValue( "rtt_duration", ullDuration); }

	ULONGLONG GetICMPOpts(void);
	inline void SetICMPOpts(ULONGLONG ullICMPOpts) { SetKeyValue( "icmp_opts", ullICMPOpts); }
};

