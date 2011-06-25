
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxmt.h>
#include <vector>
#include <mysql.h>

//keynames for settings in config file
#define SSBC_DATABASE_TYPE				_T("DatabaseType")
#define SSBC_DATABASE_HOST				_T("DatabaseHost")
#define SSBC_DATABASE_PORT				_T("DatabasePort")
#define SSBC_DATABASE_NAME				_T("DatabaseName")
#define SSBC_DATABASE_USER				_T("DatabaseUser")
#define SSBC_DATABASE_PASS				_T("DatabasePass")

#define SSBC_SRV_CFGFILE				_T("ssbc_srv.conf")

#define SSCB_CSV_COMMA_STRING _T(";")
#define SSCB_CSV_LINEEND_STRING _T("\r\n")

typedef struct{
	CString strClientUUID;
	CString strClientName;
	int     nClientID;
	CString strBeginTime;
	CString strEndTime;
} SSBC_CLIENT_IDENTITY, *PSSBC_CLIENT_IDENTITY;

typedef struct{
	CString strAction;
	CString strParam;
	CString strClientID;
	CString strClientName;
	CString strClientUUID;
	CString strTime;
} SSBC_CLIENT_HISTORY, *PSSBC_CLIENT_HISTORY;

typedef struct{
	CString strType;
	CString strDuration;
	CString strBytes;
	CString strWait;
	CString strTime;
} SSBC_CLIENT_HD_RESULT, *PSSBC_CLIENT_HD_RESULT;

typedef struct{
	CString strClientID;
	CString strMallocZeroDuration;
	CString strBytes;
	CString strTime;
} SSBC_CLIENT_MEM_RESULT, *PSSBC_CLIENT_MEM_RESULT;

typedef struct{
	CString strAddDuration;
	CString strDivDuration;
	CString strAllDuration;
	CString strAddMultiplier;
	CString strDivMultiplier;
	CString strTime;
} SSBC_CLIENT_CPUMEASURE_RESULT, *PSSBC_CLIENT_CPUMEASURE_RESULT;

typedef struct{
	CString strRTTDuration;
	CString strICMPOpts;
	CString strTime;
} SSBC_CLIENT_PING_RESULT, *PSSBC_CLIENT_PING_RESULT;

typedef struct{
	CString strHandshakeDuration;
	CString strTime;
} SSBC_CLIENT_TCPCON_RESULT, *PSSBC_CLIENT_TCPCON_RESULT;

#define SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX		WM_USER + 100

typedef enum {
	SSBC_HARDDRIVE,
	SSBC_MEMORY,
	SSBC_CPUMEASURE,
	SSBC_PING,
	SSBC_TCPCON
} SSBC_RESULT_TYPE;

#include "MainFrm.h"
#include "DatabaseConnection.h"
#include "ResultContextMenu.h"
#include "SolidSBCTimeChooseDlg.h"
#include "SolidSBCConfigFile.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


