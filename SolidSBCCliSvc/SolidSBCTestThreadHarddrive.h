#pragma once

//main thread
UINT VMPerfTestThreadHarddrive(LPVOID lpParam);

//helper functions
BOOL VMPerfTestThreadHarddriveShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd);
UINT VMPerfTestThreadHarddriveReader(LPVOID lpParam);
UINT VMPerfTestThreadHarddriveWriter(LPVOID lpParam);
void WaitForChildHarddriveThreads(CWinThread* pReaderThread, CWinThread*  pWriterThread);