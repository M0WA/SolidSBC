#include "stdafx.h"
#include "SolidSBCTestCPUMeasure.h"

CVMPerfTestCPUMeasure::CVMPerfTestCPUMeasure(void)
{
}

CVMPerfTestCPUMeasure::~CVMPerfTestCPUMeasure(void)
{
}

int CVMPerfTestCPUMeasure::StartCPUMeasure(HWND hMsgWnd, UINT nAdditionCount, UINT nDivisionCount, BOOL bTransmitData, UINT nAutoFixMultiplier, double dFixMultiplierTreshold )
{
	PVMPERF_TEST_CPUMEASURE_THREAD_PARAM pParam = new VMPERF_TEST_CPUMEASURE_THREAD_PARAM;
	pParam->nAdditionCnt			= nAdditionCount;
	pParam->nDivisionCnt			= nDivisionCount;
	pParam->bTransmitData			= bTransmitData;
	pParam->nAutoFixMultiplier		= nAutoFixMultiplier;
	pParam->dFixMultiplierTreshold	= dFixMultiplierTreshold;

	return CVMPerfTestBase::Start(&VMPerfTestThreadCPUMeasure,hMsgWnd,pParam);
}

int CVMPerfTestCPUMeasure::StopCPUMeasure(void)
{
	return CVMPerfTestBase::Stop();
}
