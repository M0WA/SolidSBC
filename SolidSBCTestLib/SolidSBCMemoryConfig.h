#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCMemoryConfig : CSolidSBCTestConfig
{
public:
	CSolidSBCMemoryConfig(void);
	~CSolidSBCMemoryConfig(void);

	//getter
	BOOL GetRandomize(void);
	UINT GetMinMem(void);
	UINT GetMaxMem(void);
	BOOL GetTransmitData(void);

	//setter
	inline BOOL SetRandomize(const BOOL& bRandomize)
		{ return SetAttributeByName<BOOL>(_T("Randomize"), bRandomize); }
	
	inline BOOL SetMinMem(const UINT& nMinMem)
		{ return SetAttributeByName<UINT>(_T("MinMem"), nMinMem); }

	inline BOOL SetMaxMem(const UINT& nMaxMem)
		{ return SetAttributeByName<UINT>(_T("MaxMem"), nMaxMem); }

	inline BOOL SetTransmitData(const BOOL& bTransmitData)
		{ return SetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData); }

protected:
	virtual void RegisterAttributes(void);
};

