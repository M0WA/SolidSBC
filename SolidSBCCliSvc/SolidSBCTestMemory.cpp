#include "stdafx.h"
#include "SolidSBCTestMemory.h"

CVMPerfTestMemory::CVMPerfTestMemory(void)
{
}

CVMPerfTestMemory::~CVMPerfTestMemory(void)
{
}


int CVMPerfTestMemory::StartMemory(HWND hMsgWnd, BOOL bRandomize,ULONG nMinMemory,ULONG nMaxMemory, BOOL bTransmitData)
{
	PVMPERF_TEST_MEM_THREAD_PARAM pParam = new VMPERF_TEST_MEM_THREAD_PARAM;
	pParam->bRandomize	= bRandomize;
	pParam->nMinMemory  = nMinMemory;
	pParam->nMaxMemory  = nMaxMemory;
	pParam->bTransmitData = bTransmitData;

	return CVMPerfTestBase::Start(&VMPerfTestThreadMemory,hMsgWnd,pParam);
}

int CVMPerfTestMemory::StopMemory(void)
{
	return CVMPerfTestBase::Stop();
}