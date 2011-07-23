#pragma once

#include "SolidSBCTestLib.h"
#include "../SolidSBCTestSDK/SolidSBCTestSDK.h"

/*
typedef struct : SSBC_BASE_PACKET_HEADER {
	DOUBLE		dAddDuration;
	DOUBLE		dDivDuration;
	DOUBLE		dOverallDuration;
	ULONGLONG	ullAddMultiplier;
	ULONGLONG	ullDivMultiplier;
} SSBC_CPUMEASURE_TESTRESULT_PACKET, *PSSBC_CPUMEASURE_TESTRESULT_PACKET;
*/

class SOLIDSBCTESTLIB_API CSolidSBCCPUMeasureResult : CSolidSBCTestResult
{
public:
	CSolidSBCCPUMeasureResult(void) : CSolidSBCTestResult("cpumeasureResults") {};
	~CSolidSBCCPUMeasureResult(void) {};

	DOUBLE GetAddDuration(void);
	inline void SetAddDuration(DOUBLE dAddDuration) { SetKeyValue( "addduration", dAddDuration); }
	
	DOUBLE GetDivDuration(void);
	inline void SetDivDuration(DOUBLE dDivDuration) { SetKeyValue( "divduration", dDivDuration); }
	
	DOUBLE GetOverallDuration(void);
	inline void SetOverallDuration(DOUBLE dOverallDuration) { SetKeyValue( "overallduration", dOverallDuration); }
	
	ULONGLONG GetAddMultiplier(void);
	inline void SetAddMultiplier(ULONGLONG ullAddMultiplier) { SetKeyValue( "addmultiplier", ullAddMultiplier); }
	
	ULONGLONG GetDivMultiplier(void);
	inline void SetDivMultiplier(ULONGLONG ullDivMultiplier) { SetKeyValue( "divmultiplier", ullDivMultiplier); }
};

