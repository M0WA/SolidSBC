#pragma once

#include "SolidSBCTestBase.h"
#include "SolidSBCTestThreadCPU.h"

class CVMPerfTestCPU : public CVMPerfTestBase
{
public:
	CVMPerfTestCPU(void);
	~CVMPerfTestCPU(void);
	int StartCPU(HWND hMsgWnd, BOOL bRandomize, UINT nMaxRand, double fSleepTime, UINT nThreadCnt);
	int StopCPU(void);
};
