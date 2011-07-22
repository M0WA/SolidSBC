#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCTestCPU.h"
#include "SolidSBCTestCPUMeasure.h"
#include "SolidSBCTestMemory.h"

CSolidSBCTestManagerImpl::CSolidSBCTestManagerImpl(void)
: CSolidSBCTestManager()
{
	AddTest( "CPU-Test",         &SolidSBCTestThreadCPU  );
	AddTest( "CPU-Measure-Test", &SolidSBCTestCPUMeasure );
	AddTest( "Memory-Test",      &SolidSBCTestMemory     );
	AddTest("test-name4", 0);
}