#pragma once

#include "SolidSBCTestSDK.h"
#include "../SolidSBCDatabaseLib/defines.h"

#include <sstream>
#include <string>

class SOLIDSBCTESTSDK_API CSolidSBCTestResult
{
protected:
	CSolidSBCTestResult(char* sTableName) : m_sTableName(sTableName) {};

public:
	virtual ~CSolidSBCTestResult(void) {};

	std::string ToSQL(void);
	std::string GetTestDBStructure(void);

protected:
	//setter
	template <class Tvalue> 
	void SetKeyValue(const std::string& sKey, const Tvalue& value)
	{
		std::stringstream sStream;
		sStream << std::fixed << value;
		m_mapColumnsValues[sKey] = sStream.str();
	}

	//getter
	template <class Tvalue>
	bool GetKeyValue(const std::string& sKey, Tvalue& Value, const Tvalue& DefaultValue)
	{
		Value = DefaultValue;

		if (!m_mapColumnsValues.count(sKey)) {
			return false; }

		std::istringstream in( m_mapColumnsValues[sKey]);
		return (in >> Value && in.eof());
	}

	void RegisterAttributeDataType(const std::string& sAttributeName, const DataType dataType);
	
private:
	//TODO: move this stuff to database library
	//      and implement it database type dependent (mysql,oracle etc)
	bool DataTypeToString(std::string& strDataType, const DataType dataType);

	std::string                       m_sTableName;
	std::map<std::string,std::string> m_mapColumnsValues;
	std::map<std::string,std::string> m_mapColumnsTypes;
};