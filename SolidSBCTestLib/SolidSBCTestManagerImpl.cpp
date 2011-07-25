#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCCPUConfig.h"
#include "SolidSBCCPUMeasureConfig.h"
#include "SolidSBCMemoryConfig.h"
#include "SolidSBCHarddriveConfig.h"
#include "SolidSBCNetTcpConfig.h"
#include "SolidSBCNetPingConfig.h"

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
	AddTest( SSBC_CPU_TEST_NAME        , &SolidSBCCPUTest        , new CSolidSBCCPUConfig()        );
	AddTest( SSBC_CPUMEASURE_TEST_NAME , &SolidSBCCPUMeasureTest , new CSolidSBCCPUMeasureConfig() );
	AddTest( SSBC_MEMORY_TEST_NAME     , &SolidSBCMemoryTest     , new CSolidSBCMemoryConfig()     );
	AddTest( SSBC_HARDDRIVE_TEST_NAME  , &SolidSBCHarddriveTest  , new CSolidSBCHarddriveConfig()  );
	AddTest( SSBC_NETTCP_TEST_NAME     , &SolidSBCNetTcpTest     , new CSolidSBCNetTcpConfig()     );
	AddTest( SSBC_NETPING_TEST_NAME    , &SolidSBCNetPingTest    , new CSolidSBCNetPingConfig()    );
}
