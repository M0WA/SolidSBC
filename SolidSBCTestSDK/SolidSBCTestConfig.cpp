#include "StdAfx.h"
#include "SolidSBCTestConfig.h"

CSolidSBCTestConfig::CSolidSBCTestConfig(const CString& strTestname, const CString& strXmlFile)
{
	m_pXmlFile = new CSolidSBCXMLFile(strXmlFile);

	m_strTestname = strTestname;

	//add standard attributes
	RegisterXPathByAttributeName(_T("TestName"));
}

CSolidSBCTestConfig::~CSolidSBCTestConfig(void)
{
	delete m_pXmlFile;
	m_pXmlFile = NULL;
}

void CSolidSBCTestConfig::RegisterXPathByAttributeName(const CString& strAttributeName)
{
	CString sXPath = m_strTestname + _T("/") + strAttributeName +_T("[1]");
	m_mapAttributeXPaths[strAttributeName] = sXPath;
}

CString CSolidSBCTestConfig::GenerateEmptyXML(void)
{
	CString strAttributesXML;
	std::map<CString,CString>::iterator iIter = m_mapAttributeXPaths.begin();
	for(;iIter != m_mapAttributeXPaths.end(); iIter++)
	{
		CString strAttributeDefaultValue = _T("INSERT VALUE HERE");
		if ( !(*iIter).first.Compare(_T("TestName")) )
			strAttributeDefaultValue = m_strTestname;

		CString strSingleAttribute;
		strSingleAttribute.Format( 
			_T( "\t<%s>\r\n" )
			_T( "\t\t%s\r\n" )
			_T( "\t</%s>\r\n" )
			, (*iIter).first
			, strAttributeDefaultValue
			, (*iIter).first
		);
		strAttributesXML.Append(strSingleAttribute);
	}

	CString strGeneratedXML;
	strGeneratedXML.Format(
		_T( "<Test>\r\n" )
		_T( "%s" )
		_T( "</Test>"	)
		, strAttributesXML
	);

	return strGeneratedXML;
}