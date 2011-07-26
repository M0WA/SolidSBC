#pragma once

#include "SolidSBCTestSDK.h"

class SOLIDSBCTESTSDK_API CSolidSBCTestManager
{
protected:
	CSolidSBCTestManager(void);
	~CSolidSBCTestManager(void);

public:
	int StartTest(const std::string& sXML);
	int StopTest (const std::string& sTestName);

	int GetTestNames  (std::vector<std::string>& vecTestnames);
	int GetTestResults(std::vector<CSolidSBCTestResult*>& vecResults);

	CSolidSBCTestConfig* GetTestConfigByName( const std::string& sTestName );
	void                 SetTestConfigByName( const std::string& sTestName, CSolidSBCTestConfig* pConfig );

	int GetRegisteredTestCount(void) { return m_vecTestNames.size(); };

protected:
	void AddTest(AFX_THREADPROC pThreadFunc, CSolidSBCTestConfig* pDefaultConfig);

private:
	AFX_THREADPROC GetThreadFuncByName(const std::string& sTestName);
	std::vector<SSBC_TESTNAME_FUNC_PAIR_TYPE> m_vecTestNames;
	SSBC_RESULT_MAP_TYPE                      m_mapTestResults;
	SSBC_CONFIG_MAP_TYPE                      m_mapTestConfigs;

	std::vector<CSolidSBCTestThread*>         m_vecRunningTests;
};