#include "StdAfx.h"
#include "SolidSBCTestConfig.h"

CSolidSBCTestConfig::CSolidSBCTestConfig(const CString& strTestname, const CString& strXmlFile)
{
	m_pXmlFile = new CSolidSBCXMLFile(strXmlFile);

	m_strTestname = strTestname;
	RegisterAttributes(m_mapAttributeXPaths);

	//add standard attributes
	m_mapAttributeXPaths[_T("TestName")] = GetXPathFromNodeName(_T("TestName"));
}

CSolidSBCTestConfig::~CSolidSBCTestConfig(void)
{
	delete m_pXmlFile;
	m_pXmlFile = NULL;
}

CString CSolidSBCTestConfig::GetXPathFromNodeName(const CString& strNodeName)
{
	return m_strTestname + _T("/") + strNodeName +_T("[1]");
}