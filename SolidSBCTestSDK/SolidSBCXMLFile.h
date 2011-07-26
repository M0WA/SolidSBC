#pragma once

#include "SolidSBCTestSDK.h"
#include <sstream>

class SOLIDSBCTESTSDK_API CSolidSBCXMLFile
{
public:
	CSolidSBCXMLFile(const CString& strXml);
	~CSolidSBCXMLFile(void);

	int  Save(const CString& strFileName);
	bool Validate(const CString& strXSDFile);

	template<class Tvalue> bool GetNodeValue(const CString& strXPath, Tvalue& value)
	{
		CString strValue;
		bool bSuccess = !GetNodeString(strXPath, strValue);
		
		USES_CONVERSION;
		std::string sValue = T2A(strValue);
		if(bSuccess)
		{
			std::stringstream in;
			in << sValue;
			return (in >> value && in.eof());
		}
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
	
protected:
	HRESULT CreateAndInitDOM(void);
	void    loadDOM(void);
	
	int GetNodeString(const CString& strXPath, CString& strValue);
	int SetNodeString(const CString& strXPath, const CString& strValue);

	CString m_strXml;
	IXMLDOMDocument* m_pXMLDom;

private:
	int  Init(void);
};

