#ifdef SOLIDSBCTESTSDK_EXPORTS
	#define SOLIDSBCTESTSDK_API __declspec(dllexport)
#else
	#define SOLIDSBCTESTSDK_API __declspec(dllimport)
#endif

#include <string>
#include <map>
#include <vector>

class CSolidSBCTestThread;

class SOLIDSBCTESTSDK_API CSolidSBCTestResult
{
protected:
	CSolidSBCTestResult(void) {};
	~CSolidSBCTestResult(void){};

public:
	virtual std::string ToSQL(void)    = 0;
	virtual byte*       ToBytes(void)  = 0;
	virtual std::string ToString(void) = 0;
};

typedef std::pair< CSolidSBCTestThread*, std::vector<CSolidSBCTestResult*>* > SSBC_TEST_PAIR_TYPE;
typedef std::map< std::string, SSBC_TEST_PAIR_TYPE >                          SSBC_TEST_MAP_TYPE;

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