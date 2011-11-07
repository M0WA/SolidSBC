#include "stdafx.h"
#include "SolidSBCTestManager.h"

#include <stdio.h>

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

	//delete all dummy results
	for(std::map<std::string,CSolidSBCTestResult*>::iterator iIter = m_mapTestResultDataTypes.begin(); iIter != m_mapTestResultDataTypes.end(); iIter++)
		delete (*iIter).second;

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

void CSolidSBCTestManager::RegisterTest(AFX_THREADPROC pThreadFunc, CSolidSBCTestConfig* pTestConfig, CSolidSBCTestResult* pTestResult)
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
	m_mapTestResultDataTypes[sTestName] = pTestResult;
}

int CSolidSBCTestManager::StartTest(const std::string& sXML)
{
	USES_CONVERSION;
	CString strXml = CString(A2T(sXML.c_str()));
	CString strTestName = CSolidSBCTestConfig::GetTestNameFromXML(strXml);
	if(strTestName == _T(""))
		return 1;

	AFX_THREADPROC threadFunc = GetThreadFuncByName(std::string(T2A(strTestName)));
	CSolidSBCTestThread* pThread = new CSolidSBCTestThread(std::string(T2A(strTestName)),threadFunc,m_mapTestResults[std::string(T2A(strTestName))]);
	if( !pThread )
		return 1;

	CSolidSBCTestConfig* pConfig = m_mapTestConfigs[std::string(T2A(strTestName))];
	pConfig->SetXml(A2T(sXML.c_str()));
	pThread->StartThread(pConfig);
	m_vecRunningTests.push_back(pThread);
	return 0;
}

int CSolidSBCTestManager::StopTest(const std::string& sTestName)
{
	int nStopped = 0;

	while(1)
	{
		bool bTestFound = false;
		for( std::vector<CSolidSBCTestThread*>::iterator iIter = m_vecRunningTests.begin(); iIter != m_vecRunningTests.end(); iIter++)
		{
			if ( (*iIter) && ((*iIter)->GetTestName() == sTestName) )
			{
				(*iIter)->StopThread();
				delete (*iIter);
				(*iIter) = NULL;
				nStopped++;
				
				m_vecRunningTests.erase(iIter);
				bTestFound = true;
				break;
			}
		}

		if(!bTestFound)
			break;
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

void CSolidSBCTestManager::GetCreateTableStatements(std::string& sStatement)
{
	std::map<std::string,CSolidSBCTestResult*>::iterator iIter = m_mapTestResultDataTypes.begin();
	for(; iIter != m_mapTestResultDataTypes.end(); iIter++)
		if( (*iIter).second != NULL )
			sStatement += (*iIter).second->GetTestDBStructure();
}