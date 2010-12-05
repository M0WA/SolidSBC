#pragma once

/*
class CResultStack
{
public:
	CResultStack(void);
	~CResultStack(void);
	int Flush(void);
	int FlushCPUMeasure(void);
	int FlushHD(void);
	int AddHDResult(PVMPERFSRV_HD_TESTRESULT pResult);
	int AddCPUMeasureResult(PVMPERF_CPUMEASURE_TESTRESULT pResult);
	int AddMemResult(PVMPERF_MEM_TESTRESULT pResult);
	int AddNetPingResult(PVMPERF_NET_PING_TESTRESULT pResult);
	int AddNetTCPConResult(PVMPERF_NET_TCPCON_TESTRESULT pResult);
	int AddClientHistoryResult(CClientResultInfo* pResult, VMPERF_CLIENT_HISTORY_ACTION nAction, UINT nActionParam);

private:
	int FlushFile(CString strCompleteLog, CString strFileName);

	std::vector<VMPERFSRV_HD_TESTRESULT> m_vHDTestResult;
	ULONG m_lNextHDResultID;

	std::vector<VMPERF_CPUMEASURE_TESTRESULT > m_vCPUMeasureResult;
	ULONG m_lNextCPUMeasureResultID;
};
*/
