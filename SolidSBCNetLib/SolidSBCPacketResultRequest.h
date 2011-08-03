#pragma once

#include "SolidSBCNetLib.h"

class SOLIDSBCNETLIB_API CSolidSBCPacketResultRequest : public CSolidSBCPacket
{
public:
	CSolidSBCPacketResultRequest(const CString& strComputerName,const CString& strClientUUID);
	CSolidSBCPacketResultRequest(const PBYTE pRawPacket);
	~CSolidSBCPacketResultRequest(void);
	
	void GetClientUUID(CString& sClientUUID)   { sClientUUID   = m_sClientUUID;   }
	void GetClientName(CString& sComputerName) { sComputerName = m_sComputerName; }

	SSBC_PACKET_VIRTUAL_INCLUDE

private:
	CString m_sClientUUID;
	CString m_sComputerName;

};

