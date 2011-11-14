#include "stdafx.h"
#include "SolidSBCTestResult.h"

#include <sstream>
#include <cctype>
#include <algorithm>

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

void CSolidSBCTestResult::RegisterAttributeDataType(const std::string& sAttributeName, const DataType dataType)
{
	if(dataType != DB_NONE)
	{
		std::string sDataType = "";
		DataTypeToString(sDataType,dataType);
		m_mapColumnsTypes[sAttributeName] = sDataType;
	}
}

std::string CSolidSBCTestResult::GetTestDBStructure(void)
{
	std::string sUpperTableName = m_sTableName;
	std::transform(sUpperTableName.begin(), sUpperTableName.end(), sUpperTableName.begin(), toupper); 
	std::stringstream ssCreateTableSQL;
	ssCreateTableSQL << "\r\nCREATE TABLE `" << m_sTableName << "`";
	ssCreateTableSQL << "\r\n(";
	ssCreateTableSQL << "\r\n\t`ID` INT NOT NULL AUTO_INCREMENT,";
	ssCreateTableSQL << "\r\n\t`CLIENT_ID` INT NOT NULL,";

	int nCount = 0;
	std::map<std::string,std::string>::iterator iIter = m_mapColumnsTypes.begin();
	for(nCount = 0;iIter != m_mapColumnsTypes.end(); iIter++, nCount++)
		ssCreateTableSQL << "\r\n\t`" << (*iIter).first << "` " << (*iIter).second << ", ";
	
	ssCreateTableSQL << "\r\n\tPRIMARY KEY (`ID`) ,";
	ssCreateTableSQL << "\r\n\tINDEX `FK_TESTLIB_MEMRESULT_CLIENT_ID` (`CLIENT_ID` ASC) ,";
	ssCreateTableSQL << "\r\n\tCONSTRAINT `FK_" << sUpperTableName << "_CLIENT_ID`";
    ssCreateTableSQL << "\r\n\tFOREIGN KEY (`CLIENT_ID` ) REFERENCES `Clients` (`ID` ) ON DELETE NO ACTION ON UPDATE NO ACTION";
	ssCreateTableSQL << "\r\n);\r\n";
	return nCount ? ssCreateTableSQL.str() : "";
}

bool CSolidSBCTestResult::DataTypeToString(std::string& strDataType, const DataType dataType)
{
	switch(dataType)
	{
	case DB_INT16:
		strDataType = "SMALLINT";
		break;
	case DB_INT32:
		strDataType = "INT";
		break;
	case DB_INT64:
		strDataType = "BIGINT";
		break;
	case DB_DOUBLE:
		strDataType = "DOUBLE";
		break;
	case DB_STRING255:
		strDataType = "CHAR(255)";
		break;
	case DB_STRING1024:
		strDataType = "VARCHAR(1024)";
		break;
	case DB_STRING4096:
		strDataType = "VARCHAR(4096)";
		break;
	default:
		return false;
	}

	return true;
}