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