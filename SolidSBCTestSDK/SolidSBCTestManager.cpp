#include "stdafx.h"
#include "SolidSBCTestManager.h"

using namespace std;

CSolidSBCTestManager::CSolidSBCTestManager(void)
{
}

CSolidSBCTestManager::~CSolidSBCTestManager(void)
{
	//stop and delete all running tests
	for( std::vector<CSolidSBCTestThread*>::iterator iIter = m_vecRunningTests.begin(); iIter != m_vecRunningTests.end(); iIter++)
	{
		(*iIter)->StopThread();
		delete (*iIter);
		(*iIter) = NULL;
	}
	m_vecRunningTests.clear();

	//delete all configs for tests
	for( SSBC_CONFIG_MAP_TYPE::iterator iIter = m_mapTestConfigs.begin(); iIter != m_mapTestConfigs.end(); iIter++)	{
		delete (*iIter).second;	}
	m_mapTestConfigs.clear();
	
	//delete all remaining results
	for( SSBC_RESULT_MAP_TYPE::iterator iIter = m_mapTestResults.begin(); iIter != m_mapTestResults.end(); iIter++)	{
		
		(*iIter).second.pResultMutex->Lock();

		std::vector<CSolidSBCTestResult*>* pResultVector = (*iIter).second.pResults;
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

		(*iIter).second.pResultMutex->Unlock();
		delete (*iIter).second.pResultMutex;
	}
	m_mapTestResults.clear();
}

int CSolidSBCTestManager::GetTestNames(std::vector<std::string>& vecTestnames)
{
	std::vector<SSBC_TESTNAME_FUNC_PAIR_TYPE>::iterator iIter;
	vecTestnames.clear();
	vecTestnames.reserve(m_vecTestNames.size());
	for( iIter = m_vecTestNames.begin(); iIter != m_vecTestNames.end(); iIter++)
		vecTestnames.push_back( (*iIter).first );
	return (int)vecTestnames.size();
}

void CSolidSBCTestManager::RegisterTest(AFX_THREADPROC pThreadFunc, CSolidSBCTestConfig* pTestConfig)
{
	USES_CONVERSION;
	std::string sTestName = T2A(pTestConfig->GetTestName());

	SSBC_RESULTS_CONTAINER resultContainer;
	resultContainer.pResultMutex = new CMutex();
	resultContainer.pResults     = new vector<CSolidSBCTestResult*>();

	SSBC_TESTNAME_FUNC_PAIR_TYPE pairNameFunc(sTestName,pThreadFunc);
	m_vecTestNames.push_back(pairNameFunc);
	m_mapTestConfigs[sTestName] = pTestConfig;
	m_mapTestResults[sTestName] = resultContainer;
}

int CSolidSBCTestManager::StartTest(const std::string& sXML)
{
	USES_CONVERSION;
	CString strXml = CString(A2T(sXML.c_str()));
	CString strTestName = CSolidSBCTestConfig::GetTestNameFromXML(strXml);
	if(strTestName == _T(""))
		return 1;

	AFX_THREADPROC threadFunc = GetThreadFuncByName(std::string(T2A(strTestName)));
	CSolidSBCTestThread* pThread = new CSolidSBCTestThread(std::string(T2A(strXml)),threadFunc,m_mapTestResults[std::string(T2A(strTestName))]);
	if( !pThread )
		return 1;

	pThread->StartThread(m_mapTestConfigs[std::string(T2A(strTestName))]);
	m_vecRunningTests.push_back(pThread);
	return 0;
}

int CSolidSBCTestManager::StopTest(const std::string& sTestName)
{
	int nStopped = 0;
	for( std::vector<CSolidSBCTestThread*>::iterator iIter = m_vecRunningTests.begin(); iIter != m_vecRunningTests.end(); iIter++)
		if ( (*iIter) && ((*iIter)->GetTestName() == sTestName) )
		{
			(*iIter)->StopThread();
			delete (*iIter);
			(*iIter) = NULL;
			m_vecRunningTests.erase(iIter);

			nStopped++;
		}
	return nStopped;
}

int CSolidSBCTestManager::GetTestResults(std::vector<CSolidSBCTestResult*>& vecResults)
{
	int nSize = 0;
	std::vector<CSolidSBCTestResult*>* pResultVector = 0;
		
	for( SSBC_RESULT_MAP_TYPE::iterator iIter = m_mapTestResults.begin(); iIter != m_mapTestResults.end(); iIter++) {

		(*iIter).second.pResultMutex->Lock();

		pResultVector = (*iIter).second.pResults;
		if ( pResultVector && pResultVector->size() ) {
			nSize += (int)pResultVector->size();
			vecResults.insert( vecResults.end(), pResultVector->begin(), pResultVector->end() );
			pResultVector->clear();	}

		(*iIter).second.pResultMutex->Unlock();
	}

	return nSize;
}

CSolidSBCTestConfig* CSolidSBCTestManager::GetTestConfigByName( const std::string& sTestName )
{ 
	return m_mapTestConfigs[sTestName]; 
}

void CSolidSBCTestManager::SetTestConfigByName( const std::string& sTestName, CSolidSBCTestConfig* pConfig )
{
	delete m_mapTestConfigs[sTestName];
	m_mapTestConfigs[sTestName] = pConfig;
}

AFX_THREADPROC CSolidSBCTestManager::GetThreadFuncByName(const std::string& sTestName)
{
	for( std::vector<SSBC_TESTNAME_FUNC_PAIR_TYPE>::iterator iIter = m_vecTestNames.begin(); iIter != m_vecTestNames.end(); iIter++)
		if ( (*iIter).first == sTestName )
			return (*iIter).second;

	return NULL;
}

bool CSolidSBCTestManager::HasTestName( const std::string& sTestName )
{
	for( std::vector<SSBC_TESTNAME_FUNC_PAIR_TYPE>::iterator iIter = m_vecTestNames.begin(); iIter != m_vecTestNames.end(); iIter++)
		if ( (*iIter).first == sTestName )
			return true;
	return false;
}