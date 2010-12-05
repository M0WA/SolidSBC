#include "stdafx.h"
#include "SolidSBCTestHarddrive.h"

CVMPerfTestHarddrive::CVMPerfTestHarddrive(void)
{
}

CVMPerfTestHarddrive::~CVMPerfTestHarddrive(void)
{
}

int CVMPerfTestHarddrive::StartHarddrive(HWND hMsgWnd, BOOL bRandomRead, BOOL bRandomWrite, ULONG ulReadMax, ULONG ulWriteMax, UINT nReadWriteDelay, BOOL bTransmitData)
{
	PVMPERF_TEST_HARDDRIVE_THREAD_PARAM pParam = new VMPERF_TEST_HARDDRIVE_THREAD_PARAM;
	pParam->bRandomRead  = bRandomRead;
	pParam->bRandomWrite = bRandomWrite;
	pParam->ulReadMax    = ulReadMax;
	pParam->ulWriteMax   = ulWriteMax;
	pParam->nReadWriteDelay = nReadWriteDelay;
	pParam->bTransmitData   = bTransmitData;

	return CVMPerfTestBase::Start(&VMPerfTestThreadHarddrive,hMsgWnd,pParam);
}

int CVMPerfTestHarddrive::StopHarddrive(void)
{
	return CVMPerfTestBase::Stop();
}
