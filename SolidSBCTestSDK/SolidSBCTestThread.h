#pragma once

#include <afxmt.h>
#include <map>
#include <string>

#include "SolidSBCTestSDK.h"

typedef AFX_THREADPROC PSSBC_TEST_THREAD_FUNC;

class SOLIDSBCTESTSDK_API CSolidSBCTestThread
{
public:
	CSolidSBCTestThread(const std::string& sTestName, const PSSBC_TEST_THREAD_FUNC pThreadFunc, const SSBC_RESULTS_CONTAINER& resultContainer);
	~CSolidSBCTestThread(void);

	int  StartThread( CSolidSBCTestConfig* pTestConfig, HWND hWnd = NULL );
	int  StopThread ( void );
	inline std::string GetTestName(void) { return m_sTestName; };
	
	//static helper functions
	//useable in test thread functions
	static BOOL ShallThreadEnd(PSSBC_TEST_THREAD_PARAM pParam);
	static void AddResult     (PSSBC_TEST_THREAD_PARAM pParam, CSolidSBCTestResult* pResult);

private:
	std::string             m_sTestName;

	SSBC_TEST_THREAD_PARAM  m_testThreadParam;

	CWinThread*             m_pThread;
	PSSBC_TEST_THREAD_FUNC  m_pThreadFunc;

	SSBC_TEST_STATE m_nState;
	CMutex          m_StateMutex;
};

