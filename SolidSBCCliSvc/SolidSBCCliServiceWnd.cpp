#include "StdAfx.h"
#include "SolidSBCCliServiceWnd.h"

#include <time.h>

CMutex g_cSvcLogMutex;
CString g_strImagePath;

CSolidSBCCliServiceWnd::CSolidSBCCliServiceWnd(void)
{
}

CSolidSBCCliServiceWnd::~CSolidSBCCliServiceWnd(void)
{
}

int CSolidSBCCliServiceWnd::LogServiceMessage( CString strMessage, SSBC_CLISVC_LOGMSG_TYPE nType )
{
	USES_CONVERSION;

	time_t rawtime;
	time(&rawtime);
	struct tm timeinfo;
	localtime_s( &timeinfo, &rawtime );

	TCHAR pszTime[256];
	memset(pszTime,0,256);
	_tcsftime(pszTime,256,_T("%d/%m/%y-%H:%m:%S "),&timeinfo);

	switch ( nType ) {

		case SSBC_CLISVC_LOGMSG_TYPE_ERROR:
			strMessage = _T("[ERROR] ") + strMessage;
			break;

		case SSBC_CLISVC_LOGMSG_TYPE_WARN:
			strMessage = _T("[WARN]  ") + strMessage;
			break;

		case SSBC_CLISVC_LOGMSG_TYPE_INFO:
			strMessage = _T("[INFO]  ") + strMessage;
			break;

#ifdef _DEBUG
		case SSBC_CLISVC_LOGMSG_TYPE_DEBUG:
			strMessage = _T("[DEBUG] ") + strMessage;
			break;

		case SSBC_CLISVC_LOGMSG_TYPE_DEBUG_BREAK:
			strMessage = _T("[DEBUG_BREAK] ") + strMessage;
			break;
#endif

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

#ifdef _DEBUG
	if ( nType == SSBC_CLISVC_LOGMSG_TYPE_DEBUG_BREAK )
		DebugBreak();
#endif

	return 0;
}

CString CSolidSBCCliServiceWnd::GetSvcLogFileName(void)
{
	//assemble name for svc settings file
	CString strFileName = _T("");
	if ( g_strImagePath != _T("") ){
		strFileName.Format(_T("%s\\%s"),g_strImagePath,SSBC_CLISVC_LOG_FILENAME);
	} else {			
		TCHAR szFileNamePath[1024];
		GetModuleFileName(NULL,szFileNamePath,1024);
		CString strFileNamePath = szFileNamePath;
		int nPos = strFileNamePath.ReverseFind(_T('\\'));
		CString strPath = strFileNamePath.Left(nPos);
		strFileName.Format(_T("%s\\%s"),strPath,SSBC_CLISVC_LOG_FILENAME);
		g_strImagePath = szFileNamePath;
	}
	return strFileName;
}