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
	AddTest( SSBC_CPU_TEST_NAME        , &SolidSBCCPUTest        );
	AddTest( SSBC_CPUMEASURE_TEST_NAME , &SolidSBCCPUMeasureTest );
	AddTest( SSBC_MEMORY_TEST_NAME     , &SolidSBCMemoryTest     );
	AddTest( SSBC_HARDDRIVE_TEST_NAME  , &SolidSBCHarddriveTest  );
	AddTest( SSBC_NETTCP_TEST_NAME     , &SolidSBCNetTcpTest     );
	AddTest( SSBC_NETPING_TEST_NAME    , &SolidSBCNetPingTest    );
}