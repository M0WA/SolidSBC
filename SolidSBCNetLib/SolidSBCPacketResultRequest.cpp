#include "StdAfx.h"
#include "SolidSBCPacketResultRequest.h"


CSolidSBCPacketResultRequest::CSolidSBCPacketResultRequest(const CString& strComputerName,const CString& strClientUUID)
: CSolidSBCPacket()
, m_sClientUUID(strClientUUID)
, m_sComputerName(strComputerName)
{
	CString sPacketXml;
	sPacketXml.Format (
	_T("<ResultRequest>\n")
	_T("\t<ClientUUID>\n")
		_T("\t\t%s\n")
	_T("\t</ClientUUID>\n")
	_T("\t<ComputerName>\n")
		_T("\t\t%s\n")
	_T("\t</ComputerName>\n")
	_T("</ResultRequest>")
	, m_sClientUUID
	, m_sComputerName
	);
	ParseXml(sPacketXml);
}

CSolidSBCPacketResultRequest::CSolidSBCPacketResultRequest(const PBYTE pRawPacket)
: CSolidSBCPacket(pRawPacket)
{
	USES_CONVERSION;
	std::string sClientUUID, sComputerName;
	m_sClientUUID   = A2T(GetNodeValue<std::string>(_T("ResultRequest/ClientUUID[1]")  , sClientUUID   ) ? sClientUUID.c_str()   : "" );
	m_sComputerName = A2T(GetNodeValue<std::string>(_T("ResultRequest/ComputerName[1]"), sComputerName ) ? sComputerName.c_str() : "" );
}

CSolidSBCPacketResultRequest::~CSolidSBCPacketResultRequest(void)
{
}