#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCCPUMeasureConfig : public CSolidSBCTestConfig
{
public:
	CSolidSBCCPUMeasureConfig(const CString& strXml = _T(""));
	~CSolidSBCCPUMeasureConfig(void);
	
	//getter
	UINT   GetDivisionCnt(void);
	UINT   GetAdditionCnt(void);
	BOOL   GetTransmitData(void);
	UINT   GetAutoFixMultiplier(void);
	double GetFixMultiplierTreshold(void);
	
	//setter
	inline BOOL SetDivisionCnt(const UINT& nDivisionCnt)
		{ return SetAttributeByName<UINT>(_T("DivisionCnt"), nDivisionCnt); }

	inline BOOL SetAdditionCnt(const UINT& nAdditionCnt)
		{ return SetAttributeByName<UINT>(_T("AdditionCnt"), nAdditionCnt); }

	inline BOOL SetTransmitData(const BOOL& bTransmitData)
		{ return SetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData); }

	inline BOOL SetAutoFixMultiplier(const UINT& nAutoFixMultiplier)
		{ return SetAttributeByName<UINT>(_T("AutoFixMultiplier"), nAutoFixMultiplier); }

	inline BOOL SetFixMultiplierTreshold(const double& dFixMultiplierTreshold) 
		{ return SetAttributeByName<double>(_T("FixMultiplierTreshold"), dFixMultiplierTreshold); }

protected:
	void RegisterAttributes(void);
};

