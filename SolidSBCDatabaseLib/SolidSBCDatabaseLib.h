#pragma once

#ifdef SOLIDSBCDATABASELIB_EXPORTS
#define SOLIDSBCDATABASELIB_API __declspec(dllexport)
#else
#define SOLIDSBCDATABASELIB_API __declspec(dllimport)
#endif

#include "defines.h"
#include "SolidSBCResultDBConnector.h"

class CSolidSBCResultDBConnector;
SOLIDSBCDATABASELIB_API void InitDatabase(SSBC_DB_TYPE type);
SOLIDSBCDATABASELIB_API CSolidSBCResultDBConnector* GetDatabaseConnection(void);