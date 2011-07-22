#pragma once

#ifdef SOLIDSBCTESTSDK_EXPORTS
	#define SOLIDSBCTESTSDK_API __declspec(dllexport)
#else
	#define SOLIDSBCTESTSDK_API __declspec(dllimport)
#endif

//common predefined stuff
#include "defines.h"

//needed by test dlls
#include "PerformanceCounter.h"
#include "SolidSBCTestResult.h"
#include "SolidSBCTestThread.h"

//needed by client
#include "SolidSBCTestManager.h"