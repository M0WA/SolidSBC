#include "StdAfx.h"
#include "SolidSBCTestConfig.h"

CSolidSBCTestConfig::CSolidSBCTestConfig(const CString& strTestname, const CString& strXmlFile)
{
	m_pXmlFile = new CSolidSBCXMLFile(strXmlFile);

	m_strTestname = strTestname;
	RegisterAttributes();

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