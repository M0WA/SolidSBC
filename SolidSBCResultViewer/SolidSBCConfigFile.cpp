#include "StdAfx.h"
#include "SolidSBCConfigFile.h"

CSolidSBCConfigFile::CSolidSBCConfigFile(void)
: m_bIsInitialized(FALSE)
{
}

CSolidSBCConfigFile::~CSolidSBCConfigFile(void)
{
	if (m_bIsInitialized)
		m_cConfigFile.Close();

	m_bIsInitialized = FALSE;
}

int CSolidSBCConfigFile::Init(CString strFileNamePath)
{
	if (m_bIsInitialized)
		m_cConfigFile.Close();

	CFileException fileException;

	if ( !m_cConfigFile.Open( strFileNamePath, CFile::modeReadWrite, &fileException ) ){
		if ( !m_cConfigFile.Open( strFileNamePath, CFile::modeCreate | CFile::modeReadWrite, &fileException ) ){
	
			{
				//error
			}
			return 0;
		}
	}

	m_bIsInitialized = TRUE;
	return 1;
}

int CSolidSBCConfigFile::GetParamInt(CString strKeyName, LPINT pnValue)
{
	if (!m_bIsInitialized)
		return 1;

	CString strValue;
	int nError = GetParamStr(strKeyName, &strValue);
	if (nError)
		return 2;

	strValue.Replace(_T(" "),_T(""));
	strValue.Replace(_T("	"),_T(""));
	*pnValue = _ttoi(strValue);

	return 0;
}

int CSolidSBCConfigFile::GetParamStr(CString strKeyName, CString* pstrValue)
{
	if (!m_bIsInitialized)
		return 1;

	CString strContent = _T("");
	TRY
	{
		m_cConfigFile.SeekToBegin();
		char szBuffer[1024] = {0};
		int  nBufferLen     = sizeof( szBuffer );
		int  nActual        = 0;
		while ( (nActual = m_cConfigFile.Read( szBuffer, nBufferLen )) > 0 ){
			szBuffer[nActual] = 0;
			strContent += szBuffer;}
	}
	CATCH(CException, e)
	{
		int i = 0;
		i++;
	}
	END_CATCH
	strContent += _T("\n"); //adding for parsing reasons (make sure the last entry has \n at end)

	int nKeyStartPos   = strContent.Find(strKeyName);
	if ( nKeyStartPos == -1 )
		return 2;
	int nEqualSign     = strContent.Find(_T("="),nKeyStartPos);
	if ( nEqualSign == -1 )
		return 2;
	int nValueStartPos = nEqualSign + 1;
	int nValueStopPos  = strContent.Find(_T("\n"),nValueStartPos);
	if ( nValueStopPos == -1 )
		return 2;

	CString strValue = strContent.Mid( nValueStartPos, nValueStopPos - nValueStartPos );
	strValue.Replace(_T("\n"),_T(""));
	strValue.Replace(_T("\r"),_T(""));
	*pstrValue = strValue;

	return 0;
}
