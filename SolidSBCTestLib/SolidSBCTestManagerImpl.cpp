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
	RegisterTest( &SolidSBCCPUTest        , new CSolidSBCCPUConfig()        );
	RegisterTest( &SolidSBCCPUMeasureTest , new CSolidSBCCPUMeasureConfig() );
	RegisterTest( &SolidSBCMemoryTest     , new CSolidSBCMemoryConfig()     );
	RegisterTest( &SolidSBCHarddriveTest  , new CSolidSBCHarddriveConfig()  );
	RegisterTest( &SolidSBCNetTcpTest     , new CSolidSBCNetTcpConfig()     );
	RegisterTest( &SolidSBCNetPingTest    , new CSolidSBCNetPingConfig()    );
}
