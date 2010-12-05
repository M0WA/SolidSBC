#pragma once

extern CString g_strImagePath;

class CSolidSBCSrvServiceWnd
{
public:
	CSolidSBCSrvServiceWnd(void);
	~CSolidSBCSrvServiceWnd(void);
	static int LogServiceMessage( CString strMessage, SSBC_SRVSVC_LOGMSG_TYPE nType =  SSBC_SRVSVC_LOGMSG_TYPE_INFO );
	static CString GetSvcLogFileName(void);
};
