#pragma once

#include "SolidSBCTestSDK.h"
#include <sstream>

using namespace std;

class SOLIDSBCTESTSDK_API CSolidSBCXMLFile
{
public:
	CSolidSBCXMLFile(const CString& strXml);
	~CSolidSBCXMLFile(void);

	int  Save(const CString& strFileName);
	bool Validate(const CString& strXSDFile);
	bool SetXmlString(const CString& strXml);

	template<class Tvalue> bool GetNodeValue(const CString& strXPath, Tvalue& value)
	{
		CString strValue;
		bool bSuccess = !GetNodeString(strXPath, strValue);
		
		USES_CONVERSION;
		std::string sValue = T2A(strValue);
		if(bSuccess)
		{
			std::stringstream in;
			in << std::skipws;
			in << sValue;
			in >> value;
			return !in.bad();
		}
		return bSuccess;
	}

	template <class string> bool GetNodeValue(const CString strXPath, std::string& value)
	{
		bool bSuccess = false;
		CString strValue;
		bSuccess = !GetNodeString(strXPath, strValue);

		USES_CONVERSION;
		value = T2A(strValue);
		return bSuccess;
	}

	template<class Tvalue> bool SetNodeValue(const CString& strXPath, const Tvalue& value)
	{
		CString strValue;
		std::stringstream sStream;
		sStream << value;

		bool bSuccess = !SetNodeString(strXPath, CString(sStream.str().c_str()));
		return bSuccess;
	}

private:
	int  Init(void);
	int  Clear(void);

	HRESULT CreateAndInitDOM(void);
	void    loadDOM(void);
	
	int GetNodeString(const CString& strXPath, CString& strValue);
	int SetNodeString(const CString& strXPath, const CString& strValue);

	CString          m_strXml;
	IXMLDOMDocument* m_pXMLDom;
};

