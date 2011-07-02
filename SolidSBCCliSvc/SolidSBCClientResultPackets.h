#pragma once

#include <icmpapi.h>
#include "SolidSBCClientConfigPackets.h"

#define SSBC_CLIENTUUID_SIZE 260


//////////////////////////////////////////////////////////////////////
//result/control data channel packets
//////////////////////////////////////////////////////////////////////
typedef enum SSBC_PACKET_TYPE {
	SSBC_INVALID_PACKET_TYPE						= 0,
	SSBC_CONN_RES_REQUEST_PACKET_TYPE				= 1,
	SSBC_PROFILE_CHANGE_REQUEST_PACKET_TYPE			= 2,
	SSBC_HD_TESTRESULT_PACKET_TYPE	 				= 3,
	SSBC_CPUMEASURE_TESTRESULT_PACKET_TYPE			= 4,
	SSBC_MEMORY_TESTRESULT_PACKET_TYPE				= 5,
	SSBC_NETWORK_PING_TESTRESULT_PACKET_TYPE		= 6,
	SSBC_NETWORK_TCPCON_TESTRESULT_PACKET_TYPE		= 7
};

typedef struct {
	SSBC_PACKET_TYPE type;
	UINT             nPacketSize;
} SSBC_BASE_PACKET_HEADER, *PSSBC_BASE_PACKET_HEADER;

typedef struct {
	UINT    nProfileID;
	TCHAR   szClientName[SSBC_PROFILE_MAX_CLIENT_NAME];   
	TCHAR   szClientUUID[SSBC_CLIENTUUID_SIZE];
	struct  sockaddr_in client; 	
} SSBC_CONN_RES_REQUEST_PACKET, *PSSBC_CONN_RES_REQUEST_PACKET;

typedef struct {
	UINT    nChangeToProfileID;
} SSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET, *PSSBC_RESULT_PROFILE_CHANGE_REQUEST_PACKET;

typedef enum SSBC_TEST_HARDDRIVE_RESULT_TYPE { 
	SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ	    = 0,
	SSBC_TEST_HARDDRIVE_RESULT_TYPE_WRITE	    = 1,
	SSBC_TEST_HARDDRIVE_RESULT_TYPE_READ_INIT   = 2
};

typedef struct {
	DOUBLE dSeconds;
	ULONG  ulBytes;
	ULONG  ulWait;
	SSBC_TEST_HARDDRIVE_RESULT_TYPE nType;
} SSBC_HD_TESTRESULT_PACKET,*PSSBC_HD_TESTRESULT_PACKET ;

typedef struct {
	DOUBLE		dAddDuration;
	DOUBLE		dDivDuration;
	DOUBLE		dOverallDuration;
	ULONGLONG	ullAddMultiplier;
	ULONGLONG	ullDivMultiplier;
} SSBC_CPUMEASURE_TESTRESULT_PACKET,*PSSBC_CPUMEASURE_TESTRESULT_PACKET ;

typedef struct {
	DOUBLE dMallocZeroDuration;
	ULONG  ulBytes;
} SSBC_MEMORY_TESTRESULT_PACKET,*PSSBC_MEMORY_TESTRESULT_PACKET ;

typedef struct {
	ICMP_ECHO_REPLY icmpReply;
} SSBC_NETWORK_PING_TESTRESULT_PACKET, *PSSBC_NETWORK_PING_TESTRESULT_PACKET;

typedef struct {
	DOUBLE dDuration;
} SSBC_NETWORK_TCPCON_TESTRESULT_PACKET, *PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET;
