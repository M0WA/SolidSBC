#include "StdAfx.h"
#include "SolidSBCPacketTestResult.h"

CSolidSBCPacketTestResult::CSolidSBCPacketTestResult(CSolidSBCTestResult* pResult)
: CSolidSBCPacket()
, m_sResultSQL(_T(""))
{
	USES_CONVERSION;
	CString sXmlPacket = _T("");

	sXmlPacket.Format(
		_T("<TestResult>\n")
		_T("\t%s")
		_T("</TestResult>")
		, A2T(pResult->ToSQL().c_str())
		);

	ParseXml(sXmlPacket);
}

CSolidSBCPacketTestResult::CSolidSBCPacketTestResult(const PBYTE pPacket)
: CSolidSBCPacket(pPacket)
, m_sResultSQL(_T(""))
{
	USES_CONVERSION;
	std::string sResult;
	if ( GetNodeValue<std::string&>( _T("TestResult[1]"), sResult ) )
		m_sResultSQL = A2T(sResult.c_str());
}

CSolidSBCPacketTestResult::~CSolidSBCPacketTestResult(void)
{
}