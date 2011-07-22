#include <map>
#include <vector>
#include <afxmt.h>

class CSolidSBCTestThread;
class CSolidSBCTestResult;

typedef std::pair< CSolidSBCTestThread*, std::vector<CSolidSBCTestResult*>* > SSBC_TEST_PAIR_TYPE;
typedef std::map < std::string, SSBC_TEST_PAIR_TYPE >                         SSBC_TEST_MAP_TYPE;

typedef enum {
	SSBC_TEST_STATE_INVALID  = 0,
	SSBC_TEST_STATE_INACTIVE = 1,
	SSBC_TEST_STATE_ACTIVE   = 2,
	SSBC_TEST_STATE_STOPPING = 3,
} SSBC_TEST_STATE, *PSSBC_TEST_STATE;

typedef struct {
	PSSBC_TEST_STATE pnState;
	CMutex* pMutex;
	LPVOID  pThreadParam;
	std::vector<CSolidSBCTestResult*>* pResults;
} SSBC_TEST_THREAD_PARAM,*PSSBC_TEST_THREAD_PARAM;