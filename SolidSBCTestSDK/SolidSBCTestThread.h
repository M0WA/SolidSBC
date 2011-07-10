#pragma once

#include <afxmt.h>
#include <map>
#include <string>

typedef enum {
	SSBC_TEST_STATE_INVALID  = 0,
	SSBC_TEST_STATE_INACTIVE = 1,
	SSBC_TEST_STATE_ACTIVE   = 2,
	SSBC_TEST_STATE_STOPPING = 3,
} SSBC_TEST_STATE, *PSSBC_TEST_STATE;

typedef AFX_THREADPROC PSSBC_TEST_THREAD_FUNC;

typedef struct {
	PSSBC_TEST_STATE pnState;
	CMutex* pMutex;
	LPVOID  pThreadParam;
	std::vector<CSolidSBCTestResult*>* pResults;
} SSBC_TEST_THREAD_PARAM,*PSSBC_TEST_THREAD_PARAM;

class CSolidSBCTestThread
{
public:
	CSolidSBCTestThread(const std::string& sTestName, const PSSBC_TEST_THREAD_FUNC pThreadFunc, std::vector<CSolidSBCTestResult*>* pResults);

	int  StartThread( LPVOID pTestParam, HWND hWnd = NULL );
	int  StopThread ( void );
	inline std::string GetTestName(void) { return m_sTestName; };

public:
	static BOOL ShallThreadEnd(PSSBC_TEST_THREAD_PARAM pParam);

protected:
	~CSolidSBCTestThread(void);

private:
	std::string             m_sTestName;

	SSBC_TEST_THREAD_PARAM  m_testThreadParam;

	CWinThread*             m_pThread;
	PSSBC_TEST_THREAD_FUNC  m_pThreadFunc;

	SSBC_TEST_STATE m_nState;
	CMutex          m_Mutex;
};

