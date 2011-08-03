#pragma once

#include "stdafx.h"

typedef enum {
	//not used
	SSBC_PACKET_TYPE_UNKNOWN         = 0,

	//config packets
	SSBC_PACKET_TYPE_CONFIG_REQUEST   = 1,
	SSBC_PACKET_TYPE_CONFIG_RESPONSE  = 2,
	SSBC_PACKET_TYPE_CONFIG           = 3,

	//result packets
	SSBC_PACKET_TYPE_RESULT_REQUEST   = 4,
	SSBC_PACKET_TYPE_RESULT_RESPONSE  = 5,
	SSBC_PACKET_TYPE_RESULT           = 6,

	//end marker for this typedef
	SSBC_PACKET_TYPE_MAX

} SSBC_PACKET_TYPE,*PSSBC_PACKET_TYPE;

typedef struct {

	void SSBC_PACKET_HEADER(void)
	{
		memset(this,0,sizeof(this));
	}

	UINT             nPacketSize;
	SSBC_PACKET_TYPE nType;
	UINT             nReserved;
	UINT             nCheckSum;
} SSBC_PACKET_HEADER, *PSSBC_PACKET_HEADER;

#define SSBC_PACKET_VIRTUAL_INCLUDE \
	virtual int              GetPacketBytes(std::vector<byte>& vecPacketBytes) { return CSolidSBCPacket::GetPacketBytes_Intern(vecPacketBytes);} \
	virtual PBYTE            GetPacketBytes(int& nPacketSize)                  { return CSolidSBCPacket::GetPacketBytes_Intern(nPacketSize);}