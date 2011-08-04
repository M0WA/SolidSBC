#include "StdAfx.h"
#include "SolidSBCPacketConfigRequest.h"


CSolidSBCPacketConfigRequest::CSolidSBCPacketConfigRequest(const CString& sClientUUID, const CString& sComputerName)
: CSolidSBCPacket()
, m_sClientUUID(sClientUUID)
, m_sComputerName(sComputerName)
{
	USES_CONVERSION;

	CString sPacketXml;
	sPacketXml.Format (
	_T("<ConfigRequest>\n")
	_T("\t<ClientUUID>\n")
		_T("\t\t%s\n")
	_T("\t</ClientUUID>\n")
	_T("\t<ComputerName>\n")
		_T("\t\t%s\n")
	_T("\t</ComputerName>\n")
	_T("</ConfigRequest>")
	, m_sClientUUID
	, m_sComputerName
	);
	ParseXml(sPacketXml);
}

CSolidSBCPacketConfigRequest::CSolidSBCPacketConfigRequest(const PBYTE pPacket)
: CSolidSBCPacket(pPacket)
{
	USES_CONVERSION;
	std::string sClientUUID, sComputerName;
	m_sClientUUID   = A2T(GetNodeValue<std::string>(_T("ConfigRequest/ClientUUID[1]")  , sClientUUID   ) ? sClientUUID.c_str()   : "" );
	m_sComputerName = A2T(GetNodeValue<std::string>(_T("ConfigRequest/ComputerName[1]"), sComputerName ) ? sComputerName.c_str() : "" );
}

CSolidSBCPacketConfigRequest::~CSolidSBCPacketConfigRequest(void)
{
}