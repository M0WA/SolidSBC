#pragma once

#include <vector>
#include <SolidSBCXMLFile.h>
#include "SolidSBCPacketDefinitions.h"

class CSolidSBCPacket : public CSolidSBCXMLFile
{
protected:
	CSolidSBCPacket(void);
	CSolidSBCPacket(const PBYTE pPacket);
public:
	~CSolidSBCPacket(void);

	virtual int     GetPacketBytes(std::vector<byte>& vecPacketBytes)=0;
	virtual PBYTE   GetPacketBytes(int& nPacketSize)=0;

	CString         GetXml(void) { return m_sPacketXml; }

	static PBYTE    ReceivePacket(SOCKET hClientSocket)
	{
		int nHeaderSize = sizeof(SSBC_PACKET_HEADER);
		SSBC_PACKET_HEADER header;
		memset(&header,0,nHeaderSize);
	
		u_long iMode = 0;
		ioctlsocket(hClientSocket, FIONBIO, &iMode);

		//receive header
		int nRead = recv(hClientSocket,(char*)&header,nHeaderSize,0);
		if ( nRead != nHeaderSize )
			return NULL;

		//allocate memory for packet
		int nBufferSize = header.nPacketSize+10;
		if ( nBufferSize < 0 || nBufferSize > 5000 || header.nType <= SSBC_PACKET_TYPE_UNKNOWN || header.nType >= SSBC_PACKET_TYPE_MAX)
			return NULL;

		PBYTE pPacket = new BYTE[nBufferSize];
		ZeroMemory(pPacket,nBufferSize);
		memcpy(pPacket,&header,header.nPacketSize);

		//receive payload
		int nPayloadSize = header.nPacketSize - nHeaderSize;
		nRead = recv(hClientSocket,(char*)&pPacket[nHeaderSize],nPayloadSize,0);
		if ( nRead != nPayloadSize ){
			delete pPacket;
			return NULL;}

		return pPacket;
	}

protected:
	int				SetPacketBytes(const PBYTE pPacketBytes);
	bool			ParseXml(const CString& sPacketXml);
	
	int             GetPacketBytes_Intern(std::vector<byte>& vecPacketBytes);
	PBYTE           GetPacketBytes_Intern(int& nPacketSize);

private:
	SSBC_PACKET_TYPE GetPacketType(void);
	CString m_sPacketXml;
};
