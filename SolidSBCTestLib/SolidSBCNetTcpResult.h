#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCNetTcpResult : CSolidSBCTestResult
{
public:
	CSolidSBCNetTcpResult(void);
	
	DOUBLE GetDuration(void);
	inline void SetDuration(DOUBLE dDuration) { SetKeyValue( "handshake_duration", dDuration); }
};