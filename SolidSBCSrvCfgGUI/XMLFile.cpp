#include "StdAfx.h"
#include "XMLFile.h"

CXMLFile::CXMLFile(void)
: m_pXMLDom(NULL)
{
	HRESULT hr = CoInitialize(NULL);
    if(SUCCEEDED(hr))
		CreateAndInitDOM();
}

CXMLFile::~CXMLFile(void)
{
    SAFE_RELEASE(m_pXMLDom);
	CoUninitialize();
}

int CXMLFile::Init(CString strFileName)
{
	m_strFileName = strFileName;
    loadDOM();

	return 0;
}

// Helper function to create a DOM instance. 
HRESULT CXMLFile::CreateAndInitDOM(void)
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


void CXMLFile::loadDOM()
{
    HRESULT hr = S_OK;
    IXMLDOMParseError *pXMLErr = NULL;

    BSTR bstrErr = NULL;
    VARIANT_BOOL varStatus;
    VARIANT varFileName;
    VariantInit(&varFileName);

	COleVariant strVar(m_strFileName);
	varFileName=strVar.Detach();
	m_pXMLDom->load(varFileName, &varStatus);
	if (varStatus != VARIANT_TRUE) {
		CHK_HR(m_pXMLDom->get_parseError(&pXMLErr));
		CHK_HR(pXMLErr->get_reason(&bstrErr));
		SAFE_RELEASE(m_pXMLDom);}

CleanUpXML:
    SAFE_RELEASE(pXMLErr);
    SysFreeString(bstrErr);
    VariantClear(&varFileName);
}


int CXMLFile::GetNodeInt(CString strXPath, LPINT pnValue)
{
	CString strNodeValue;
	int nErr = GetNodeString(strXPath, &strNodeValue);

	if (!nErr)
		*pnValue = _ttoi(strNodeValue);

	return nErr;
}

int CXMLFile::GetNodeFloat(CString strXPath, double* pfValue)
{
	CString strNodeValue;
	int nErr = GetNodeString(strXPath, &strNodeValue);
	
	if (!nErr)
		*pfValue = _tstof(strNodeValue);

	return nErr;
}

int CXMLFile::GetNodeString(CString strXPath, CString* pstrValue)
{
	int nReturn = 0;

	IXMLDOMNode *pNode = NULL;
    HRESULT hr = m_pXMLDom->selectSingleNode(*((BSTR*)&strXPath), &pNode);
	if (hr != S_OK){
		return 1;}

	BSTR bstrValue;
	hr = pNode->get_text(&bstrValue);
	if (hr != S_OK){  
		SAFE_RELEASE(pNode);
		return 2;}

	*pstrValue = bstrValue;
	SAFE_RELEASE(pNode);
	return 0;
}

int CXMLFile::Save(CString strFileName)
{
	HRESULT hr = m_pXMLDom->save(CComVariant(strFileName));
	
	if (hr != S_OK){
		return 1;}

	return 0;
}

int CXMLFile::SetNodeString(CString strXPath, CString strValue)
{
	IXMLDOMNode *pNode = NULL;

	if (  (m_pXMLDom->selectSingleNode(*((BSTR*)&strXPath), &pNode) != S_OK) ){

		IXMLDOMNode *pPortfolioNode = FindNodeByName(_T("portfolio"));
		if ( !pPortfolioNode )
			return 1;

		if ( (m_pXMLDom->createTextNode(*((BSTR*)&strXPath), (IXMLDOMText**)&pNode)) != S_OK ){
			return 2;}
		else {
			return 3;}

		//m_pXMLDom->xxx
		SAFE_RELEASE(pPortfolioNode);

		return 4;
	}

	int nReturn = 0;
	HRESULT hr = pNode->put_text(*((BSTR*)&strValue));
	if (hr != S_OK){ 		
		SAFE_RELEASE(pNode);
		return 5;}

	SAFE_RELEASE(pNode);
	return 0;
}

int CXMLFile::SetNodeInt(CString strXPath, int nValue)
{
	CString strValue = _T("");
	strValue.Format(_T("%d"),nValue);
	return SetNodeString(strXPath, strValue);
}

int CXMLFile::SetNodeFloat(CString strXPath, double dValue)
{
	CString strValue = _T("");
	strValue.Format(_T("%f"),dValue);
	return SetNodeString(strXPath, strValue);
}

IXMLDOMNode* CXMLFile::FindNodeByName(CString strName)
{
	IXMLDOMNodeList* pNodesList = NULL;
	if ((m_pXMLDom->get_childNodes(&pNodesList)) != S_OK){  
		return NULL;}

	long lNodeListLen = 0;
	CString strNodeName;
	if ( pNodesList->get_length(&lNodeListLen) != S_OK ){
		SAFE_RELEASE(pNodesList);
		return NULL;
	}

	for (long i = 0; i < lNodeListLen; i++){
		IXMLDOMNode *pTestNode = NULL;
		if ( ( pNodesList->get_item( i, &pTestNode )       != S_OK )
			||( pTestNode->get_baseName( (BSTR*)&strNodeName ) != S_OK  )
			){
				SAFE_RELEASE(pTestNode);
				SAFE_RELEASE(pNodesList);
				return NULL;}

		if ( strName == strNodeName ){		
				SAFE_RELEASE(pNodesList);
				return pTestNode;
		}
	}

	return NULL;
}
