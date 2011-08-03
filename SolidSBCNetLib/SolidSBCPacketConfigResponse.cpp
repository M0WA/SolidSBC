#include "StdAfx.h"
#include "SolidSBCPacketConfigResponse.h"


CSolidSBCPacketConfigResponse::CSolidSBCPacketConfigResponse(const CString& sXml)
: CSolidSBCPacket()
{
	SetXmlString(sXml);
}

CSolidSBCPacketConfigResponse::CSolidSBCPacketConfigResponse(const PBYTE pRawPacket)
: CSolidSBCPacket(pRawPacket)
{
}

CSolidSBCPacketConfigResponse::~CSolidSBCPacketConfigResponse(void)
{
}