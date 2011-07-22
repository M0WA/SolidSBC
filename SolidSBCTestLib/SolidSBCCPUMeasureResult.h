#pragma once

#include "SolidSBCTestLib.h"
#include "../SolidSBCTestSDK/SolidSBCTestSDK.h"

typedef struct : SSBC_BASE_PACKET_HEADER {
	DOUBLE		dAddDuration;
	DOUBLE		dDivDuration;
	DOUBLE		dOverallDuration;
	ULONGLONG	ullAddMultiplier;
	ULONGLONG	ullDivMultiplier;
} SSBC_CPUMEASURE_TESTRESULT_PACKET, *PSSBC_CPUMEASURE_TESTRESULT_PACKET;

class SOLIDSBCTESTLIB_API CSolidSBCCPUMeasureResult : CSolidSBCTestResult
{
public:
	CSolidSBCCPUMeasureResult(void) : CSolidSBCTestResult("cpumeasureResults") {};
	~CSolidSBCCPUMeasureResult(void) {};

	inline DOUBLE GetAddDuration(void)                { return m_Result.dAddDuration; }
	inline void   SetAddDuration(DOUBLE dAddDuration) { m_Result.dAddDuration = dAddDuration; }
	
	inline DOUBLE GetDivDuration(void)                { return m_Result.dDivDuration; }
	inline void   SetDivDuration(DOUBLE dDivDuration) { m_Result.dDivDuration = dDivDuration; }
	
	inline DOUBLE GetOverallDuration(void)                    { return m_Result.dOverallDuration; }
	inline void   SetOverallDuration(DOUBLE dOverallDuration) { m_Result.dOverallDuration = dOverallDuration; }
	
	inline ULONGLONG GetAddMultiplier(void)                       { return m_Result.ullAddMultiplier; }
	inline void      SetAddMultiplier(ULONGLONG ullAddMultiplier) { m_Result.ullAddMultiplier = ullAddMultiplier; }
	
	inline ULONGLONG GetDivMultiplier(void)                       { return m_Result.ullDivMultiplier; }
	inline void      SetDivMultiplier(ULONGLONG ullDivMultiplier) { m_Result.ullDivMultiplier = ullDivMultiplier; }

	virtual byte* ToBytes (void) { return GetPacketFromHeader(&m_Result); };

protected:
	virtual std::map<std::string,std::string> GetColumnsValues(void);

private:
	SSBC_CPUMEASURE_TESTRESULT_PACKET m_Result;
};

