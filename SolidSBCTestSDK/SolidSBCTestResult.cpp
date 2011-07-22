#include "stdafx.h"
#include "SolidSBCTestResult.h"

#include <sstream>

byte* CSolidSBCTestResult::GetPacketFromHeader(PSSBC_BASE_PACKET_HEADER pHeader)
{
	byte* pPacket = new byte[pHeader->nPacketSize];
	ZeroMemory(pPacket,pHeader->nPacketSize);
	memcpy(pPacket,pHeader,pHeader->nPacketSize);

	return pPacket;
}

std::string CSolidSBCTestResult::ToSQL(void)
{
	std::stringstream sStream;
	sStream << std::string("INSERT INTO ") << GetTableName() << " ";

	std::map<std::string,std::string> mapColumnsValues = GetColumnsValues();
	std::map<std::string,std::string>::iterator iterColumnValues;

	std::string sColumnsSQL = "( ";
	std::string sValuesSQL  = "( ";
	for( iterColumnValues = mapColumnsValues.begin(); 
		 iterColumnValues != mapColumnsValues.end();
		 iterColumnValues++ )
	{
		
		if (iterColumnValues != mapColumnsValues.begin()){
			sColumnsSQL += ", ";
			sValuesSQL  += ", ";
		}

		sColumnsSQL += "`" + (*iterColumnValues).first  + "`";
		sValuesSQL  += "'" + (*iterColumnValues).second + "'";
	}
	sColumnsSQL += " )";
	sValuesSQL  += " )";

	sStream << sColumnsSQL << " VALUES ";
	sStream << sValuesSQL  << " ;";

	return sStream.str();
}