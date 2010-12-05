#pragma once

#include "SolidSBCTestBase.h"
#include "SolidSBCTestThreadCPUMeasure.h"

class CVMPerfTestCPUMeasure : public CVMPerfTestBase
{
public:
	CVMPerfTestCPUMeasure(void);
	~CVMPerfTestCPUMeasure(void);
	int StartCPUMeasure(HWND hMsgWnd, UINT nAdditionCount, UINT nDivisionCount, BOOL bTransmitData, UINT nAutoFixMultiplier, double dFixMultiplierTreshold);
	int StopCPUMeasure(void);
};
