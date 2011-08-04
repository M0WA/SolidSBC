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

	std::stringstream ssColumnsSQL, ssValuesSQL;
	ssColumnsSQL << "( `CLIENT_ID`";
	ssValuesSQL  << "( %s ";
	for( iterColumnValues  = m_mapColumnsValues.begin(); 
		 iterColumnValues != m_mapColumnsValues.end();
		 iterColumnValues++ )
	{
		ssColumnsSQL << ", `" << std::fixed << (*iterColumnValues).first  << "`";
		ssValuesSQL  << ", '" << std::fixed << (*iterColumnValues).second << "'";
	}
	ssColumnsSQL << " )";
	ssValuesSQL  << " )";
	
	//assemble sql string
	std::stringstream sStream;
	sStream << "INSERT INTO " << m_sTableName << " ";
	sStream << ssColumnsSQL.str() << " VALUES ";
	sStream << ssValuesSQL.str()  << " ;";

	return sStream.str();
}