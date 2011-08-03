#include <map>
#include <vector>
#include <afxmt.h>

class CSolidSBCTestThread;
class CSolidSBCTestResult;
class CSolidSBCTestConfig;

typedef enum {
	SSBC_TEST_STATE_INVALID  = 0,
	SSBC_TEST_STATE_INACTIVE = 1,
	SSBC_TEST_STATE_ACTIVE   = 2,
	SSBC_TEST_STATE_STOPPING = 3,
} SSBC_TEST_STATE, *PSSBC_TEST_STATE;

typedef struct {
	PSSBC_TEST_STATE pnState;
	CMutex*          pStateMutex;
} SSBC_STATE_CONTAINER, *PSSBC_STATE_CONTAINER;

typedef struct {
	std::vector<CSolidSBCTestResult*>* pResults;
	CMutex*                            pResultMutex;
} SSBC_RESULTS_CONTAINER, *PSSBC_RESULTS_CONTAINER;

typedef std::pair< std::string, AFX_THREADPROC>          SSBC_TESTNAME_FUNC_PAIR_TYPE;
typedef std::map < std::string, SSBC_RESULTS_CONTAINER > SSBC_RESULT_MAP_TYPE;
typedef std::map < std::string, CSolidSBCTestConfig* >	 SSBC_CONFIG_MAP_TYPE;

typedef struct {
	SSBC_STATE_CONTAINER   stateContainer;
	SSBC_RESULTS_CONTAINER resultContainer;
	CSolidSBCTestConfig*   pTestConfig;
} SSBC_TEST_THREAD_PARAM,*PSSBC_TEST_THREAD_PARAM;

