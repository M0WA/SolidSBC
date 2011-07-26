#pragma once

#include <SolidSBCTestSDK.h>

#define SSBC_PROFILE_MAX_SERVER_NAME	512

class CSolidSBCNetPingConfig : public CSolidSBCTestConfig
{
public:
	CSolidSBCNetPingConfig(const CString& strXml = _T(""));
	~CSolidSBCNetPingConfig(void);

	//getter
	UINT    GetInterval(void);
	CString GetHost(void);
	UINT    GetTTL(void);
	UINT    GetPayloadSize(void);
	BOOL    GetTransmitData(void);
	
	
	inline BOOL SetInterval(const UINT& nInterval)
		{ return SetAttributeByName<UINT>(_T("Interval"), nInterval); }
	
	/*
	CString strHost;
	*/

	inline BOOL SetTTL(const UINT& nTTL)
		{ return SetAttributeByName<UINT>(_T("TTL"), nTTL); }

	inline BOOL SetPayloadSize(const UINT& nPayloadSize)
		{ return SetAttributeByName<UINT>(_T("PayloadSize"), nPayloadSize); }

	inline BOOL SetTransmitData(const BOOL& bTransmitData)
		{ return SetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData); }

protected:
	void RegisterAttributes(void);
};

