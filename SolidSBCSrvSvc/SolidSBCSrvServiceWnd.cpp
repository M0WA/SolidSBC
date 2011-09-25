#include "StdAfx.h"
#include "SolidSBCSrvServiceWnd.h"

#include <time.h>

CString g_strImagePath = _T(".\\");
CMutex  g_cSvcLogMutex;

CSolidSBCSrvServiceWnd::CSolidSBCSrvServiceWnd(void)
{
}

CSolidSBCSrvServiceWnd::~CSolidSBCSrvServiceWnd(void)
{
}

int CSolidSBCSrvServiceWnd::LogServiceMessage(CString strMessage, SSBC_SRVSVC_LOGMSG_TYPE nType)
{
#ifndef _DEBUG
	// Throw away debug messages when we are in a non-debug version.
	// Do this stuff hardcoded, because we dont want the user to see
	// our internals.
	if ( nType >= SSBC_SRVSVC_LOGMSG_TYPE_DEBUG )
		return 0;
#endif
	USES_CONVERSION;

	time_t rawtime;
	time(&rawtime);
	struct tm timeinfo;
	localtime_s( &timeinfo, &rawtime );

	TCHAR pszTime[256];
	memset(pszTime,0,256);
	_tcsftime(pszTime,256,_T("%d/%m/%y-%H:%m:%S "),&timeinfo);
	
	switch ( nType ) {

		case SSBC_SRVSVC_LOGMSG_TYPE_ERROR:
			strMessage = _T("[ERROR] ") + strMessage;
			break;

		case SSBC_SRVSVC_LOGMSG_TYPE_WARN:
			strMessage = _T("[WARN]  ") + strMessage;
			break;

		case SSBC_SRVSVC_LOGMSG_TYPE_INFO:
			strMessage = _T("[INFO]  ") + strMessage;
			break;

		case SSBC_SRVSVC_LOGMSG_TYPE_DEBUG:
			strMessage = _T("[DEBUG] ") + strMessage;
			break;

		case SSBC_SRVSVC_LOGMSG_TYPE_DEBUG_BREAK:
			strMessage = _T("[DEBUG_BREAK] ") + strMessage;
			break;

		default:
			strMessage = _T("[UNKNW] ") + strMessage;
			break;
	}
	strMessage = pszTime + strMessage;
	strMessage += _T("\r\n");

	char* pczMsg = T2A(strMessage);
	int nOpenFlags = ( CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareExclusive );

	g_cSvcLogMutex.Lock();
	TRY
	{
		CFile cSvcLogFile( GetSvcLogFileName(), nOpenFlags );
		cSvcLogFile.SeekToEnd();
		cSvcLogFile.Write( pczMsg, (UINT)strlen(pczMsg) );
		cSvcLogFile.Close( );
	}
	CATCH(CException, pEx)
	{
		//TODO: show error 
	}
	END_CATCH
	g_cSvcLogMutex.Unlock();

	if ( nType == SSBC_SRVSVC_LOGMSG_TYPE_DEBUG_BREAK )
		DebugBreak();
	return 0;
}

CString CSolidSBCSrvServiceWnd::GetSvcLogFileName(void)
{
	CString strFileName = _T("");
	if ( g_strImagePath != _T("") ){
		strFileName.Format(_T("%s\\%s"),g_strImagePath,SSBC_SRVSVC_LOG_FILENAME);
	} else {			
		TCHAR szFileNamePath[1024];
		GetModuleFileName(NULL,szFileNamePath,1024);
		CString strFileNamePath = szFileNamePath;
		int nPos = strFileNamePath.ReverseFind(_T('\\'));
		CString strPath = strFileNamePath.Left(nPos);
		strFileName.Format(_T("%s\\%s"),strPath,SSBC_SRVSVC_LOG_FILENAME);
		g_strImagePath = szFileNamePath;
	}

	return strFileName;
}
