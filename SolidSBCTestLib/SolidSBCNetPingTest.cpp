#include "StdAfx.h"
#include "SolidSBCNetPingTest.h"
#include "SolidSBCNetPingResult.h"
#include "SolidSBCNetPingConfig.h"

#include <Icmpapi.h>

UINT SolidSBCNetPingTest(LPVOID lpParam)
{
	USES_CONVERSION;
	
	PSSBC_TEST_THREAD_PARAM pParam          = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCNetPingConfig* pConfig	= (CSolidSBCNetPingConfig*)pParam->pTestConfig;

	DWORD                             dwTimeout		= pConfig->GetTTL();
    HANDLE                            hIcmpFile		= INVALID_HANDLE_VALUE;

	//find ip of target host
	unsigned long ipaddr = inet_addr( T2A( pConfig->GetHost() ) );
	if ( ipaddr == INADDR_NONE ){ //must be a hostname
		struct hostent* pHost = gethostbyname( T2A( pConfig->GetHost() ) );
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
    DWORD nReplySize     = sizeof( ICMP_ECHO_REPLY ) + pConfig->GetPayloadSize();
    PBYTE pReplyBuffer   = new BYTE[nReplySize];
	PBYTE pSendData      = new BYTE[pConfig->GetPayloadSize()];
	memset(pSendData, 0xFC, pConfig->GetPayloadSize());
	
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

				if ( pConfig->GetTransmitData() ){

					CSolidSBCNetPingResult* pResult = new CSolidSBCNetPingResult();
					pResult->SetDuration( pEchoReply->RoundTripTime );
					pResult->SetICMPOpts( (ULONGLONG)pEchoReply->Status );
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
		if (  (pConfig->GetInterval() > 0)
		   && (pConfig->GetInterval() > dwTimeout)
		   ) {
			UINT nDurationMS = static_cast<UINT>(cCnt.Stop() * 1000.0f);
			if ( nDurationMS < pConfig->GetInterval() ) {
				UINT nSleepMS = pConfig->GetInterval() - nDurationMS;
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