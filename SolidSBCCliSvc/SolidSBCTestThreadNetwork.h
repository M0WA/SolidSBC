#pragma once

UINT VMTestThreadNetwork(LPVOID lParam);

//thread local helper functions and child threads
BOOL VMPerfTestThreadNetworkShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd);
UINT VMTestThreadNetworkTCPConnThread(LPVOID lParam);
UINT VMTestThreadNetworkPingThread(LPVOID lParam);