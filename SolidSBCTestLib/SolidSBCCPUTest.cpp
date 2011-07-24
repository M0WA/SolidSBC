#include "stdafx.h"
#include "SolidSBCCPUTest.h"
#include "SolidSBCCPUConfig.h"

#pragma optimize( "", off )

typedef struct {
	BOOL   bRandomize;
	UINT   nMaxRand;
	double fSleepTime;
	UINT   nThreadCnt;
} SSBC_CPU_TEST_THREAD_PARAM, *PSSBC_CPU_TEST_THREAD_PARAM;

UINT SolidSBCTestThreadCPUChildThread(LPVOID lpParam);
BOOL WaitForChildCPUThreads(UINT nCnt, CWinThread** pWinThreads);

UINT SolidSBCCPUTest(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM pParam  = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCCPUConfig*     pConfig = (CSolidSBCCPUConfig*)pParam->pTestConfig;

	CWinThread** pThreads = (CWinThread**)new CWinThread*[pConfig->GetThreadCnt()];

	for (UINT i = 0; i < pConfig->GetThreadCnt(); i++) {
		pThreads[i] = AfxBeginThread(SolidSBCTestThreadCPUChildThread,lpParam,0,0,CREATE_SUSPENDED);
		if ( pThreads[i] ){
			pThreads[i]->m_bAutoDelete = FALSE;
			pThreads[i]->ResumeThread();}
	}
	
	BOOL bWaitChild = WaitForChildCPUThreads(pConfig->GetThreadCnt(), pThreads);

	for (UINT i = 0; i < pConfig->GetThreadCnt(); i++) {
		delete pThreads[i];}
	delete [] pThreads;
	pThreads = NULL;
	
	delete pParam;
	pParam = NULL;

	delete pConfig;
	pConfig = NULL;

	return 0;
}

BOOL WaitForChildCPUThreads(UINT nCnt, CWinThread** pWinThreads)
{	
	PHANDLE hThreads = new HANDLE[nCnt];
	ZeroMemory(hThreads,sizeof(HANDLE)*nCnt);
	for (UINT i = 0; i < nCnt; i++){
		hThreads[i] = pWinThreads[i]->m_hThread;}

	DWORD dw = 0;
	do{
		dw = WaitForMultipleObjects((DWORD)nCnt,hThreads,TRUE,INFINITE);
	} while (dw != WAIT_OBJECT_0);
	delete [] hThreads;
	return TRUE;
}

UINT SolidSBCTestThreadCPUChildThread(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM     pParam       = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCCPUConfig*     pConfig = (CSolidSBCCPUConfig*)pParam->pTestConfig;

	//randomize cpu usage
	if (pConfig->GetRandomize()){
		unsigned int number = 0, nRandomNumber = 0;

		while(1){
			if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
				break;

			//how long should the pause interval last?
			rand_s( &number );
			nRandomNumber = number % (pConfig->GetMaxRand()+1);

			//make some pause before using cpu power, check for exit every second
			for ( UINT i = 0; i < nRandomNumber; i++ ){
				Sleep(950);
				if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
					break;
			}

			//how long shall cpu-intensive interval last?
			DWORD dwStart = GetTickCount();
			rand_s( &number );
			nRandomNumber = number % (pConfig->GetMaxRand()+1);
			nRandomNumber *= 1000;

			//use some cpu power
			while ( 1 ){
				if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
					break;
				if ( ( GetTickCount() - dwStart ) >= nRandomNumber )
					break;
			}
		}
	}
	// static cpu usage
	else {
		int i = 0;
		struct timeval waitstruct;
		long double fDecimalPlaces  = (long double)pConfig->GetSleepMS() - (long double)((long)pConfig->GetSleepMS());
		waitstruct.tv_sec           = (long) pConfig->GetSleepMS()/1000;
		waitstruct.tv_usec          = (long) ((long double)(fDecimalPlaces * 1000.0f * 1000.0f));

		while(1){
			if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
				break;

			//we do not need to check for an interval >1s here,
			//because this would be insanely high...
			select( NULL, NULL, NULL, NULL, &waitstruct);
		}
	}

	return 0;
}

#pragma optimize( "", on )