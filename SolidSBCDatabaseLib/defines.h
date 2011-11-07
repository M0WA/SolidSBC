#pragma once

//database types
typedef enum SSBC_DB_TYPE {
	SSBC_DB_TYPE_NONE		= 0,
	SSBC_DB_TYPE_MYSQL		= 1
};

//enum for client_history table action types
typedef enum SSBC_CLIENT_HISTORY_ACTION{
	SSBC_CLIENT_ACTION_ADD_RESULT_CLIENT    = 1,
	SSBC_CLIENT_ACTION_DELETE_RESULT_CLIENT = 2,
};

////////////////////////////////////////////////////////////////////
//
// data types used in results
//	
typedef enum DataType
{
	DB_NONE, // attributes with type "NONE"
				// will be ommited while 
				// generating structure
	DB_INT16,
	DB_INT32,
	DB_INT64,
	DB_UINT16,
	DB_UINT32,
	DB_UINT64,
	DB_DOUBLE,
	DB_STRING255,
	DB_STRING1024,
	DB_STRING4096,
};