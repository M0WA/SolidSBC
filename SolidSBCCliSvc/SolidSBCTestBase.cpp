#include "stdafx.h"
#include "SolidSBCTestBase.h"

CVMPerfTestBase::CVMPerfTestBase(void)
: m_pThread(NULL)
, m_bShallStop(FALSE)
{
}

CVMPerfTestBase::~CVMPerfTestBase(void)
{
	Stop();
}

int CVMPerfTestBase::Start(AFX_THREADPROC threadFunc, HWND hWnd, LPVOID threadParam)
{
	Stop();

	m_baseThreadParam.threadParam = threadParam;
	m_baseThreadParam.pcStopMutex = &m_cStopMutex;
	m_baseThreadParam.pbShallStop = &m_bShallStop;
	m_bShallStop = FALSE;
	if ( !(m_pThread = AfxBeginThread(threadFunc,(LPVOID)&m_baseThreadParam,0,0,CREATE_SUSPENDED)) )
		return 0;

	m_pThread->m_bAutoDelete = FALSE;
	return m_pThread->ResumeThread();
}

int CVMPerfTestBase::Stop(void)
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

	m_cStopMutex.Lock();
	m_bShallStop = TRUE;
	m_cStopMutex.Unlock();

	dwErr = WaitForSingleObject(m_pThread->m_hThread,INFINITE);
	delete m_pThread;
	m_pThread = NULL;

	m_bShallStop = FALSE;
	return 0;
}
