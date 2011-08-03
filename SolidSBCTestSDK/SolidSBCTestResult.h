#pragma once

#include "SolidSBCTestSDK.h"

#include <sstream>

class SOLIDSBCTESTSDK_API CSolidSBCTestResult
{
protected:
	CSolidSBCTestResult(char* sTableName) : m_sTableName(sTableName) {};

public:
	virtual ~CSolidSBCTestResult(void) {};

	std::string ToSQL(void);

protected:
	//setter
	template <class Tvalue> 
	void SetKeyValue(const std::string& sKey, const Tvalue& value)
	{
		std::stringstream sStream;
		sStream << value;
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
	
private:
	std::string                       m_sTableName;
	std::map<std::string,std::string> m_mapColumnsValues;
};