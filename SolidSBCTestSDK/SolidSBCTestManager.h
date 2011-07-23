#pragma once

#include "SolidSBCTestSDK.h"

class SOLIDSBCTESTSDK_API CSolidSBCTestManager
{
protected:
	CSolidSBCTestManager(void);
	~CSolidSBCTestManager(void);

public:
	int StartTest     (const std::string& sTestName, LPVOID pszParam);
	int StopTest      (const std::string& sTestName);

	int GetTestNames  (std::vector<std::string>& vecTestnames);
	int GetTestResults(std::vector<CSolidSBCTestResult*>& vecResults);

protected:
	void AddTest(const std::string& sTestName, const AFX_THREADPROC pThreadFunc);

private:
	CSolidSBCTestThread* GetThreadByName(const std::string& sTestName);
	SSBC_TEST_MAP_TYPE m_mapSolidSBCTestThreads;
};