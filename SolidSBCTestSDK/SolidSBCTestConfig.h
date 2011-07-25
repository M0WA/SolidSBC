#pragma once

#include "SolidSBCTestSDK.h"

class SOLIDSBCTESTSDK_API CSolidSBCXMLFile;
class SOLIDSBCTESTSDK_API CSolidSBCTestConfig
{
protected:
	CSolidSBCTestConfig(const CString& strTestname, const CString& strXmlFile);
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
	CString GenerateEmptyXML(void);

protected:
	//register your attributes
	//by calling RegisterXPathByAttributeName for each attribute
	void RegisterXPathByAttributeName(const CString& strAttributeName);

private:
	CSolidSBCXMLFile* m_pXmlFile;
	CString m_strTestname;
	std::map<CString,CString> m_mapAttributeXPaths;
};

