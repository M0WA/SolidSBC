#include "StdAfx.h"
#include "SolidSBCTestSDK.h"
#include "SolidSBCTestThread.h"

CSolidSBCTestThread::CSolidSBCTestThread(const std::string& sTestName, const PSSBC_TEST_THREAD_FUNC pThreadFunc, std::vector<CSolidSBCTestResult*>* pResults)
: m_pThread(NULL)
, m_sTestName(sTestName)
, m_pThreadFunc(pThreadFunc)
{
	m_testThreadParam.pnState  = &m_nState;
	m_testThreadParam.pMutex   = &m_Mutex;
	m_testThreadParam.pResults = pResults;
}

CSolidSBCTestThread::~CSolidSBCTestThread(void)
{
	StopThread();
}

int CSolidSBCTestThread::StartThread(LPVOID pTestParam, HWND hWnd)
{
	StopThread();

	m_testThreadParam.pThreadParam = pTestParam;
	if ( !(m_pThread = AfxBeginThread(m_pThreadFunc,(LPVOID)&m_testThreadParam,0,0,CREATE_SUSPENDED)) )
		return 0;
	m_pThread->m_bAutoDelete = FALSE;
	
	m_Mutex.Lock();
	m_nState = SSBC_TEST_STATE_ACTIVE;
	m_Mutex.Unlock();

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
	
	m_Mutex.Lock();
	m_nState = SSBC_TEST_STATE_STOPPING;
	m_Mutex.Unlock();

	dwErr = WaitForSingleObject(m_pThread->m_hThread,INFINITE);
	delete m_pThread;
	m_pThread = NULL;

	m_Mutex.Lock();
	m_nState = SSBC_TEST_STATE_INACTIVE;
	m_Mutex.Unlock();
	return 0;
}

BOOL CSolidSBCTestThread::ShallThreadEnd(PSSBC_TEST_THREAD_PARAM pParam)
{
	pParam->pMutex->Lock();
	BOOL bReturn = ((*(pParam->pnState)) == SSBC_TEST_STATE_ACTIVE) ? TRUE : FALSE;
	pParam->pMutex->Unlock();
	return bReturn;
}