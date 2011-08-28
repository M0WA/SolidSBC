#include "StdAfx.h"
#include "SolidSBCPacket.h"

CSolidSBCPacket::CSolidSBCPacket(void)
: CSolidSBCXMLFile(_T(""))
, m_sPacketXml(_T(""))
{
}

CSolidSBCPacket::CSolidSBCPacket(const PBYTE pPacket)
: CSolidSBCXMLFile(_T(""))
, m_sPacketXml(_T(""))
{
  	SetPacketBytes(pPacket);
}

CSolidSBCPacket::~CSolidSBCPacket(void)
{
}

int CSolidSBCPacket::GetPacketBytes_Intern(std::vector<byte>& vecPacketBytes)
{
	CStringW sXml = L"";
	
#ifdef _UNICODE
	sXml = m_sPacketXml;
#elif
	USES_CONVERSION;
	sXml = T2A(m_sPacketXml);
#endif
	//calculate sizes
	int nPayloadSize = sizeof(wchar_t) * sXml.GetLength();
	int nHeaderSize  = sizeof(SSBC_PACKET_HEADER);
	int nPacketSize  = nHeaderSize + nPayloadSize;
	vecPacketBytes.reserve(nPacketSize);

	//generate header
	SSBC_PACKET_HEADER header;
	memset(&header,0,nHeaderSize);
	header.nPacketSize = (UINT)nPacketSize;
	header.nType       = GetPacketType();

	//set header
	PBYTE pHeader = (PBYTE)&header;
	for(int i=0; i < nHeaderSize; i++)
		vecPacketBytes.push_back(pHeader[i]);

	//set payload
	PBYTE pPacketByte = (PBYTE)sXml.GetString();
	for(int i=0; i < nPayloadSize; i++)
		vecPacketBytes.push_back(pPacketByte[i]);

	return nPacketSize;
}

int CSolidSBCPacket::SetPacketBytes(const PBYTE pPacketBytes)
{
	int nHeaderSize     = (int)sizeof(SSBC_PACKET_HEADER);
	wchar_t* pPacketXml = (wchar_t*)&pPacketBytes[nHeaderSize];
	int nBytesSet       = (int)wcslen(pPacketXml);

	//TODO: this is very dangerous...
#ifdef _UNICODE
	m_sPacketXml = pPacketXml;
#elif
	USES_CONVERSION;
	char* pcszPacketBytes = W2A((wchar_t*)pPacketXml);
	m_sPacketXml = pcszPacketBytes;
#endif

	return nBytesSet ? ParseXml(m_sPacketXml) : 0;
}

bool CSolidSBCPacket::ParseXml(const CString& sPacketXml)
{
	m_sPacketXml = sPacketXml;
	return SetXmlString(m_sPacketXml);
}

PBYTE CSolidSBCPacket::GetPacketBytes_Intern(int& nPacketSize)
{
	std::vector<byte> vecPacketBytes;
	GetPacketBytes(vecPacketBytes);
	nPacketSize = (int)vecPacketBytes.size();
	
	PBYTE pPacketBytes = new BYTE[nPacketSize];
	for (int i = 0; i < nPacketSize; i++)
		pPacketBytes[i] = vecPacketBytes[i];

	return pPacketBytes;
}

SSBC_PACKET_TYPE CSolidSBCPacket::GetPacketType(void)
{
	if( (m_sPacketXml.Find(L"<TestResult>") != -1) && (m_sPacketXml.Find(L"</TestResult>") != -1) )
		return SSBC_PACKET_TYPE_RESULT;
	else if( (m_sPacketXml.Find(L"<ResultRequest>") != -1) && (m_sPacketXml.Find(L"</ResultRequest>") != -1) )
		return SSBC_PACKET_TYPE_RESULT_REQUEST;
	else if( (m_sPacketXml.Find(L"<ResultResponse>") != -1) && (m_sPacketXml.Find(L"</ResultResponse>") != -1) )
		return SSBC_PACKET_TYPE_RESULT_RESPONSE;
	else if( (m_sPacketXml.Find(L"<ConfigRequest>") != -1) && (m_sPacketXml.Find(L"</ConfigRequest>") != -1) )
		return SSBC_PACKET_TYPE_CONFIG_REQUEST;
	else if( (m_sPacketXml.Find(L"<ConfigResponse>") != -1) && (m_sPacketXml.Find(L"</ConfigResponse>") != -1) )
		return SSBC_PACKET_TYPE_CONFIG_RESPONSE;
	else if( (m_sPacketXml.Find(L"<TestConfig>") != -1) && (m_sPacketXml.Find(L"</TestConfig>") != -1) )
		return SSBC_PACKET_TYPE_CONFIG;
	else
		return SSBC_PACKET_TYPE_UNKNOWN;
}