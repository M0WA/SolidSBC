#include "StdAfx.h"
#include "SolidSBCCliServiceWnd.h"

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

	_tzset();
	char tmpTime[9];
	ZeroMemory(tmpTime,9);
    _strtime_s(tmpTime, 9 );
	CString strTime;
	strTime.Format( _T("%s\t"), A2T(tmpTime) );

	switch ( nType ) {

		case SSBC_CLISVC_LOGMSG_TYPE_ERROR:
			strMessage = _T("[ERROR]\t") + strMessage;
			break;

		case SSBC_CLISVC_LOGMSG_TYPE_WARN:
			strMessage = _T("[WARN]\t") + strMessage;
			break;

		case SSBC_CLISVC_LOGMSG_TYPE_INFO:
			strMessage = _T("[INFO]\t") + strMessage;
			break;

#ifdef _DEBUG
		case SSBC_CLISVC_LOGMSG_TYPE_DEBUG:
			strMessage = _T("[DEBUG]\t") + strMessage;
			break;

		case SSBC_CLISVC_LOGMSG_TYPE_DEBUG_BREAK:
			strMessage = _T("[DEBUG_BREAK]\t") + strMessage;
			break;
#endif

		default:
			strMessage = _T("[UNKNW]\t") + strMessage;
			break;
	}
	strMessage = strTime + strMessage;
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