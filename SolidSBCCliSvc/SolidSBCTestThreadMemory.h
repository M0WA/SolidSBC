#pragma once

//main thread
UINT VMPerfTestThreadMemory(LPVOID lpParam);

//helper functions
BOOL VMPerfTestThreadMemoryShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd);