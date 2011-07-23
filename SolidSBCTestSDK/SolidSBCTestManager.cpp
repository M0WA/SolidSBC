#include "stdafx.h"
#include "SolidSBCTestManager.h"

using namespace std;

CSolidSBCTestManager::CSolidSBCTestManager(void)
{
}

CSolidSBCTestManager::~CSolidSBCTestManager(void)
{
	for( SSBC_TEST_MAP_TYPE::iterator iIter = m_mapSolidSBCTestThreads.begin(); iIter != m_mapSolidSBCTestThreads.end(); iIter++)
	{
		//end and delete thread
		CSolidSBCTestThread* pThread = (*iIter).second.first;
		if (pThread)
		{
			pThread->StopThread();
			delete pThread;
			pThread = 0;
		}

		(*iIter).second.second.pResultMutex->Lock();
		std::vector<CSolidSBCTestResult*>* pResultVector = (*iIter).second.second.pResults;
		if (pResultVector && pResultVector->size())
		{
			std::vector<CSolidSBCTestResult*>::iterator iIterRes = pResultVector->begin();
			for ( ; iIterRes != pResultVector->end(); iIterRes++ )
			{
				CSolidSBCTestResult* pResult = (*iIterRes);
				if ( pResult )
					delete pResult;
			}
		}
		delete pResultVector;
		(*iIter).second.second.pResultMutex->Unlock();

		delete (*iIter).second.second.pResultMutex;
	}
}

int CSolidSBCTestManager::GetTestNames(std::vector<std::string>& vecTestnames)
{
	SSBC_TEST_MAP_TYPE::iterator iIter;
	int nSize = 0;

	nSize = (int)m_mapSolidSBCTestThreads.size();

	vecTestnames.clear();
	vecTestnames.reserve(nSize);

	for( iIter = m_mapSolidSBCTestThreads.begin(); iIter != m_mapSolidSBCTestThreads.end(); iIter++)
		vecTestnames.push_back( (*iIter).first );

	return nSize;
}

void CSolidSBCTestManager::AddTest(const std::string& sTestName, const AFX_THREADPROC pThreadFunc)
{
	SSBC_RESULTS_CONTAINER resultContainer;
	resultContainer.pResultMutex = new CMutex();
	resultContainer.pResults     = new vector<CSolidSBCTestResult*>();

	m_mapSolidSBCTestThreads[sTestName] = SSBC_TEST_PAIR_TYPE( 
		new CSolidSBCTestThread( sTestName, pThreadFunc, resultContainer ), 
		resultContainer
	);
}

int CSolidSBCTestManager::StartTest(const std::string& sTestName, LPVOID pszParam, bool bIsCallback, unsigned int nCallbackInterval)
{
	CSolidSBCTestThread* pThread = GetThreadByName(sTestName);
	if( !pThread )
		return 1;

	return pThread->StartThread(pszParam);
}

int CSolidSBCTestManager::StopTest(const std::string& sTestName)
{
	CSolidSBCTestThread* pThread = GetThreadByName(sTestName);
	if( !pThread )
		return 1;

	return pThread->StopThread();
}

CSolidSBCTestThread* CSolidSBCTestManager::GetThreadByName(const std::string& sTestName)
{
	if (m_mapSolidSBCTestThreads.find(sTestName) != m_mapSolidSBCTestThreads.end())
		return m_mapSolidSBCTestThreads[sTestName].first;
	return NULL;
}

int CSolidSBCTestManager::GetTestResults(std::vector<CSolidSBCTestResult*>& vecResults)
{
	int nSize = 0;
	std::vector<CSolidSBCTestResult*>* pResultVector = 0;
		
	for( SSBC_TEST_MAP_TYPE::iterator iIter = m_mapSolidSBCTestThreads.begin(); iIter != m_mapSolidSBCTestThreads.end(); iIter++) {

		(*iIter).second.second.pResultMutex->Lock();

		pResultVector = (*iIter).second.second.pResults;
		if ( pResultVector && pResultVector->size() ) {
			nSize += pResultVector->size();
			vecResults.insert( vecResults.end(), pResultVector->begin(), pResultVector->end() );
			pResultVector->clear();	}

		(*iIter).second.second.pResultMutex->Unlock();
	}

	return nSize;
}