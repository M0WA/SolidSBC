// SolidSBCTestSDK.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SolidSBCTestSDK.h"
#include "SolidSBCTestThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

CSolidSBCTestManager::CSolidSBCTestManager(void)
{
}

CSolidSBCTestManager::~CSolidSBCTestManager(void)
{
}

int CSolidSBCTestManager::GetTestNames(std::vector<std::string>& vecTestnames)
{
	SSBC_TEST_MAP_TYPE::iterator iIter;
	int nSize = 0;

	nSize = (int)m_mapSolidSBCTestThreads.size();
	for( iIter = m_mapSolidSBCTestThreads.begin(); iIter != m_mapSolidSBCTestThreads.end(); iIter++)
		vecTestnames.push_back( (*iIter).first );

	return nSize;
}

void CSolidSBCTestManager::AddTest(const std::string& sTestName, const AFX_THREADPROC pThreadFunc)
{
	vector<CSolidSBCTestResult*>* pResultVector = new vector<CSolidSBCTestResult*>();
	m_mapSolidSBCTestThreads[sTestName] = SSBC_TEST_PAIR_TYPE( 
		new CSolidSBCTestThread( sTestName, pThreadFunc, pResultVector ), 
		pResultVector
	);
}

int CSolidSBCTestManager::StartTest(const std::string& sTestName, LPVOID pszParam)
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

	pThread->StopThread();
	return 0;
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
	for( SSBC_TEST_MAP_TYPE::iterator iIter = m_mapSolidSBCTestThreads.begin(); iIter != m_mapSolidSBCTestThreads.end(); iIter++)
	{
		if ( (*iIter).second.second )
		{
			vector<CSolidSBCTestResult*>::iterator iIterResults;
			for ( iIterResults = (*iIter).second.second->begin(); iIterResults != (*iIter).second.second->end(); iIterResults++ ){
				vecResults.push_back(*iIterResults);}
			nSize += (*iIter).second.second->size();
			(*iIter).second.second->clear();
		}
	}
	return nSize;
}