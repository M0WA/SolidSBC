#include "stdafx.h"
#include "SolidSBCPerformanceCounter.h"
#include "SolidSBCTestThreadHarddrive.h"

#pragma optimize( "", off )

UINT VMPerfTestThreadHarddriveReader(LPVOID lpParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM      pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lpParam;
	PVMPERF_TEST_HARDDRIVE_THREAD_PARAM pThreadParam = (PVMPERF_TEST_HARDDRIVE_THREAD_PARAM)pParam->threadParam;

	ULONG ulReadBytes = pThreadParam->ulReadMax;
	unsigned int number = 0, nRandomNumber = 0;
	BYTE pBytes[VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ] = {0};
	memset(pBytes,0xFE,VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ);

	//create tmp file with size of pThreadParam->ulReadMax to read from
	TRY 
	{
		CFile cTmpFile;
		double dSeconds = 0;
		CPerformanceCounter cntTime;

		//start measuring
		cntTime.Start();

		cTmpFile.Open(VMPERFORMER_TEST_HARDDRIVE_TMP_READ_FILE,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		ULONG ulBytesWritten = 0, ulWriteBytesStep = VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ;
		for (ULONG i = 0; i <= ulReadBytes; i++){

			if ( (ulReadBytes - ulBytesWritten) < VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ ){
				ulWriteBytesStep = (ulReadBytes - ulBytesWritten);}

			cTmpFile.Write(pBytes,ulWriteBytesStep);
			ulBytesWritten += ulWriteBytesStep;}
		cTmpFile.Close();

		//stop measuring
		dSeconds = cntTime.Stop();

		//tell result to dialog
		if ( pThreadParam->bTransmitData ){
			PSSBC_HD_TESTRESULT_PACKET pResult = new SSBC_HD_TESTRESULT_PACKET;
			pResult->nType    = SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ_INIT;
			pResult->ulBytes  = ulReadBytes;
			pResult->ulWait	  = pThreadParam->nReadWriteDelay;
			pResult->dSeconds = dSeconds;
			
			PSSBC_TEST_RESULT_PACKET pPacket = new SSBC_TEST_RESULT_PACKET;
			pPacket->pParam	    = pResult;
			pPacket->nParamSize = sizeof(SSBC_HD_TESTRESULT_PACKET);
			pPacket->hdr.type   = SSBC_HD_TESTRESULT_PACKET_TYPE;
			
			g_cClientService.SendTestResult(pPacket);
		}
	}
	CATCH(CException, e)
	{
		e->ReportError();
	}
	END_CATCH

	while ( !VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
	{
		double dSeconds = 0;
		CPerformanceCounter cntTime;

		if ( pThreadParam->bRandomRead ) {
			rand_s( &number );
			ulReadBytes = number % (pThreadParam->ulReadMax+1);}

		//read from file
		TRY 
		{
			CFile cTmpFile;
			//start measuring
			cntTime.Start();

			//read file
			cTmpFile.Open(VMPERFORMER_TEST_HARDDRIVE_TMP_READ_FILE,CFile::typeBinary|CFile::modeRead|CFile::osNoBuffer);
			UINT nRead = 0;
			do {
				nRead = cTmpFile.Read(pBytes,VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ);
			} while (nRead);
			cTmpFile.Close();

			//stop measuring
			dSeconds = cntTime.Stop();
		}
		CATCH(CException, e)
		{
			e->ReportError();
		}
		END_CATCH

		//tell result to dialog
		if ( pThreadParam->bTransmitData ){
			PSSBC_HD_TESTRESULT_PACKET pResult = new SSBC_HD_TESTRESULT_PACKET;
			pResult->nType    = SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ;
			pResult->ulBytes  = ulReadBytes;
			pResult->ulWait	  = pThreadParam->nReadWriteDelay;
			pResult->dSeconds = dSeconds;
			
			PSSBC_TEST_RESULT_PACKET pPacket = new SSBC_TEST_RESULT_PACKET;
			pPacket->pParam	    = pResult;
			pPacket->nParamSize = sizeof(SSBC_HD_TESTRESULT_PACKET);
			pPacket->hdr.type   = SSBC_HD_TESTRESULT_PACKET_TYPE;

			g_cClientService.SendTestResult(pPacket);
		}
		
		//end thread?
		if ( VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
			break;
		
		//sleep for given interval and check every second if we should end...
		for (ULONG i = 0; i < pThreadParam->nReadWriteDelay; i++){
			Sleep(950);
			i++;
			if ( VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
				break;
		}

		//end thread?
		if ( VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
			break;
	}

	//delete tmp file
	DeleteFile(VMPERFORMER_TEST_HARDDRIVE_TMP_READ_FILE);
	return 0;
}

UINT VMPerfTestThreadHarddriveWriter(LPVOID lpParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM      pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lpParam;
	PVMPERF_TEST_HARDDRIVE_THREAD_PARAM pThreadParam = (PVMPERF_TEST_HARDDRIVE_THREAD_PARAM)pParam->threadParam;

	ULONG ulWriteBytes = pThreadParam->ulWriteMax;
	unsigned int number = 0, nRandomNumber = 0;
	BYTE pBytes[VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE] = {0};
	memset(pBytes,0xFE,VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE);

	while ( !VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
	{
		if ( pThreadParam->bRandomWrite ) {
			rand_s( &number );
			ulWriteBytes = number % (pThreadParam->ulWriteMax+1);}
		
		CPerformanceCounter cntTime;
		ULONG ulBytesWritten = 0, ulWriteBytesStep = VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE;
		double dSeconds = 0;
		TRY 
		{
			CFile cTmpFile;

			//start measuring
			cntTime.Start();

			//write file
			cTmpFile.Open(VMPERFORMER_TEST_HARDDRIVE_TMP_WRITE_FILE,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::osWriteThrough);
			for (ULONG i = 0; i <= ulWriteBytes; i++){

				// check if next step is > VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE
				if ( (ulWriteBytes - ulBytesWritten) < VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE ){
					ulWriteBytesStep = (ulWriteBytes - ulBytesWritten);}

				cTmpFile.Write(pBytes,ulWriteBytesStep);
				ulBytesWritten += ulWriteBytesStep;}
			cTmpFile.Close();

			//stop measuring
			dSeconds = cntTime.Stop();
		}
		CATCH(CException, e)
		{
			e->ReportError();
		}
		END_CATCH
	
		//tell result to dialog
		if ( pThreadParam->bTransmitData ){
			PSSBC_HD_TESTRESULT_PACKET pResult = new SSBC_HD_TESTRESULT_PACKET;	
			pResult->nType    = SSBC_TEST_HARDDRIVE_RESULT_TYPE_WRITE;
			pResult->ulBytes  = ulBytesWritten;
			pResult->ulWait	  = pThreadParam->nReadWriteDelay;
			pResult->dSeconds = dSeconds;
			
			PSSBC_TEST_RESULT_PACKET pPacket = new SSBC_TEST_RESULT_PACKET;
			pPacket->pParam	    = pResult;
			pPacket->nParamSize = sizeof(SSBC_HD_TESTRESULT_PACKET);
			pPacket->hdr.type   =  SSBC_HD_TESTRESULT_PACKET_TYPE;
			
			g_cClientService.SendTestResult(pPacket);
		}

		//end thread?
		if ( VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
			break;

		//sleep for given interval and check every second if we should end...
		for (ULONG i = 0; i < pThreadParam->nReadWriteDelay; i++){

			Sleep(900);
			i++;

			//end thread?
			if ( VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
				break;
		}

		//end thread?
		if ( VMPerfTestThreadHarddriveShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) )
			break;
	}

	//delete tmp file
	DeleteFile(VMPERFORMER_TEST_HARDDRIVE_TMP_WRITE_FILE);
	return 0;
}

void WaitForChildHarddriveThreads(CWinThread* pReaderThread, CWinThread*  pWriterThread)
{
	if (pReaderThread){
		WaitForSingleObject(pReaderThread->m_hThread, INFINITE);}

	if (pWriterThread){
		WaitForSingleObject(pWriterThread->m_hThread, INFINITE);}
}

BOOL VMPerfTestThreadHarddriveShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd)
{
	BOOL bReturn = TRUE;

	pcMutex->Lock();
	bReturn = *pbShallEnd;
	pcMutex->Unlock();

	return bReturn;
}

UINT VMPerfTestThreadHarddrive(LPVOID lpParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM      pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lpParam;
	PVMPERF_TEST_HARDDRIVE_THREAD_PARAM pThreadParam = (PVMPERF_TEST_HARDDRIVE_THREAD_PARAM)pParam->threadParam;

	CWinThread *pReaderThread = NULL, *pWriterThread = NULL;

	//start reader thread
	if (pThreadParam->ulReadMax > 0){
		pReaderThread = AfxBeginThread(	VMPerfTestThreadHarddriveReader, lpParam, 0, 0, CREATE_SUSPENDED);
		pReaderThread->m_bAutoDelete = FALSE;
		pReaderThread->ResumeThread();
	}

	//start writer thread
	if (pThreadParam->ulWriteMax > 0){
		pWriterThread = AfxBeginThread(	VMPerfTestThreadHarddriveWriter, lpParam, 0, 0, CREATE_SUSPENDED);
		pWriterThread->m_bAutoDelete = FALSE;
		pWriterThread->ResumeThread();
	}

	//stay alive and check for end
	while (1){
		//end thread?
		if ( VMPerfTestThreadHarddriveShallEnd(pParam->pcStopMutex,pParam->pbShallStop) )
			break;
		Sleep(333);}

	//wait for child threads
	WaitForChildHarddriveThreads(pReaderThread, pWriterThread);

	//delete thread memories
	delete pReaderThread;
	pReaderThread = NULL;

	delete pWriterThread;
	pWriterThread = NULL;

	delete pThreadParam;
	pThreadParam = NULL;

	return 0; 
}

#pragma optimize( "", on )