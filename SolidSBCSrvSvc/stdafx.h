// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxmt.h>
#include <afxsock.h>
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include <vector>
#import <msxml3.dll> raw_interfaces_only

//reference additional headers your program requires here
#include <mysql.h>

#include <SolidSBCNetLib.h>

#include "defines.h"
#include "SolidSBCSrvServiceWnd.h"

#include "SolidSBCResultClientHandlerSocket.h"
#include "SolidSBCConfigClientSocket.h"
#include "SolidSBCResultClientSocket.h"

#include "SolidSBCClientConfigInfo.h"
#include "SolidSBCClientResultInfo.h"
#include "SolidSBCClientConfigProfile.h"
#include "SolidSBCClientList.h"