#pragma once

#include "SolidSBCTestBase.h"
#include "SolidSBCTestThreadMemory.h"

class CVMPerfTestMemory : public CVMPerfTestBase
{
public:
	CVMPerfTestMemory(void);
	~CVMPerfTestMemory(void);
	int StartMemory(HWND hMsgWnd, BOOL bRandomize,ULONG nMinMemory,ULONG nMaxMemory, BOOL bTransmitData);
	int StopMemory(void);
};
