#pragma once

#define SSBC_CLISVC_VERSION_MAJOR		_T("1")
#define SSBC_CLISVC_VERSION_MINOR		_T("5")

#define SSBC_CLISVC_NAME				_T("SolidSbcCli")
#define SSBC_CLISVC_DISPLAY				_T("SolidSBC Client")
#define SSBC_CLISVC_DESCRIPTION			_T("SolidSBC Client Service")

#define SSBC_CLISVC_LOG_FILENAME		_T("ssbc_cli.log")
#define SSBC_CLISVC_SETTINGS_FILENAME	_T("ssbc_cli.dat")
#define MAX_SERVER_NAME 1024

#define SSBC_CLIENT_UUID_FILENAMEPATH   "ssbc_cli.uuid"
#define SSBC_CLIENT_UUID_MAX_RAND		0xff

//define to search for test dlls 
//in build directories in release version 
#define _DEBUG_TEST_DLL_BUILD_DIRECTORY 0

//define to see extra logging messages
//concerning test-dlls
#define _DEBUG_TEST_DLL_OUTPUT          0

typedef struct {
	BOOL    bAutoStart;
	TCHAR*  szDataSource[MAX_SERVER_NAME];
	DWORD   dwSrvConfPort;
	DWORD   dwSrvDataPort;
	BOOL    bAutoReconnect;
} SSBC_CLISVC_PARAM, *PSSBC_CLISVC_PARAM;
