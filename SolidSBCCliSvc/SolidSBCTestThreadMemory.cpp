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
				int nPacketSize = sizeof(SSBC_BASE_PACKET_HEADER) + sizeof(SSBC_MEMORY_TESTRESULT_PACKET);
				PBYTE pPacket = new byte[nPacketSize];
				ZeroMemory(pPacket,nPacketSize);

				((PSSBC_BASE_PACKET_HEADER)pPacket)->type = SSBC_MEMORY_TESTRESULT_PACKET_TYPE;
				((PSSBC_BASE_PACKET_HEADER)pPacket)->nPacketSize = nPacketSize;
			
				PSSBC_MEMORY_TESTRESULT_PACKET pResult = ((PSSBC_MEMORY_TESTRESULT_PACKET)&pPacket[sizeof(SSBC_BASE_PACKET_HEADER)]);
				pResult->dMallocZeroDuration = dMallocZeroDuration;
				pResult->ulBytes			 = ulMallocZeroBytes;

				g_cClientService.SendTestResult((PSSBC_BASE_PACKET_HEADER)pPacket);
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