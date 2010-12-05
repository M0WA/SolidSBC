#include "stdafx.h"
#include "SolidSBCTestThreadMemory.h"
#include "SolidSBCPerformanceCounter.h"

#pragma optimize( "", off )

BOOL VMPerfTestThreadMemoryShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd)
{
	BOOL bReturn = TRUE;
	pcMutex->Lock();
	bReturn = *pbShallEnd;
	pcMutex->Unlock();
	return bReturn;
}

UINT VMPerfTestThreadMemory(LPVOID lpParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lpParam;
	PVMPERF_TEST_MEM_THREAD_PARAM  pThreadParam = (PVMPERF_TEST_MEM_THREAD_PARAM)pParam->threadParam;

	if ( pThreadParam->bRandomize ){
		UINT nDiff = (UINT)pThreadParam->nMaxMemory - (UINT)pThreadParam->nMinMemory;
		UINT number,nRandomNumber;

		while ( 1 ){
			rand_s( &number );
			nRandomNumber =  number % (nDiff + 1);
			nRandomNumber += pThreadParam->nMinMemory;

			CPerformanceCounter cMallocZeroCnt;
			
			ULONG ulMallocZeroBytes = (ULONG)nRandomNumber;
			cMallocZeroCnt.Start();
			PBYTE pMem = new BYTE[ulMallocZeroBytes];
			ZeroMemory(pMem,ulMallocZeroBytes);
			double dMallocZeroDuration = cMallocZeroCnt.Stop();

			//send result //TODO: !!!!!!!!!!!!!!! limit msg/seconds !!!!!!!!!!!!!!!!
			if ( pThreadParam->bTransmitData ) {

				PSSBC_MEMORY_TESTRESULT_PACKET pResult = new SSBC_MEMORY_TESTRESULT_PACKET;
				pResult->dMallocZeroDuration = dMallocZeroDuration;
				pResult->ulBytes			 = ulMallocZeroBytes;

				
				PSSBC_TEST_RESULT_PACKET pPacket = new SSBC_TEST_RESULT_PACKET;
				pPacket->pParam	    = pResult;
				pPacket->nParamSize = sizeof(SSBC_MEMORY_TESTRESULT_PACKET);
				pPacket->hdr.type   = SSBC_MEMORY_TESTRESULT_PACKET_TYPE;

				g_cClientService.SendTestResult(pPacket);
			}

			rand_s( &number );
			nRandomNumber =  number % (5000 + 1);

			//check every second if should exit
			double dSeconds = (double)nRandomNumber / 1000.0f;
			DWORD  dwMilliSeconds = ((ULONG)dSeconds) % (1000 + 1);
			for (ULONG i = 0; i < (ULONG)dSeconds; i++){
				if ( VMPerfTestThreadMemoryShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
					break;
				Sleep(970);}
			Sleep(dwMilliSeconds);

			delete pMem;

			if ( VMPerfTestThreadMemoryShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
				break;
		}
	}
	else{
		PBYTE pMem = new BYTE[pThreadParam->nMaxMemory];

		while( !VMPerfTestThreadMemoryShallEnd(pParam->pcStopMutex, pParam->pbShallStop) ){
			ZeroMemory(pMem,pThreadParam->nMaxMemory);
			Sleep(100);}

		delete [] pMem;
		pMem = NULL;
	}

	delete ((PVMPERF_TEST_MEM_THREAD_PARAM)pParam->threadParam);
	pParam->threadParam = NULL;
	return 0;
}
#pragma optimize( "", on )