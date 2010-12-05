#pragma once

#include "SolidSBCTestBase.h"
#include "SolidSBCTestThreadNetwork.h"

class CVMPerfTestNetwork : public CVMPerfTestBase
{
public:
	CVMPerfTestNetwork(void);
	~CVMPerfTestNetwork(void);
	int StartNetwork(HWND hMsgWnd, BOOL bEnablePingTest, BOOL bEnableTcpConnTest, VMPERF_TEST_NETWORK_PING_PARAM pingParam, VMPERF_TEST_NETWORK_TCPCONN_PARAM tcpconParam);
	int StopNetwork(void);
};
