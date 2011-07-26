#pragma once

#include "SolidSBCTestSDK.h"

class SOLIDSBCTESTSDK_API CSolidSBCXMLFile;
class SOLIDSBCTESTSDK_API CSolidSBCTestConfig
{
protected:
	CSolidSBCTestConfig(const CString& strTestname, const CString& strXml);
public:
	~CSolidSBCTestConfig(void);

	template<class T> bool GetAttributeByName(const CString& strAttributeName, T& value)
	{
		return m_pXmlFile->GetNodeValue<T>( m_mapAttributeXPaths[strAttributeName], value );
	}

	template<class T> bool SetAttributeByName(const CString& strAttributeName, const T& value)
	{
		return m_pXmlFile->SetNodeValue<T>( m_mapAttributeXPaths[strAttributeName], value );
	}

	inline CString GetTestName(void) { return m_strTestname; };
	CString GenerateXML(void);

	static CString GetTestNameFromXML(const CString& strXml);

protected:
	//register your attributes
	//by calling RegisterXPathByAttributeName for each attribute
	void RegisterAttribute(const CString& strAttributeName, const CString& strAttributeDefault = _T("0"));
	void Init(void);

private:
	void RegisterXPathByAttributeName(const CString& strAttributeName);
	void RegisterDefaultValueByAttributeName(const CString& strAttributeName, const CString& strAttributeDefault);

	CSolidSBCXMLFile*         m_pXmlFile;
	CString                   m_strTestname;
	CString                   m_strXml;
	std::map<CString,CString> m_mapAttributeXPaths;
	std::map<CString,CString> m_mapAttributeDefaults;
};

