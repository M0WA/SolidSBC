#pragma once

#include "SolidSBCTestLib.h"
#include "../SolidSBCTestSDK/SolidSBCTestSDK.h"

class SOLIDSBCTESTLIB_API CSolidSBCMemoryResult : CSolidSBCTestResult
{
public:
	CSolidSBCMemoryResult(void) : CSolidSBCTestResult("memResults") {};
	~CSolidSBCMemoryResult(void) {};

	DOUBLE GetMallocZeroDuration(void);
	inline void SetMallocZeroDuration(DOUBLE dMallocZeroDuration) { SetKeyValue( "malloczeroduration", dMallocZeroDuration); };

	ULONG  GetByteCount(void);
	inline void SetByteCount(ULONG ulBytes) { SetKeyValue( "bytes", ulBytes); };
};