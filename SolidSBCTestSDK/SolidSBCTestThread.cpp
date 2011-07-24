#include "StdAfx.h"
#include "SolidSBCTestSDK.h"
#include "SolidSBCTestThread.h"

CSolidSBCTestThread::CSolidSBCTestThread(const std::string& sTestName, const PSSBC_TEST_THREAD_FUNC pThreadFunc, const SSBC_RESULTS_CONTAINER& resultContainer)
: m_pThread(NULL)
, m_sTestName(sTestName)
, m_pThreadFunc(pThreadFunc)
{

	m_testThreadParam.stateContainer.pnState     = &m_nState;
	m_testThreadParam.stateContainer.pStateMutex = &m_StateMutex;

	m_testThreadParam.resultContainer = resultContainer;
}

CSolidSBCTestThread::~CSolidSBCTestThread(void)
{
	StopThread();
}

int CSolidSBCTestThread::StartThread(CSolidSBCTestConfig* pTestConfig, HWND hWnd)
{
	StopThread();

	m_testThreadParam.pTestConfig = pTestConfig;
	if ( !(m_pThread = AfxBeginThread(m_pThreadFunc,(LPVOID)&m_testThreadParam,0,0,CREATE_SUSPENDED)) )
		return 0;
	m_pThread->m_bAutoDelete = FALSE;
	
	m_StateMutex.Lock();
	m_nState = SSBC_TEST_STATE_ACTIVE;
	m_StateMutex.Unlock();

	return m_pThread->ResumeThread();
}

int CSolidSBCTestThread::StopThread(void)
{
	if (!m_pThread)
		return 0;

	DWORD dwErr = WaitForSingleObject(m_pThread->m_hThread,0);
	switch(dwErr)
	{
	case WAIT_ABANDONED:
	case WAIT_OBJECT_0:
		delete m_pThread;
		m_pThread = NULL;
		return 0;
	case WAIT_TIMEOUT:
		break;
	default:
		return 1;
	}
	
	m_StateMutex.Lock();
	m_nState = SSBC_TEST_STATE_STOPPING;
	m_StateMutex.Unlock();

	dwErr = WaitForSingleObject(m_pThread->m_hThread,INFINITE);
	delete m_pThread;
	m_pThread = NULL;

	m_StateMutex.Lock();
	m_nState = SSBC_TEST_STATE_INACTIVE;
	m_StateMutex.Unlock();
	return 0;
}

void CSolidSBCTestThread::AddResult(PSSBC_TEST_THREAD_PARAM pParam, CSolidSBCTestResult* pResult)
{
	pParam->resultContainer.pResultMutex->Lock();
	pParam->resultContainer.pResults->push_back(pResult);
	pParam->resultContainer.pResultMutex->Unlock();
}

BOOL CSolidSBCTestThread::ShallThreadEnd(PSSBC_TEST_THREAD_PARAM pParam)
{
	pParam->stateContainer.pStateMutex->Lock();
	BOOL bReturn = ((*(pParam->stateContainer.pnState)) == SSBC_TEST_STATE_ACTIVE) ? TRUE : FALSE;
	pParam->stateContainer.pStateMutex->Unlock();
	return bReturn;
}