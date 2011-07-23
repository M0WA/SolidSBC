#include "StdAfx.h"
#include "SolidSBCNetTcpResult.h"

DOUBLE CSolidSBCNetTcpResult::GetDuration(void)
{
	DOUBLE dValue = 0.0;
	GetKeyValue( "handshake_duration", dValue , (DOUBLE) 0.0 );
	return dValue;
}