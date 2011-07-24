#pragma once

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUpXML; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUpXML; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

class CXMLFile
{
public:
	CXMLFile(void);
	~CXMLFile(void);

	int Init(CString strFileName);
	int Save(CString strFileName);
	int SetNodeString(CString strXPath, CString strValue);
	int SetNodeInt(CString strXPath, int nValue);
	int SetNodeFloat(CString strXPath, double dValue);
	int GetNodeInt(CString strXPath, LPINT pnValue);
	int GetNodeFloat(CString strXPath, double* pfValue);
	int GetNodeString(CString strXPath, CString* pstrValue);

protected:
	HRESULT CreateAndInitDOM(void);
	void    loadDOM();
	CString m_strFileName;
	IXMLDOMDocument* m_pXMLDom;
private:
	IXMLDOMNode* FindNodeByName(CString strName);
};
