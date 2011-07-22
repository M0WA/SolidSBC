#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCTestCPU.h"
#include "SolidSBCTestMemory.h"

using namespace std;

CSolidSBCTestManagerImpl::CSolidSBCTestManagerImpl(void)
: CSolidSBCTestManager()
{
	AddTest( "CPU-Test"   , &SolidSBCTestThreadCPU );
	AddTest( "Memory-Test", &SolidSBCTestMemory    );
	AddTest("test-name3",0);
	AddTest("test-name4",0);
}