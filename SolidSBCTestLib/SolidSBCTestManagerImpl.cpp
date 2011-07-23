#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCTestCPU.h"
#include "SolidSBCTestCPUMeasure.h"
#include "SolidSBCTestMemory.h"

CSolidSBCTestManagerImpl::CSolidSBCTestManagerImpl(void)
: CSolidSBCTestManager()
{
	//add tests that are available in this library
	AddTest( "CPU-Test"        , &SolidSBCTestThreadCPU  );
	AddTest( "CPU-Measure-Test", &SolidSBCTestCPUMeasure );
	AddTest( "Memory-Test"     , &SolidSBCTestMemory     );
}