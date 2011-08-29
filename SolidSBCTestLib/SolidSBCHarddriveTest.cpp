#include "StdAfx.h"
#include "SolidSBCHarddriveTest.h"
#include "SolidSBCHarddriveResult.h"
#include "SolidSBCHarddriveConfig.h"

#pragma optimize( "", off )

UINT SolidSBCTestThreadHarddriveReader(LPVOID lpParam);
UINT SolidSBCTestThreadHarddriveWriter(LPVOID lpParam);
void WaitForChildHarddriveThreads(CWinThread* pReaderThread, CWinThread*  pWriterThread);

#pragma warning( push )
#pragma warning ( disable : 4748 )
UINT SolidSBCTestThreadHarddriveReader(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM pParam = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCHarddriveConfig* pConfig = (CSolidSBCHarddriveConfig*)pParam->pTestConfig;

	ULONG ulReadBytes = pConfig->GetReadMax();
	unsigned int number = 0, nRandomNumber = 0;
	BYTE pBytes[SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ] = {0};
	memset(pBytes,0xFE,SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ);

	//create tmp file with size of pConfig->GetReadMax() to read from
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
		if ( pConfig->GetTransmitData() ){
			CSolidSBCHarddriveResult* pResult = new CSolidSBCHarddriveResult();
			pResult->SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ_INIT);
			pResult->SetByteCount(ulReadBytes);
			pResult->SetWaitCount(pConfig->GetReadWriteDelay());
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

		if ( pConfig->GetRandomRead() ) {
			rand_s( &number );
			ulReadBytes = number % (pConfig->GetReadMax()+1);}

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
		if ( pConfig->GetTransmitData() ){

			CSolidSBCHarddriveResult* pResult = new CSolidSBCHarddriveResult();
			pResult->SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ);
			pResult->SetByteCount(ulReadBytes);
			pResult->SetWaitCount(pConfig->GetReadWriteDelay());
			pResult->SetDuration(dSeconds);

			CSolidSBCTestThread::AddResult(pParam, (CSolidSBCTestResult*)pResult);
		}
		
		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd( pParam ) )
			break;
		
		//sleep for given interval and check every second if we should end...
		for (ULONG i = 0; i < pConfig->GetReadWriteDelay(); i++){
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
	CSolidSBCHarddriveConfig* pConfig = (CSolidSBCHarddriveConfig*)pParam->pTestConfig;

	ULONG ulWriteBytes = pConfig->GetWriteMax();
	unsigned int number = 0, nRandomNumber = 0;
	BYTE pBytes[SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE] = {0};
	memset(pBytes,0xFE,SSBC_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE);

	while ( !CSolidSBCTestThread::ShallThreadEnd(pParam) )
	{
		if ( pConfig->GetRandomWrite() ) {
			rand_s( &number );
			ulWriteBytes = number % (pConfig->GetWriteMax()+1);}
		
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
		if ( pConfig->GetTransmitData() ){

			CSolidSBCHarddriveResult* pResult = new CSolidSBCHarddriveResult();
			pResult->SetResultType(SSBC_TEST_HARDDRIVE_RESULT_TYPE_WRITE);
			pResult->SetByteCount(ulBytesWritten);
			pResult->SetWaitCount(pConfig->GetReadWriteDelay());
			pResult->SetDuration(dSeconds);

			CSolidSBCTestThread::AddResult(pParam, (CSolidSBCTestResult*)pResult);
		}

		//end thread?
		if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
			break;

		//sleep for given interval and check every second if we should end...
		for (ULONG i = 0; i < pConfig->GetReadWriteDelay(); i++){

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

#pragma warning( pop )

void WaitForChildHarddriveThreads(CWinThread* pReaderThread, CWinThread*  pWriterThread)
{
	if (pReaderThread){
		WaitForSingleObject(pReaderThread->m_hThread, INFINITE);}

	if (pWriterThread){
		WaitForSingleObject(pWriterThread->m_hThread, INFINITE);}
}

UINT SolidSBCHarddriveTest(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM pParam = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCHarddriveConfig* pConfig = (CSolidSBCHarddriveConfig*)pParam->pTestConfig;

	CWinThread *pReaderThread = NULL, *pWriterThread = NULL;

	//start reader thread
	if (pConfig->GetReadMax() > 0){
		pReaderThread = AfxBeginThread(	SolidSBCTestThreadHarddriveReader, lpParam, 0, 0, CREATE_SUSPENDED);
		pReaderThread->m_bAutoDelete = FALSE;
		pReaderThread->ResumeThread();
	}

	//start writer thread
	if (pConfig->GetWriteMax() > 0){
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

	delete pConfig;
	pConfig = NULL;

	return 0; 
}

#pragma optimize( "", on )