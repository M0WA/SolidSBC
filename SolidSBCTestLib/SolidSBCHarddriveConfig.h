#pragma once

#include <SolidSBCTestSDK.h>

class CSolidSBCHarddriveConfig : CSolidSBCTestConfig
{
public:
	CSolidSBCHarddriveConfig(void);
	~CSolidSBCHarddriveConfig(void);

	//getter
	BOOL  GetRandomRead(void);
	BOOL  GetRandomWrite(void);
	ULONG GetReadMax(void);
	ULONG GetWriteMax(void);
	UINT  GetReadWriteDelay(void);
	BOOL  GetTransmitData(void);

	//setter
	inline BOOL SetRandomRead(const BOOL& bRandomRead)
		{ return SetAttributeByName<BOOL>(_T("RandomRead"), bRandomRead); }

	inline BOOL SetRandomWrite(const BOOL& bRandomWrite)
		{ return SetAttributeByName<BOOL>(_T("RandomWrite"), bRandomWrite); }

	inline BOOL SetReadMax(const ULONG& nReadMax)
		{ return SetAttributeByName<ULONG>(_T("ReadMax"), nReadMax); }

	inline BOOL SetWriteMax(const ULONG& nWriteMax)
		{ return SetAttributeByName<ULONG>(_T("WriteMax"), nWriteMax); }
	
	inline BOOL SetReadWriteDelay(const UINT& nReadWriteDelay)
		{ return SetAttributeByName<ULONG>(_T("ReadWriteDelay"),nReadWriteDelay ); }

	inline BOOL SetTransmitData(const BOOL& bTransmitData)
		{ return SetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData); }

protected:
	virtual void RegisterAttributes(void);
};

