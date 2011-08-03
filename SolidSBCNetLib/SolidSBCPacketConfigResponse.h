#pragma once

#include "SolidSBCNetLib.h"

class SOLIDSBCNETLIB_API CSolidSBCPacketConfigResponse : public CSolidSBCPacket
{
public:
	CSolidSBCPacketConfigResponse(const CString& sXml);
	CSolidSBCPacketConfigResponse(const PBYTE pRawPacket);
	~CSolidSBCPacketConfigResponse(void);
	
	SSBC_PACKET_VIRTUAL_INCLUDE
};

