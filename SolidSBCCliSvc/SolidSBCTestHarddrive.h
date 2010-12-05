#pragma once

#include "SolidSBCTestBase.h"
#include "SolidSBCTestThreadHarddrive.h"

class CVMPerfTestHarddrive : public CVMPerfTestBase
{
public:
	CVMPerfTestHarddrive(void);
	~CVMPerfTestHarddrive(void);
	int StartHarddrive(HWND hMsgWnd, BOOL bRandomRead, BOOL bRandomWrite, ULONG ulReadMax, ULONG ulWriteMax, UINT nReadWriteDelay, BOOL bTransmitData);
	int StopHarddrive(void);
};
