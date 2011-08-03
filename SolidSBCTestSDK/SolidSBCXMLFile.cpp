#include "StdAfx.h"
#include "SolidSBCXMLFile.h"

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUpXML; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUpXML; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

CSolidSBCXMLFile::CSolidSBCXMLFile(const CString& strXml)
: m_pXMLDom(NULL)
, m_strXml(strXml)
{
	Init();
}

CSolidSBCXMLFile::~CSolidSBCXMLFile(void)
{
	Clear();
}

int CSolidSBCXMLFile::Init(void)
{
	HRESULT hr = CoInitialize(NULL);
    if(SUCCEEDED(hr))
		CreateAndInitDOM();
    loadDOM();
	return 0;
}

int CSolidSBCXMLFile::Clear(void)
{
    SAFE_RELEASE(m_pXMLDom);
	CoUninitialize();
	m_pXMLDom = NULL;
	return 0;
}

// Helper function to create a DOM instance. 
HRESULT CSolidSBCXMLFile::CreateAndInitDOM(void)
{
	IXMLDOMDocument **ppDoc = &m_pXMLDom;
    HRESULT hr = CoCreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
    if (SUCCEEDED(hr))
    {
        // these methods should not fail so don't inspect result
        (*ppDoc)->put_async(VARIANT_FALSE);  
        (*ppDoc)->put_validateOnParse(VARIANT_FALSE);
        (*ppDoc)->put_resolveExternals(VARIANT_FALSE);
    }
    return hr;
}


void CSolidSBCXMLFile::loadDOM(void)
{
    HRESULT hr = S_OK;
    IXMLDOMParseError *pXMLErr = NULL;

    BSTR bstrErr = NULL;
    VARIANT_BOOL varStatus;
	BSTR varXml = m_strXml.AllocSysString();
	m_pXMLDom->loadXML(varXml, &varStatus);
	
	if (varStatus != VARIANT_TRUE) {
		CHK_HR(m_pXMLDom->get_parseError(&pXMLErr));
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		SAFE_RELEASE(m_pXMLDom);}

CleanUpXML:
    SAFE_RELEASE(pXMLErr);
    SysFreeString(bstrErr);
    SysFreeString(varXml);
}

int CSolidSBCXMLFile::GetNodeString(const CString& strXPath, CString& strValue)
{
	IXMLDOMNode *pNode = NULL;
	BSTR bstrXPath = strXPath.AllocSysString();
    HRESULT hr = m_pXMLDom->selectSingleNode(bstrXPath, &pNode);
    SysFreeString(bstrXPath);
	if (hr != S_OK){
		return 1;}

	BSTR bstrValue;
	hr = pNode->get_text(&bstrValue);
	if (hr != S_OK){  
		SAFE_RELEASE(pNode);
		return 2;}

	strValue = bstrValue;
    SysFreeString(bstrValue);
	SAFE_RELEASE(pNode);
	return 0;
}

int CSolidSBCXMLFile::Save(const CString& strFileName)
{
	HRESULT hr = m_pXMLDom->save(CComVariant(strFileName));
	if (hr != S_OK){
		return 1;}
	return 0;
}

int CSolidSBCXMLFile::SetNodeString(const CString& strXPath, const CString& strValue)
{
	IXMLDOMNode *pNode = NULL;
	
	BSTR bstrXPath = strXPath.AllocSysString();
	if (  (m_pXMLDom->selectSingleNode(bstrXPath, &pNode) != S_OK) ){
		
		if ( (m_pXMLDom->createTextNode(bstrXPath, (IXMLDOMText**)&pNode)) != S_OK ){
			return 1;}
		else {
			return 2;}
		
		SysFreeString(bstrXPath);
		return 3;
	}

	BSTR bstrValue = strValue.AllocSysString();
	HRESULT hr = pNode->put_text(bstrValue);
	if (hr != S_OK){ 		
		SAFE_RELEASE(pNode);
		return 4;}
    SysFreeString(bstrValue);

	SAFE_RELEASE(pNode);
	return 0;
}

bool CSolidSBCXMLFile::Validate(const CString& strXSDFile)
{	
	return true;
}

bool CSolidSBCXMLFile::SetXmlString(const CString& strXml)
{
	Clear();

	m_strXml = strXml;
	Init();

	return true;
}