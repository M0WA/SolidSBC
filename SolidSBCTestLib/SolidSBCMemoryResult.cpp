#include "StdAfx.h"
#include "SolidSBCMemoryResult.h"

#include <sstream>

std::map<std::string,std::string> CSolidSBCMemoryResult::GetColumnsValues(void)
{
	std::map<std::string,std::string> mapColumnsValues;

	std::stringstream sStream;
	sStream << GetMallocZeroDuration();
	mapColumnsValues["malloczeroduration"] = sStream.str();

	sStream.clear();
	sStream << GetByteCount();
	mapColumnsValues["bytes"] = sStream.str();

	return mapColumnsValues;
}

