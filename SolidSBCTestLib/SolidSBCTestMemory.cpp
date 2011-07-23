#include "StdAfx.h"
#include "SolidSBCTestMemory.h"
#include "SolidSBCMemoryResult.h"

typedef struct {
	BOOL  bRandomize;
	ULONG nMinMemory;
	ULONG nMaxMemory;
	BOOL  bTransmitData;
} SSBC_MEMORY_TEST_THREAD_PARAM, *PSSBC_MEMORY_TEST_THREAD_PARAM;

#pragma optimize( "", off )

UINT SolidSBCTestMemory(LPVOID lpParam)
{	
	PSSBC_TEST_THREAD_PARAM pParam              = (PSSBC_TEST_THREAD_PARAM)lpParam;
	PSSBC_MEMORY_TEST_THREAD_PARAM pThreadParam = (PSSBC_MEMORY_TEST_THREAD_PARAM)pParam->pThreadParam;

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

				CSolidSBCMemoryResult* pResult = new CSolidSBCMemoryResult();
				pResult->SetMallocZeroDuration(dMallocZeroDuration);
				pResult->SetByteCount(ulMallocZeroBytes);

				CSolidSBCTestThread::AddResult(pParam,(CSolidSBCTestResult*)pResult);
			}

			rand_s( &number );
			nRandomNumber =  number % (5000 + 1);

			//check every second if should exit
			double dSeconds = (double)nRandomNumber / 1000.0f;
			DWORD  dwMilliSeconds = ((ULONG)dSeconds) % (1000 + 1);
			for (ULONG i = 0; i < (ULONG)dSeconds; i++){
				if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
					break;
				Sleep(970);}
			Sleep(dwMilliSeconds);

			delete pMem;

			if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
				break;
		}
	}
	else{
		PBYTE pMem = new BYTE[pThreadParam->nMaxMemory];

		while( !CSolidSBCTestThread::ShallThreadEnd(pParam) ){
			ZeroMemory(pMem,pThreadParam->nMaxMemory);
			Sleep(100);}

		delete [] pMem;
		pMem = NULL;
	}

	delete pThreadParam;
	pThreadParam = NULL;
	
	delete pParam;
	pParam = NULL;

	return 0;
}

#pragma optimize( "", on )