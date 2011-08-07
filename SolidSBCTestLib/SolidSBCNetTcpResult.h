#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCNetTcpResult : CSolidSBCTestResult
{
public:
	CSolidSBCNetTcpResult(void) : CSolidSBCTestResult("testLib_TCPResults") {};
	
	DOUBLE GetDuration(void);
	inline void SetDuration(DOUBLE dDuration) { SetKeyValue( "handshake_duration", dDuration); }
};

