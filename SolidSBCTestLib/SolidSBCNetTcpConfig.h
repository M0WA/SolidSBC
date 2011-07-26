#pragma once

#include <SolidSBCTestSDK.h>

#define SSBC_PROFILE_MAX_SERVER_NAME	512

class CSolidSBCNetTcpConfig : public CSolidSBCTestConfig
{
public:
	CSolidSBCNetTcpConfig(const CString& strXml = _T(""));
	~CSolidSBCNetTcpConfig(void);

	//getter
	UINT    GetInterval(void);
	CString GetHost(void);
	USHORT  GetPort(void);
	UINT    GetTTL(void);
	BOOL    GetTransmitData(void);
		
	//setter
	inline BOOL SetInterval(const UINT& nInterval)
		{ return SetAttributeByName<UINT>(_T("Interval"), nInterval); }
	
	/*
	CString strHost;
	*/

	inline BOOL SetPort(const USHORT& nPort)
		{ return SetAttributeByName<USHORT>(_T("Port"), nPort); }

	inline BOOL SetTTL(const UINT& nTTL)
		{ return SetAttributeByName<UINT>(_T("TTL"), nTTL); }

	inline BOOL SetTransmitData(const BOOL& bTransmitData)
		{ return SetAttributeByName<BOOL>(_T("TransmitData"), bTransmitData); }

protected:
	void RegisterAttributes(void);
};

