#include "stdafx.h"
#include "SolidSBCTestResult.h"

#include <sstream>

std::string CSolidSBCTestResult::ToSQL(void)
{
	//check if result has values
	if ( m_sTableName.empty() || !m_mapColumnsValues.size() ) 
		return std::string();

	//loop over result values
	std::map<std::string,std::string>::iterator iterColumnValues;
	std::string sColumnsSQL = "( `CLIENT_ID`";
	std::string sValuesSQL  = "( %s ";
	for( iterColumnValues  = m_mapColumnsValues.begin(); 
		 iterColumnValues != m_mapColumnsValues.end();
		 iterColumnValues++ )
	{
		sColumnsSQL += ", `" + (*iterColumnValues).first  + "`";
		sValuesSQL  += ", '" + (*iterColumnValues).second + "'";
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