#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCCPUMeasureResult : CSolidSBCTestResult
{
public:
	CSolidSBCCPUMeasureResult(void) : CSolidSBCTestResult("cpumeasureResults") {};

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

