#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCTestCPU.h"
#include "SolidSBCTestCPUMeasure.h"
#include "SolidSBCTestMemory.h"
#include "SolidSBCTestHarddrive.h"

CSolidSBCTestManagerImpl::CSolidSBCTestManagerImpl(void)
: CSolidSBCTestManager()
{
	//add tests that are available in this library
	AddTest( "CPU-Test"        , &SolidSBCTestCPU        );
	AddTest( "CPU-Measure-Test", &SolidSBCTestCPUMeasure );
	AddTest( "Memory-Test"     , &SolidSBCTestMemory     );
	AddTest( "Harddrive-Test"  , &SolidSBCTestHarddrive  );
}