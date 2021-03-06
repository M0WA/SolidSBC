#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCMemoryResult : CSolidSBCTestResult
{
public:
	CSolidSBCMemoryResult(void);

	DOUBLE GetMallocZeroDuration(void);
	inline void SetMallocZeroDuration(DOUBLE dMallocZeroDuration) { SetKeyValue( "malloczeroduration", dMallocZeroDuration); };

	ULONG  GetByteCount(void);
	inline void SetByteCount(ULONG ulBytes) { SetKeyValue( "bytes", ulBytes); };
};