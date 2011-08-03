#pragma once

#include <vector>
#include <SolidSBCXMLFile.h>
#include "SolidSBCPacketDefinitions.h"

class CSolidSBCPacket : public CSolidSBCXMLFile
{
protected:
	CSolidSBCPacket(void);
	CSolidSBCPacket(const PBYTE pPacketBytes);
public:
	~CSolidSBCPacket(void);

	virtual int              GetPacketBytes(std::vector<byte>& vecPacketBytes)=0;
	virtual PBYTE            GetPacketBytes(int& nPacketSize)=0;

protected:
	int   SetPacketBytes(const PBYTE pPacketBytes);
	bool  ParseXml(const CString& sPacketXml);
	
	int              GetPacketBytes_Intern(std::vector<byte>& vecPacketBytes);
	PBYTE            GetPacketBytes_Intern(int& nPacketSize);

private:
	SSBC_PACKET_TYPE GetPacketType(void);
	CString m_sPacketXml;
};
