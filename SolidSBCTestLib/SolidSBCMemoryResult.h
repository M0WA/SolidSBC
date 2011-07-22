#pragma once

#include "SolidSBCTestLib.h"
#include "../SolidSBCTestSDK/SolidSBCTestSDK.h"

typedef struct : SSBC_BASE_PACKET_HEADER {
	DOUBLE dMallocZeroDuration;
	ULONG  ulBytes;
} SSBC_MEMORY_TESTRESULT_PACKET, *PSSBC_MEMORY_TESTRESULT_PACKET;

class SOLIDSBCTESTLIB_API CSolidSBCMemoryResult : CSolidSBCTestResult
{
public:
	CSolidSBCMemoryResult(void) : CSolidSBCTestResult("memResults") {};
	~CSolidSBCMemoryResult(void) {};

	inline DOUBLE GetMallocZeroDuration(void)                       { return m_Result.dMallocZeroDuration; }
	inline void   SetMallocZeroDuration(DOUBLE dMallocZeroDuration) { m_Result.dMallocZeroDuration = dMallocZeroDuration; }

	inline ULONG  GetByteCount(void)          { return m_Result.ulBytes;    }
	inline void   SetByteCount(ULONG ulBytes) { m_Result.ulBytes = ulBytes; }

	virtual byte* ToBytes (void) { return GetPacketFromHeader(&m_Result); };
	
protected:
	virtual std::map<std::string,std::string> GetColumnsValues(void);

private:
	SSBC_MEMORY_TESTRESULT_PACKET m_Result;
};