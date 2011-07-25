#pragma once

#include "SolidSBCTestSDK.h"

class SOLIDSBCTESTSDK_API CSolidSBCTestManager
{
protected:
	CSolidSBCTestManager(void);
	~CSolidSBCTestManager(void);

public:
	int StartTest     (const std::string& sTestName, CSolidSBCTestConfig* pTestConfig);
	int StartTestByXML(const std::string& sXML);
	int StopTest      (const std::string& sTestName);

	int GetTestNames  (std::vector<std::string>& vecTestnames);
	int GetTestResults(std::vector<CSolidSBCTestResult*>& vecResults);
	int GetTestDefaultConfigs(SSBC_CONF_MAP_TYPE& mapDefaultConfigs);
	CSolidSBCTestConfig* GetTestDefaultConfigByName(const std::string& sTestName);
	int GetRegisteredTestCount(void) { return m_mapSolidSBCTestThreads.size(); };

protected:
	void AddTest(const std::string& sTestName, const AFX_THREADPROC pThreadFunc, CSolidSBCTestConfig* pDefaultConfig);

private:
	CSolidSBCTestThread* GetThreadByName(const std::string& sTestName);
	SSBC_TEST_MAP_TYPE   m_mapSolidSBCTestThreads;
	SSBC_CONF_MAP_TYPE   m_mapDefaultConfigs;
};