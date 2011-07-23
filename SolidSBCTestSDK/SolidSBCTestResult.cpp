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
	//check if result has values
	if ( m_sTableName.empty() || !m_mapColumnsValues.size() ) 
		return std::string();

	//loop over result values
	std::map<std::string,std::string>::iterator iterColumnValues;
	std::string sColumnsSQL = "( ";
	std::string sValuesSQL  = "( ";
	for( iterColumnValues  = m_mapColumnsValues.begin(); 
		 iterColumnValues != m_mapColumnsValues.end();
		 iterColumnValues++ )
	{
		
		if (iterColumnValues != m_mapColumnsValues.begin()){
			sColumnsSQL += ", ";
			sValuesSQL  += ", ";
		}

		sColumnsSQL += "`" + (*iterColumnValues).first  + "`";
		sValuesSQL  += "'" + (*iterColumnValues).second + "'";
	}
	sColumnsSQL += " )";
	sValuesSQL  += " )";
	
	//assemble sql string
	std::stringstream sStream;
	sStream << "INSERT INTO " << m_sTableName << " ";
	sStream << sColumnsSQL << " VALUES ";
	sStream << sValuesSQL  << " ;";

	return sStream.str();
}