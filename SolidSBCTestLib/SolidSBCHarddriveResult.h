#pragma once

#include <SolidSBCTestSDK.h>

typedef enum SSBC_TEST_HARDDRIVE_RESULT_TYPE { 
	SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ	    = 0,
	SSBC_TEST_HARDDRIVE_RESULT_TYPE_WRITE	    = 1,
	SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ_INIT   = 2
};

class CSolidSBCHarddriveResult : CSolidSBCTestResult
{
public:
	CSolidSBCHarddriveResult(void) : CSolidSBCTestResult("testLib_HDResults") {};
	
	DOUBLE GetDuration(void);
	inline void SetDuration(DOUBLE dDuration) { SetKeyValue( "duration", dDuration); }
	
	ULONG  GetByteCount(void);
	inline void SetByteCount(ULONG ulBytes) { SetKeyValue( "amountbytes", ulBytes); };
	
	ULONG  GetWaitCount(void);
	inline void SetWaitCount(ULONG ulBytes) { SetKeyValue( "wait", ulBytes); };

	SSBC_TEST_HARDDRIVE_RESULT_TYPE GetResultType(void);
	inline void SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE nType) { SetKeyValue( "resulttype", (int)nType); };
};

