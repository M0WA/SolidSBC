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
		, pResult->ToSQL()
		);

	ParseXml(sXmlPacket);
}

CSolidSBCPacketTestResult::CSolidSBCPacketTestResult(const PBYTE pRawPacket)
: CSolidSBCPacket(pRawPacket)
{
	USES_CONVERSION;
	std::string sResultSQL;
	m_sResultSQL = A2T(GetNodeValue<std::string>(_T("TestResult[1]")  , sResultSQL   ) ? sResultSQL.c_str()   : "" );
}

CSolidSBCPacketTestResult::~CSolidSBCPacketTestResult(void)
{
}