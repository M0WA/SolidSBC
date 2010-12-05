#include "stdafx.h"
#include "SolidSBCTestNetwork.h"

CVMPerfTestNetwork::CVMPerfTestNetwork(void)
{
}

CVMPerfTestNetwork::~CVMPerfTestNetwork(void)
{
}

int CVMPerfTestNetwork::StartNetwork(HWND hMsgWnd, BOOL bEnablePingTest, BOOL bEnableTcpConnTest, VMPERF_TEST_NETWORK_PING_PARAM pingParam, VMPERF_TEST_NETWORK_TCPCONN_PARAM tcpconParam)
{
	PVMPERF_TEST_NETWORK_THREAD_PARAM pParam = new VMPERF_TEST_NETWORK_THREAD_PARAM;
	pParam->bEnablePingTest					 = bEnablePingTest;
	pParam->bEnableTcpConnTest				 = bEnableTcpConnTest;
	pParam->pingParam						 = pingParam;
	pParam->tcpconParam						 = tcpconParam;

	return CVMPerfTestBase::Start(&VMTestThreadNetwork,hMsgWnd,pParam);
}

int CVMPerfTestNetwork::StopNetwork(void)
{
	return CVMPerfTestBase::Stop();
}
