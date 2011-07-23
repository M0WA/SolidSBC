#pragma once

//version
#define SSBC_SRVSVC_VERSION_MAJOR		_T("1")
#define SSBC_SRVSVC_VERSION_MINOR		_T("5")

//windows service releated names
#define SSBC_SRVSVC_NAME				_T("SolidSbcSrv")
#define SSBC_SRVSVC_DISPLAY				_T("SolidSBC Server")
#define SSBC_SRVSVC_DESCRIPTION			_T("SolidSBC Server Service")

//hardcoded file names
#define SSBC_SRVSVC_LOG_FILENAME		_T("ssbc_srv.log")
#define SSBC_SRV_CFGFILE				_T("ssbc_srv.conf")
#define SSBC_SRV_PROFILEFILE			_T("ssbc_profiles.xml")

//server ports
#define SSBC_CFG_SERVER_PORT			52410
#define SSBC_DTA_SERVER_PORT			52411

//logtypes
typedef enum SSBC_SRVSVC_LOGMSG_TYPE {
	SSBC_SRVSVC_LOGMSG_TYPE_ERROR		= 0,
	SSBC_SRVSVC_LOGMSG_TYPE_WARN		= 1,
	SSBC_SRVSVC_LOGMSG_TYPE_INFO		= 2,
	SSBC_SRVSVC_LOGMSG_TYPE_DEBUG		= 3, //from here all messages suppressed in release version
	SSBC_SRVSVC_LOGMSG_TYPE_DEBUG_BREAK	= 4
};

//keynames for settings in config file
#define SSBC_DATABASE_TYPE				_T("DatabaseType")
#define SSBC_DATABASE_HOST				_T("DatabaseHost")
#define SSBC_DATABASE_PORT				_T("DatabasePort")
#define SSBC_DATABASE_NAME				_T("DatabaseName")
#define SSBC_DATABASE_USER				_T("DatabaseUser")
#define SSBC_DATABASE_PASS				_T("DatabasePass")