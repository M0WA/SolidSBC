#include "StdAfx.h"
#include "SolidSBCPacketConfigResponse.h"


CSolidSBCPacketConfigResponse::CSolidSBCPacketConfigResponse(const CString& sXml)
: CSolidSBCPacket()
{
	ParseXml(sXml);
}

CSolidSBCPacketConfigResponse::CSolidSBCPacketConfigResponse(const PBYTE pPacket)
: CSolidSBCPacket(pPacket)
{
}

CSolidSBCPacketConfigResponse::~CSolidSBCPacketConfigResponse(void)
{
}