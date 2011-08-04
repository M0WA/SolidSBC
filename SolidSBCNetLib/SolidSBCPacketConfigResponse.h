#pragma once

#include "SolidSBCNetLib.h"

class SOLIDSBCNETLIB_API CSolidSBCPacketConfigResponse : public CSolidSBCPacket
{
public:
	CSolidSBCPacketConfigResponse(const CString& sXml);
	CSolidSBCPacketConfigResponse(const PBYTE pPacket);
	~CSolidSBCPacketConfigResponse(void);
	
	SSBC_PACKET_VIRTUAL_INCLUDE
};

