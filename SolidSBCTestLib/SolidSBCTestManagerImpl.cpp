#include "StdAfx.h"
#include "SolidSBCTestManagerImpl.h"

#include "SolidSBCCPUConfig.h"
#include "SolidSBCCPUMeasureConfig.h"
#include "SolidSBCMemoryConfig.h"
#include "SolidSBCHarddriveConfig.h"
#include "SolidSBCNetTcpConfig.h"
#include "SolidSBCNetPingConfig.h"

#include "SolidSBCCPUMeasureResult.h"
#include "SolidSBCMemoryResult.h"
#include "SolidSBCHarddriveResult.h"
#include "SolidSBCNetPingResult.h"
#include "SolidSBCNetTcpResult.h"

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
	RegisterTest( &SolidSBCCPUTest        , new CSolidSBCCPUConfig()       , NULL );
	RegisterTest( &SolidSBCCPUMeasureTest , new CSolidSBCCPUMeasureConfig(), (CSolidSBCTestResult*)new CSolidSBCCPUMeasureResult() );
	RegisterTest( &SolidSBCMemoryTest     , new CSolidSBCMemoryConfig()    , (CSolidSBCTestResult*)new CSolidSBCMemoryResult()     );
	RegisterTest( &SolidSBCHarddriveTest  , new CSolidSBCHarddriveConfig() , (CSolidSBCTestResult*)new CSolidSBCHarddriveResult()  );
	RegisterTest( &SolidSBCNetTcpTest     , new CSolidSBCNetTcpConfig()    , (CSolidSBCTestResult*)new CSolidSBCNetTcpResult()     );
	RegisterTest( &SolidSBCNetPingTest    , new CSolidSBCNetPingConfig()   , (CSolidSBCTestResult*)new CSolidSBCNetPingResult()    );
}
