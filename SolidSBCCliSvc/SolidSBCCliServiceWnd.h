#pragma once

#include "afxwin.h"

extern CString g_strImagePath;

typedef enum SSBC_CLISVC_LOGMSG_TYPE {
	SSBC_CLISVC_LOGMSG_TYPE_ERROR		= 0,
	SSBC_CLISVC_LOGMSG_TYPE_WARN		= 1,
	SSBC_CLISVC_LOGMSG_TYPE_INFO		= 2,
	SSBC_CLISVC_LOGMSG_TYPE_DEBUG		= 3, //from here all messages suppressed in release version
	SSBC_CLISVC_LOGMSG_TYPE_DEBUG_BREAK	= 4
};

class CSolidSBCCliServiceWnd
{
public:

	CSolidSBCCliServiceWnd(void);
	~CSolidSBCCliServiceWnd(void);
	static int LogServiceMessage( CString strMessage, SSBC_CLISVC_LOGMSG_TYPE nType =  SSBC_CLISVC_LOGMSG_TYPE_INFO );
	static CString GetSvcLogFileName(void);

	/*
	static HWND GetMessagingWindow(void);
	static void CreateMessagingWindow(void);
	static void DestroyMessagingWindow(void);
	static void SendTestResult(PSSBC_TEST_RESULT_PACKET pPacket);
	static void SendDbgMsg(int nNo);
	*/
};