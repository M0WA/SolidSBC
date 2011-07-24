#include "StdAfx.h"
#include "SolidSBCNetPingTest.h"
#include "SolidSBCNetPingResult.h"

#include <Icmpapi.h>

#define SSBC_PROFILE_MAX_SERVER_NAME	512

typedef struct {
	UINT	nPingInterval;
	TCHAR	szPingHost[SSBC_PROFILE_MAX_SERVER_NAME];
	UINT	nPingTTL;
	UINT    nPingPayloadSize;
	BOOL	bPingTransmitData;
} SSBC_TEST_NETWORK_PING_PARAM, *PSSBC_TEST_NETWORK_PING_PARAM;

UINT SolidSBCNetPingTest(LPVOID lpParam)
{
	USES_CONVERSION;
	
	PSSBC_TEST_THREAD_PARAM pParam          = (PSSBC_TEST_THREAD_PARAM)lpParam;
	PSSBC_TEST_NETWORK_PING_PARAM pNetParam	= (PSSBC_TEST_NETWORK_PING_PARAM)pParam->pThreadParam;

	DWORD                             dwTimeout		= pNetParam->nPingTTL;
    HANDLE                            hIcmpFile		= INVALID_HANDLE_VALUE;

	//find ip of target host
	unsigned long ipaddr = inet_addr( T2A( pNetParam->szPingHost ) );
	if ( ipaddr == INADDR_NONE ){ //must be a hostname
		struct hostent* pHost = gethostbyname( T2A( pNetParam->szPingHost ) );
		if (   ( pHost )
			&& ( pHost->h_addrtype == 2 )
			&& ( pHost->h_length   == 4 )
		   ){
			char ip[50] = {0};
			sprintf_s(ip, 50, "%u.%u.%u.%u", 
				(unsigned char) pHost->h_addr_list[0][0],
				(unsigned char) pHost->h_addr_list[0][1],
				(unsigned char) pHost->h_addr_list[0][2],
				(unsigned char) pHost->h_addr_list[0][3]
			);

			if ( ( ipaddr = inet_addr( ip ) ) == INADDR_NONE ) {
				//TODO: notificate user that someting went wrong
				return 3;}
		}
		else {
			//TODO: notificate user that someting went wrong
			return 2;}
	}

	//calculate sizes and allocate buffer space
    DWORD nReplySize     = sizeof( ICMP_ECHO_REPLY ) + pNetParam->nPingPayloadSize;
    PBYTE pReplyBuffer   = new BYTE[nReplySize];
	PBYTE pSendData      = new BYTE[pNetParam->nPingPayloadSize];
	memset(pSendData, 0xFC, pNetParam->nPingPayloadSize);
	
	if ( (hIcmpFile = IcmpCreateFile()) == INVALID_HANDLE_VALUE ) {
		delete pSendData;       pSendData = NULL;
		delete pReplyBuffer; pReplyBuffer = NULL;
		return 1;}

	DWORD dwRetVal = 0;
	CPerformanceCounter cCnt;
	while( 1 ) {

		if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
			break;

		memset(pReplyBuffer, 0x00, nReplySize);

		//start counter for interval
		cCnt.Start();

		//send ping
		dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, pSendData, sizeof(pSendData), NULL, pReplyBuffer, nReplySize, dwTimeout);

		//valid icmp echo reply
		if ( dwRetVal ) {
			//loop though every reply message
			for ( DWORD dwPos = 0; dwPos < dwRetVal; dwPos++ ) {
				PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)&(pReplyBuffer[dwPos]);

				if ( pNetParam->bPingTransmitData ){

					CSolidSBCNetPingResult* pResult = new CSolidSBCNetPingResult();
					pResult->SetDuration( pEchoReply->RoundTripTime );

					CSolidSBCTestThread::AddResult( pParam, (CSolidSBCTestResult*) pResult );
				}
			}
		}
		else {
			//TODO: error handling for ping, maybe an internal error...
			switch ( GetLastError() ){
				case ERROR_INSUFFICIENT_BUFFER:
				case ERROR_INVALID_PARAMETER:
				case ERROR_NOT_ENOUGH_MEMORY:
				case ERROR_NOT_SUPPORTED:
				default:
					break;
			}
		}

		//check if we are faster than nPingInterval
		if (  (pNetParam->nPingInterval > 0)
		   && (pNetParam->nPingInterval > dwTimeout)
		   ) {
			UINT nDurationMS = static_cast<UINT>(cCnt.Stop() * 1000.0f);
			if ( nDurationMS < pNetParam->nPingInterval ) {
				UINT nSleepMS = pNetParam->nPingInterval - nDurationMS;
				//TODO: dont sleep too long here in once piece, 
				//      recheck regularly (every 25-50ms) if thread should end
				//      hangs on shutdown if "nSleepMS" is set too long
				Sleep(nSleepMS);
			}
		}
	}
	IcmpCloseHandle(hIcmpFile);

	//delete and null local buffers
	delete pSendData;       pSendData = NULL;
	delete pReplyBuffer; pReplyBuffer = NULL;

	return 0;
}