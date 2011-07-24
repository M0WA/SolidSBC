#pragma once

#include "SolidSBCTestSDK.h"

class SOLIDSBCTESTSDK_API CSolidSBCXMLFile;
class SOLIDSBCTESTSDK_API CSolidSBCTestConfig
{
protected:
	CSolidSBCTestConfig(const CString& strTestname, const CString& strXmlFile);
	~CSolidSBCTestConfig(void);

public:
	template<class T> bool GetAttributeByName(const CString& strAttributeName, T& value)
	{
		return m_pXmlFile->GetNodeValue<T>( m_mapAttributeXPaths[strAttributeName], value );
	}

	template<class T> bool SetAttributeByName(const CString& strAttributeName, const T& value)
	{
		return m_pXmlFile->SetNodeValue<T>( m_mapAttributeXPaths[strAttributeName], value );
	}

protected:
	//override this function to register your attributes
	virtual void RegisterAttributes(std::map<CString,CString>& mapAttributeXPaths) {};

	CString      GetXPathFromNodeName(const CString& strNodeName);

private:
	CSolidSBCXMLFile* m_pXmlFile;
	CString m_strTestname;
	std::map<CString,CString> m_mapAttributeXPaths;
};

