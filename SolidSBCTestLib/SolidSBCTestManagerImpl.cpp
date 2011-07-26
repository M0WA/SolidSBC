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
	AddTest( &SolidSBCCPUTest        , new CSolidSBCCPUConfig()        );
	AddTest( &SolidSBCCPUMeasureTest , new CSolidSBCCPUMeasureConfig() );
	AddTest( &SolidSBCMemoryTest     , new CSolidSBCMemoryConfig()     );
	AddTest( &SolidSBCHarddriveTest  , new CSolidSBCHarddriveConfig()  );
	AddTest( &SolidSBCNetTcpTest     , new CSolidSBCNetTcpConfig()     );
	AddTest( &SolidSBCNetPingTest    , new CSolidSBCNetPingConfig()    );
}
