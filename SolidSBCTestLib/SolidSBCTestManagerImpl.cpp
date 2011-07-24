#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCCPUTest.h"
#include "SolidSBCCPUMeasureTest.h"
#include "SolidSBCMemoryTest.h"
#include "SolidSBCHarddriveTest.h"
#include "SolidSBCNetTcpTest.h"
#include "SolidSBCNetPingTest.h"

CSolidSBCTestManagerImpl::CSolidSBCTestManagerImpl(void)
: CSolidSBCTestManager()
{
	//add tests that are available in this library
	AddTest( "CPU-Test"        , &SolidSBCCPUTest        );
	AddTest( "CPU-Measure-Test", &SolidSBCCPUMeasureTest );
	AddTest( "Memory-Test"     , &SolidSBCMemoryTest     );
	AddTest( "Harddrive-Test"  , &SolidSBCHarddriveTest  );
	AddTest( "Net-TCP-Test"    , &SolidSBCNetTcpTest     );
	AddTest( "Net-Ping-Test"   , &SolidSBCNetPingTest    );
}