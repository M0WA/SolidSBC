#pragma once

#include "SolidSBCTestSDK.h"

#include <sstream>

typedef struct {
	UINT   nPacketSize;
} SSBC_BASE_PACKET_HEADER, *PSSBC_BASE_PACKET_HEADER;

class SOLIDSBCTESTSDK_API CSolidSBCTestResult
{
protected:
	CSolidSBCTestResult(char* sTableName) : m_sTableName(sTableName) {};

public:
	virtual ~CSolidSBCTestResult(void) {};

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

	//helper stuff
	byte* GetPacketFromHeader(PSSBC_BASE_PACKET_HEADER pHeader);

	std::string                       m_sTableName;
	std::map<std::string,std::string> m_mapColumnsValues;

public:
	std::string ToSQL  (void);
	byte*       ToBytes(void);
};