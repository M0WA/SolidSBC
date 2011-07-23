#include "StdAfx.h"
#include "SolidSBCTestHarddrive.h"
#include "SolidSBCHarddriveResult.h"

typedef struct {
	BOOL  bRandomRead;
	BOOL  bRandomWrite;
	ULONG ulReadMax;
	ULONG ulWriteMax;
	UINT  nReadWriteDelay;
	BOOL  bTransmitData;
} SSBC_HARDDRIVE_TEST_THREAD_PARAM, *PSSBC_HARDDRIVE_TEST_THREAD_PARAM;

#define SSBC_TEST_HARDDRIVE_TMP_WRITE_FILE	_T("C:\\tmpwrite.dat")
#define SSBC_TEST_HARDDRIVE_TMP_READ_FILE	_T("C:\\tmpread.dat")

#define SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE	1024 //4MB
#define SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ	1024 //4MB

#pragma optimize( "", off )

UINT SolidSBCTestThreadHarddriveReader(LPVOID lpParam);
UINT SolidSBCTestThreadHarddriveWriter(LPVOID lpParam);
void WaitForChildHarddriveThreads(CWinThread* pReaderThread, CWinThread*  pWriterThread);

UINT SolidSBCTestThreadHarddriveReader(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM pParam = (PSSBC_TEST_THREAD_PARAM)lpParam;
	PSSBC_HARDDRIVE_TEST_THREAD_PARAM pThreadParam = (PSSBC_HARDDRIVE_TEST_THREAD_PARAM)pParam->pThreadParam;

	ULONG ulReadBytes = pThreadParam->ulReadMax;
	unsigned int number = 0, nRandomNumber = 0;
	BYTE pBytes[SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ] = {0};
	memset(pBytes,0xFE,SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ);

	//create tmp file with size of pThreadParam->ulReadMax to read from
	TRY 
	{
		CFile cTmpFile;
		double dSeconds = 0;
		CPerformanceCounter cntTime;

		//start measuring
		cntTime.Start();

		cTmpFile.Open(SSBC_TEST_HARDDRIVE_TMP_READ_FILE,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		ULONG ulBytesWritten = 0, ulWriteBytesStep = SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ;
		for (ULONG i = 0; i <= ulReadBytes; i++){

			if ( (ulReadBytes - ulBytesWritten) < SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ ){
				ulWriteBytesStep = (ulReadBytes - ulBytesWritten);}

			cTmpFile.Write(pBytes,ulWriteBytesStep);
			ulBytesWritten += ulWriteBytesStep;}
		cTmpFile.Close();

		//stop measuring
		dSeconds = cntTime.Stop();

		//tell result to dialog
		if ( pThreadParam->bTransmitData ){
			CSolidSBCHarddriveResult* pResult = new CSolidSBCHarddriveResult();
			pResult->SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ_INIT);
			pResult->SetByteCount(ulReadBytes);
			pResult->SetWaitCount(pThreadParam->nReadWriteDelay);
			pResult->SetDuration(dSeconds);

			CSolidSBCTestThread::AddResult(pParam, (CSolidSBCTestResult*)pResult);
		}
	}
	CATCH(CException, e)
	{
		e->ReportError();
	}
	END_CATCH

	while ( !CSolidSBCTestThread::ShallThreadEnd( pParam ) )
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
			cTmpFile.Open(SSBC_TEST_HARDDRIVE_TMP_READ_FILE,CFile::typeBinary|CFile::modeRead|CFile::osNoBuffer);
			UINT nRead = 0;
			do {
				nRead = cTmpFile.Read(pBytes,SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ);
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

			CSolidSBCHarddriveResult* pResult = new CSolidSBCHarddriveResult();
			pResult->SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ);
			pResult->SetByteCount(ulReadBytes);
			pResult->SetWaitCount(pThreadParam->nReadWriteDelay);
			pResult->SetDuration(dSeconds);

			CSolidSBCTestThread::AddResult(pParam, (CSolidSBCTestResult*)pResult);
		}
		
		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd( pParam ) )
			break;
		
		//sleep for given interval and check every second if we should end...
		for (ULONG i = 0; i < pThreadParam->nReadWriteDelay; i++){
			Sleep(950);
			i++;
			if ( CSolidSBCTestThread::ShallThreadEnd( pParam ) )
				break;
		}

		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd( pParam ) )
			break;
	}

	//delete tmp file
	DeleteFile(SSBC_TEST_HARDDRIVE_TMP_READ_FILE);

	return 0;
}

UINT SolidSBCTestThreadHarddriveWriter(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM pParam = (PSSBC_TEST_THREAD_PARAM)lpParam;
	PSSBC_HARDDRIVE_TEST_THREAD_PARAM pThreadParam = (PSSBC_HARDDRIVE_TEST_THREAD_PARAM)pParam->pThreadParam;

	ULONG ulWriteBytes = pThreadParam->ulWriteMax;
	unsigned int number = 0, nRandomNumber = 0;
	BYTE pBytes[SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE] = {0};
	memset(pBytes,0xFE,SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE);

	while ( !CSolidSBCTestThread::ShallThreadEnd(pParam) )
	{
		if ( pThreadParam->bRandomWrite ) {
			rand_s( &number );
			ulWriteBytes = number % (pThreadParam->ulWriteMax+1);}
		
		CPerformanceCounter cntTime;
		ULONG ulBytesWritten = 0, ulWriteBytesStep = SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE;
		double dSeconds = 0;
		TRY 
		{
			CFile cTmpFile;

			//start measuring
			cntTime.Start();

			//write file
			cTmpFile.Open(SSBC_TEST_HARDDRIVE_TMP_WRITE_FILE,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::osWriteThrough);
			for (ULONG i = 0; i <= ulWriteBytes; i++){

				// check if next step is > SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE
				if ( (ulWriteBytes - ulBytesWritten) < SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE ){
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

			CSolidSBCHarddriveResult* pResult = new CSolidSBCHarddriveResult();
			pResult->SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE_WRITE);
			pResult->SetByteCount(ulBytesWritten);
			pResult->SetWaitCount(pThreadParam->nReadWriteDelay);
			pResult->SetDuration(dSeconds);

			CSolidSBCTestThread::AddResult(pParam, (CSolidSBCTestResult*)pResult);
		}

		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
			break;

		//sleep for given interval and check every second if we should end...
		for (ULONG i = 0; i < pThreadParam->nReadWriteDelay; i++){

			Sleep(900);
			i++;

			//end thread?
			if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
				break;
		}

		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
			break;
	}

	//delete tmp file
	DeleteFile(SSBC_TEST_HARDDRIVE_TMP_WRITE_FILE);

	return 0;
}

void WaitForChildHarddriveThreads(CWinThread* pReaderThread, CWinThread*  pWriterThread)
{
	if (pReaderThread){
		WaitForSingleObject(pReaderThread->m_hThread, INFINITE);}

	if (pWriterThread){
		WaitForSingleObject(pWriterThread->m_hThread, INFINITE);}
}

UINT SolidSBCTestHarddrive(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM pParam = (PSSBC_TEST_THREAD_PARAM)lpParam;
	PSSBC_HARDDRIVE_TEST_THREAD_PARAM pThreadParam = (PSSBC_HARDDRIVE_TEST_THREAD_PARAM)pParam->pThreadParam;

	CWinThread *pReaderThread = NULL, *pWriterThread = NULL;

	//start reader thread
	if (pThreadParam->ulReadMax > 0){
		pReaderThread = AfxBeginThread(	SolidSBCTestThreadHarddriveReader, lpParam, 0, 0, CREATE_SUSPENDED);
		pReaderThread->m_bAutoDelete = FALSE;
		pReaderThread->ResumeThread();
	}

	//start writer thread
	if (pThreadParam->ulWriteMax > 0){
		pWriterThread = AfxBeginThread(	SolidSBCTestThreadHarddriveWriter, lpParam, 0, 0, CREATE_SUSPENDED);
		pWriterThread->m_bAutoDelete = FALSE;
		pWriterThread->ResumeThread();
	}

	//stay alive and check for end
	while (1){
		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
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