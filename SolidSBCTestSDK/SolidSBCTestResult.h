#pragma once

#include "SolidSBCTestSDK.h"

typedef struct {
	UINT   nPacketSize;
} SSBC_BASE_PACKET_HEADER, *PSSBC_BASE_PACKET_HEADER;

class SOLIDSBCTESTSDK_API CSolidSBCTestResult
{
protected:
	CSolidSBCTestResult(char* sTableName) : m_sTableName(sTableName) {};
	~CSolidSBCTestResult(void){};

	byte* GetPacketFromHeader(PSSBC_BASE_PACKET_HEADER pHeader);

	std::string GetTableName(void) { return m_sTableName; };
	virtual std::map<std::string,std::string> GetColumnsValues(void) = 0;

	std::string                       m_sTableName;
	std::map<std::string,std::string> m_mapColumnsValues;

public:
	std::string   ToSQL  (void);
	virtual byte* ToBytes(void) = 0;
};