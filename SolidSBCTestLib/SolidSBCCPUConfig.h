#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCCPUConfig : public CSolidSBCTestConfig
{
public:
	CSolidSBCCPUConfig(void);
	~CSolidSBCCPUConfig(void);
	
	//getter
	BOOL   GetRandomize(void);
	UINT   GetMaxRand(void);
	double GetSleepMS(void);
	UINT   GetThreadCnt(void);
	
	//setter
	inline BOOL SetDivisionCnt(const BOOL& bRandomize)
		{ return SetAttributeByName<BOOL>(_T("Randomize"), bRandomize); }

	inline BOOL SetMaxRand(const UINT& nMaxRand)
		{ return SetAttributeByName<UINT>(_T("MaxRand"), nMaxRand); }
	
	inline BOOL SetSleepMS(const double& dSleepMS)
		{ return SetAttributeByName<double>(_T("SleepMS"), dSleepMS); }

	inline BOOL SetThreadCnt(const UINT& nThreadCnt)
		{ return SetAttributeByName<UINT>(_T("ThreadCnt"), nThreadCnt); }

protected:
	void RegisterAttributes(void);
};

