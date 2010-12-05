#pragma once

//main thread
UINT VMPerfTestThreadCPU(LPVOID lpParam);

//helper functions
UINT VMPerfTestThreadCPUChildThread(LPVOID lpParam);
BOOL VMPerfTestThreadCPUShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd);
BOOL WaitForChildCPUThreads(UINT nCnt, CWinThread** pWinThreads);