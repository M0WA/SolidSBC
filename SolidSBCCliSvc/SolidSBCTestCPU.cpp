#include "stdafx.h"
#include "SolidSBCTestCPU.h"

CVMPerfTestCPU::CVMPerfTestCPU(void)
{
}

CVMPerfTestCPU::~CVMPerfTestCPU(void)
{
}

int CVMPerfTestCPU::StartCPU(HWND hMsgWnd, BOOL bRandomize, UINT nMaxRand, double fSleepTime, UINT nThreadCnt)
{
	PVMPERF_TEST_CPU_THREAD_PARAM pParam = new VMPERF_TEST_CPU_THREAD_PARAM;
	pParam->bRandomize = bRandomize;
	pParam->nMaxRand   = nMaxRand;
	pParam->fSleepTime = fSleepTime;
	pParam->nThreadCnt = nThreadCnt;

	return CVMPerfTestBase::Start(&VMPerfTestThreadCPU,hMsgWnd,pParam);
}

int CVMPerfTestCPU::StopCPU(void)
{
	return CVMPerfTestBase::Stop();
}