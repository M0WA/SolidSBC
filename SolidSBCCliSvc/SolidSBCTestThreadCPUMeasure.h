#pragma once

UINT VMPerfTestThreadCPUMeasure( LPVOID lParam );
BOOL VMPerfTestThreadCPUMeasureShallEnd( CMutex* pcMutex, LPBOOL pbShallEnd );

//thread local helper functions
UINT VMPerfTestThreadCPUMeasureFixMultipliers( double dCnt, UINT nStepCnt, ULONGLONG* pullMtpl, double dFixMultiplierThreshold, UINT nMaxSecs );