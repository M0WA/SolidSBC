#include "stdafx.h"
#include "SolidSBCResultManager.h"


CSolidSBCResultManager::CSolidSBCResultManager(void)
: m_threadResultStackThread(NULL)
{
}


CSolidSBCResultManager::~CSolidSBCResultManager(void)
{
	StopResultManager();
}

void CSolidSBCResultManager::StartResultManager(CMutex* pTestMutex, std::vector< std::pair<HMODULE, CSolidSBCTestManager*> >* pTests)
{
	StopResultManager();

	m_ResultStackParam.pTestMutex = pTestMutex;
	m_ResultStackParam.pTests     = pTests;
	m_ResultStackParam.bShallEnd  = false;

	m_threadResultStackThread = new CWinThread(ResultManagerThread,(LPVOID)&m_ResultStackParam);
	m_threadResultStackThread->CreateThread(CREATE_SUSPENDED);
	m_threadResultStackThread->m_bAutoDelete = false;
	m_threadResultStackThread->ResumeThread();
}

void CSolidSBCResultManager::StopResultManager(void)
{
	if (!m_threadResultStackThread)
		return;

	m_ResultStackParam.cStateMutex.Lock();
	m_ResultStackParam.bShallEnd = true;
	m_ResultStackParam.cStateMutex.Unlock();

	DWORD dwWait = WaitForSingleObject(m_threadResultStackThread->m_hThread, INFINITE);
	delete m_threadResultStackThread;
	m_threadResultStackThread = NULL;
}

bool CSolidSBCResultManager::ResultManagerThreadShallEnd(PSSBC_RESULT_MANAGER_THREAD_PARAM pParam)
{
	pParam->cStateMutex.Lock();
	bool bReturn = pParam->bShallEnd;
	pParam->cStateMutex.Unlock();
	return bReturn;
}

UINT CSolidSBCResultManager::ResultManagerThread(LPVOID lpParam)
{
	PSSBC_RESULT_MANAGER_THREAD_PARAM pParam = (PSSBC_RESULT_MANAGER_THREAD_PARAM)lpParam;
	
	std::vector<CSolidSBCTestResult*> vecTestResults;
	std::vector<CSolidSBCTestResult*>::iterator iResults;
	std::vector< std::pair<HMODULE, CSolidSBCTestManager*> >::iterator iTests;

	while( !ResultManagerThreadShallEnd(pParam) )
	{
		pParam->pTestMutex->Lock();
		for ( iTests = pParam->pTests->begin(); iTests != pParam->pTests->end(); iTests++)
		{
			if ( !(*iTests).second )
				continue;
			
			vecTestResults.clear();
			(*iTests).second->GetTestResults(vecTestResults);

			//no results can be fetched,
			//waiting some time...
			if( !vecTestResults.size() )
				Sleep(1000);

			for ( iResults = vecTestResults.begin(); iResults != vecTestResults.end(); iResults++)
			{
				g_cClientService.SendTestResult( (*iResults) );
				delete (*iResults);
				(*iResults) = NULL;
			}
			vecTestResults.clear();
		}
		pParam->pTestMutex->Unlock();
	}

	return 0;
}