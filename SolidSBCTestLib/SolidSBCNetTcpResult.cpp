#include "StdAfx.h"
#include "SolidSBCNetTcpResult.h"

CSolidSBCNetTcpResult::CSolidSBCNetTcpResult(void)
: CSolidSBCTestResult("testLib_NetTCPResults") 
{
	RegisterAttributeDataType("handshake_duration", DB_DOUBLE);
}

DOUBLE CSolidSBCNetTcpResult::GetDuration(void)
{
	DOUBLE dValue = 0.0;
	GetKeyValue( "handshake_duration", dValue , (DOUBLE) 0.0 );
	return dValue;
}