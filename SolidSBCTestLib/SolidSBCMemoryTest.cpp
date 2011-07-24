#include "StdAfx.h"
#include "SolidSBCMemoryTest.h"
#include "SolidSBCMemoryResult.h"
#include "SolidSBCMemoryConfig.h"

#pragma optimize( "", off )

UINT SolidSBCMemoryTest(LPVOID lpParam)
{	
	PSSBC_TEST_THREAD_PARAM pParam = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCMemoryConfig* pConfig = (CSolidSBCMemoryConfig*)pParam->pTestConfig;

	if ( pConfig->GetRandomize() ){
		UINT nDiff = (UINT)pConfig->GetMaxMem() - (UINT)pConfig->GetMinMem();
		UINT number,nRandomNumber;
	
		while ( 1 ){
			rand_s( &number );
			nRandomNumber =  number % (nDiff + 1);
			nRandomNumber += pConfig->GetMinMem();

			CPerformanceCounter cMallocZeroCnt;
			
			ULONG ulMallocZeroBytes = (ULONG)nRandomNumber;
			cMallocZeroCnt.Start();
			PBYTE pMem = new BYTE[ulMallocZeroBytes];
			ZeroMemory(pMem,ulMallocZeroBytes);
			double dMallocZeroDuration = cMallocZeroCnt.Stop();

			//send result //TODO: !!!!!!!!!!!!!!! limit msg/seconds !!!!!!!!!!!!!!!!
			if ( pConfig->GetTransmitData() ) {

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
		PBYTE pMem = new BYTE[pConfig->GetMaxMem()];

		while( !CSolidSBCTestThread::ShallThreadEnd(pParam) ){
			ZeroMemory(pMem,pConfig->GetMaxMem());
			Sleep(100);}

		delete [] pMem;
		pMem = NULL;
	}

	delete pConfig;
	pConfig = NULL;
	
	delete pParam;
	pParam = NULL;

	return 0;
}

#pragma optimize( "", on )