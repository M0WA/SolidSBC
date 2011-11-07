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

	int  GetTestResults(std::vector<CSolidSBCTestResult*>& vecResults);

	CSolidSBCTestConfig* GetTestConfigByName( const std::string& sTestName );
	void                 SetTestConfigByName( const std::string& sTestName, CSolidSBCTestConfig* pConfig );
	
	int  GetRegisteredTestCount(void) { return (int)m_vecTestNames.size(); };
	int  GetTestNames          (std::vector<std::string>& vecTestnames);
	bool HasTestName           ( const std::string& sTestName );

	void GetCreateTableStatements(std::string& sStatement);

protected:
	void RegisterTest(AFX_THREADPROC pThreadFunc, CSolidSBCTestConfig* pDefaultConfig, CSolidSBCTestResult* pTestResult);

private:
	AFX_THREADPROC GetThreadFuncByName(const std::string& sTestName);
	std::vector<SSBC_TESTNAME_FUNC_PAIR_TYPE> m_vecTestNames;

	SSBC_RESULT_MAP_TYPE                       m_mapTestResults;
	SSBC_CONFIG_MAP_TYPE                       m_mapTestConfigs;
	std::map<std::string,CSolidSBCTestResult*> m_mapTestResultDataTypes;
	std::vector<CSolidSBCTestThread*>          m_vecRunningTests;
};