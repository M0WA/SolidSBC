#pragma once

#include "SolidSBCNetLib.h"

class SOLIDSBCNETLIB_API CSolidSBCPacketTestResult : public CSolidSBCPacket
{
public:
	CSolidSBCPacketTestResult(CSolidSBCTestResult* pResult);
	CSolidSBCPacketTestResult(const PBYTE pRawPacket);
	~CSolidSBCPacketTestResult(void);
	
	void GetResultSQL(CString& sResultSQL){ sResultSQL = m_sResultSQL;   }

	SSBC_PACKET_VIRTUAL_INCLUDE

private:
	CString m_sResultSQL;
};

