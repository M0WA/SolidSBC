#pragma once

typedef struct {
	CMutex cStateMutex;
	bool   bShallEnd;

	CMutex* pTestMutex;
	std::vector< std::pair<HMODULE, CSolidSBCTestManager*> >* pTests;
} SSBC_RESULT_MANAGER_THREAD_PARAM, *PSSBC_RESULT_MANAGER_THREAD_PARAM;

class CSolidSBCResultManager
{
public:
	CSolidSBCResultManager(void);
	~CSolidSBCResultManager(void);

	void StartResultManager(CMutex* pTestMutex, std::vector< std::pair<HMODULE, CSolidSBCTestManager*> >* pTests);
	void StopResultManager(void);

	static UINT ResultManagerThread(LPVOID lpParam);
	static bool ResultManagerThreadShallEnd(PSSBC_RESULT_MANAGER_THREAD_PARAM pParam);

private:
	CWinThread*                      m_threadResultStackThread;
	SSBC_RESULT_MANAGER_THREAD_PARAM m_ResultStackParam;
};

