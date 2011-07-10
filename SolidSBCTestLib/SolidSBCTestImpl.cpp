#include "StdAfx.h"
#include "SolidSBCTestImpl.h"

using namespace std;

CSolidSBCTestImpl::CSolidSBCTestImpl(void)
: CSolidSBCTestManager()
{
	AddTest("test-name1",0);
	AddTest("test-name2",0);
	AddTest("test-name3",0);
	AddTest("test-name4",0);
}