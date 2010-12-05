#include "stdafx.h"
#include "SolidSBCTestThreadCPU.h"


#pragma optimize( "", off )

BOOL VMPerfTestThreadCPUShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd)
{
	BOOL bReturn = TRUE;
	pcMutex->Lock();
	bReturn = *pbShallEnd;
	pcMutex->Unlock();
	return bReturn;
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

UINT VMPerfTestThreadCPUChildThread(LPVOID lpParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lpParam;
	PVMPERF_TEST_CPU_THREAD_PARAM  pThreadParam = (PVMPERF_TEST_CPU_THREAD_PARAM)pParam->threadParam;

	//randomize cpu usage
	if (pThreadParam->bRandomize){
		unsigned int number = 0, nRandomNumber = 0;

		while(1){
			if ( VMPerfTestThreadCPUShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
				break;

			//how long should the pause interval last?
			rand_s( &number );
			nRandomNumber = number % (pThreadParam->nMaxRand+1);

			//make some pause before using cpu power, check for exit every second
			for ( UINT i = 0; i < nRandomNumber; i++ ){
				Sleep(950);
				if ( VMPerfTestThreadCPUShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
					break;
			}

			//how long shall cpu-intensive interval last?
			DWORD dwStart = GetTickCount();
			rand_s( &number );
			nRandomNumber = number % (pThreadParam->nMaxRand+1);
			nRandomNumber *= 1000;

			//use some cpu power
			while ( 1 ){
				if ( VMPerfTestThreadCPUShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
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
		long double fDecimalPlaces  = (long double)pThreadParam->fSleepTime - (long double)((long)pThreadParam->fSleepTime);
		waitstruct.tv_sec           = (long) pThreadParam->fSleepTime/1000;
		waitstruct.tv_usec          = (long) ((long double)(fDecimalPlaces * 1000.0f * 1000.0f));

		while(1){
			if ( VMPerfTestThreadCPUShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
				break;

			//we do not need to check for an interval >1s here,
			//because this would be insanely high...
			select( NULL, NULL, NULL, NULL, &waitstruct);
		}
	}

	return 0;
}

UINT VMPerfTestThreadCPU(LPVOID lpParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lpParam;
	PVMPERF_TEST_CPU_THREAD_PARAM  pThreadParam = (PVMPERF_TEST_CPU_THREAD_PARAM)pParam->threadParam;
	
	CWinThread** pThreads = (CWinThread**)new CWinThread*[pThreadParam->nThreadCnt];

	for (UINT i = 0; i < pThreadParam->nThreadCnt; i++) {
		pThreads[i] = AfxBeginThread(VMPerfTestThreadCPUChildThread,lpParam,0,0,CREATE_SUSPENDED);
		if ( pThreads[i] ){
			pThreads[i]->m_bAutoDelete = FALSE;
			pThreads[i]->ResumeThread();}
	}

	BOOL bWaitChild = WaitForChildCPUThreads(pThreadParam->nThreadCnt, pThreads);

	for (UINT i = 0; i < pThreadParam->nThreadCnt; i++) {
		delete pThreads[i];}
	delete [] pThreads;
	pThreads = NULL;

	delete ((PVMPERF_TEST_CPU_THREAD_PARAM)pParam->threadParam);
	pParam->threadParam = NULL;
	return 0;
}

#pragma optimize( "", on )