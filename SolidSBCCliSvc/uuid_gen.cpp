#include "stdafx.h"
#include "uuid_gen.h"

CUuidGen::CUuidGen(void)
{
}

CUuidGen::~CUuidGen(void)
{
}

char* CUuidGen::CreateUUID(void)
{
	int nUUIDSize = 260; //uuid size is 256
	char* pszUUID = new char[nUUIDSize];
	memset(pszUUID,0x00,sizeof(nUUIDSize));

	srand( (unsigned int)time(NULL) );

	//
	// UUID version 4, specified in RFC 4122
	//
	sprintf_s(pszUUID, (size_t)nUUIDSize, "%0x%0x-%0x-%0x-%0x-%0x%0x%0x",
			rand32(), rand32(),				// generate 64-bit random number
			rand32(),						// generate 32-bit random number
			((rand32() & 0x0fff) | 0x4000), // generate 32-bit random number, form 4xxx (4 indicates UUID version)
			rand32() % 0x3fff + 0x8000,     // generate 32-bit random number, range [0x8000, 0xbfff]
			rand32(), rand32(), rand32());	// generate 96-bit random number

	return pszUUID;
}

unsigned int CUuidGen::rand32(void)
{
	unsigned char nRand1 = rand() % SSBC_CLIENT_UUID_MAX_RAND + 1;
	unsigned char nRand2 = rand() % SSBC_CLIENT_UUID_MAX_RAND + 1;
	unsigned char nRand3 = rand() % SSBC_CLIENT_UUID_MAX_RAND + 1;
	unsigned char nRand4 = rand() % SSBC_CLIENT_UUID_MAX_RAND + 1;

	unsigned int nReturn = 0;
	nReturn += (unsigned int)nRand1;
	nReturn = nReturn << 8;
	nReturn += (unsigned int)nRand2;
	nReturn = nReturn << 8;
	nReturn += (unsigned int)nRand3;
	nReturn = nReturn << 8;
	nReturn += (unsigned int)nRand4;

	return nReturn;
}
