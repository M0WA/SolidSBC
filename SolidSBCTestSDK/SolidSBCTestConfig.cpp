#include "StdAfx.h"
#include "SolidSBCTestConfig.h"

CSolidSBCTestConfig::CSolidSBCTestConfig(const CString& strTestname, const CString& strXml)
: m_pXmlFile(NULL)
, m_strXml(strXml)
, m_strTestname(strTestname)
{
	//add standard attributes
	RegisterAttribute(_T("TestName"),m_strTestname);
}

CSolidSBCTestConfig::~CSolidSBCTestConfig(void)
{
	delete m_pXmlFile;
	m_pXmlFile = NULL;
}

void CSolidSBCTestConfig::RegisterAttribute(const CString& strAttributeName, const CString& strAttributeDefault)
{
	RegisterXPathByAttributeName(strAttributeName);
	RegisterDefaultValueByAttributeName(strAttributeName, strAttributeDefault);
}

void CSolidSBCTestConfig::RegisterXPathByAttributeName(const CString& strAttributeName)
{
	CString sXPath = _T("Test/") + strAttributeName +_T("[1]");
	m_mapAttributeXPaths[strAttributeName] = sXPath;
}

void CSolidSBCTestConfig::RegisterDefaultValueByAttributeName(const CString& strAttributeName, const CString& strAttributeDefault)
{
	m_mapAttributeDefaults[strAttributeName] = strAttributeDefault;
}

CString CSolidSBCTestConfig::GenerateXML(void)
{
	CString strAttributesXML;
	std::map<CString,CString>::iterator iIter = m_mapAttributeXPaths.begin();
	for(;iIter != m_mapAttributeXPaths.end(); iIter++)
	{
		CString strAttributeDefaultValue = m_mapAttributeDefaults[(*iIter).first];

		CString strSingleAttribute;
		strSingleAttribute.Format( 
			_T( "\t<%s>\r\n" )
			_T( "\t\t%s\r\n" )
			_T( "\t</%s>\r\n\r\n" )
			, (*iIter).first
			, strAttributeDefaultValue
			, (*iIter).first
		);
		strAttributesXML.Append(strSingleAttribute);
	}

	CString strGeneratedXML;
	strGeneratedXML.Format(
		_T( "<Test>\r\n\r\n" )
		_T( "%s" )
		_T( "</Test>"	)
		, strAttributesXML
	);

	return strGeneratedXML;
}

CString CSolidSBCTestConfig::GetTestNameFromXML(const CString& strXml)
{
	USES_CONVERSION;
	CSolidSBCXMLFile cXML(strXml);
	std::string sTestname = "";
	if ( !cXML.GetNodeValue<std::string>(_T("Test/TestName[1]"), sTestname) )
		return _T("");
	return CString(A2T(sTestname.c_str()));
}

void CSolidSBCTestConfig::Init(void)
{	
	if(m_pXmlFile)
		delete m_pXmlFile;

	if ( m_strXml != _T("") ) {
		m_pXmlFile = new CSolidSBCXMLFile(m_strXml); }
	else {
		m_pXmlFile = new CSolidSBCXMLFile(GenerateXML()); }
}