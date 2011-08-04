#pragma once

#include "SolidSBCNetLib.h"

class SOLIDSBCNETLIB_API CSolidSBCPacketConfigRequest : public CSolidSBCPacket
{
public:
	CSolidSBCPacketConfigRequest(const CString& sClientUUID, const CString& sComputerName);
	CSolidSBCPacketConfigRequest(const PBYTE pPacket);
	~CSolidSBCPacketConfigRequest(void);

	void GetClientUUID(CString& sClientUUID)   { sClientUUID   = m_sClientUUID;   }
	void GetClientName(CString& sComputerName) { sComputerName = m_sComputerName; }
	
	SSBC_PACKET_VIRTUAL_INCLUDE

private:
	CString m_sClientUUID;
	CString m_sComputerName;
};

