#include "stdafx.h"
#include "SolidSBCTestThreadNetwork.h"
#include "SolidSBCPerformanceCounter.h"

BOOL VMPerfTestThreadNetworkShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd)
{
	BOOL bReturn = TRUE;
	pcMutex->Lock();
	bReturn = *pbShallEnd;
	pcMutex->Unlock();
	return bReturn;
}

UINT VMTestThreadNetworkTCPConnThread(LPVOID lParam)
{
	USES_CONVERSION;

	PVMPERF_TEST_BASE_THREAD_PARAM    pThreadParam	= (PVMPERF_TEST_BASE_THREAD_PARAM)lParam;
	PVMPERF_TEST_NETWORK_THREAD_PARAM pNetParam		= (PVMPERF_TEST_NETWORK_THREAD_PARAM)pThreadParam->threadParam;
	VMPERF_TEST_NETWORK_TCPCONN_PARAM param			= pNetParam->tcpconParam;
	DWORD                             dwTimeout		= param.nTcpConnTTL;
	CPerformanceCounter cCnt;
	
	//preparing sockaddr for target
	struct sockaddr_in target;
	ZeroMemory(&target,sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port   = htons(param.nTcpConnPort);

	//find ip of target host
	target.sin_addr.s_addr = inet_addr( T2A( param.szTcpConnHost ) );
	if ( target.sin_addr.s_addr == INADDR_NONE ){ //must be a hostname
		struct hostent* pHost = gethostbyname( T2A( param.szTcpConnHost ) );
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

			if ( ( target.sin_addr.s_addr = inet_addr( ip ) ) == INADDR_NONE ) {
				//TODO: notificate user that someting went wrong
				return 3;}
		}
		else {
			//TODO: notificate user that someting went wrong
			return 2;}
	}

	while ( 1 ) {

		if ( VMPerfTestThreadNetworkShallEnd(pThreadParam->pcStopMutex,pThreadParam->pbShallStop) )
			break;

		fd_set writeFD;
		fd_set exceptFD;
		timeval selecttimeout;
		double dDurationMS = 0.0f;

		SOCKET hConnectSocket = socket(AF_INET,SOCK_STREAM,0);
		unsigned long iNonBlockMode = 1;
		ioctlsocket(hConnectSocket, FIONBIO, &iNonBlockMode); // put socket in non-blocking state

		//convert ttl to timeval struct
		if ( param.nTcpConnTTL > 1000 ) {
			selecttimeout.tv_sec  = static_cast<LONG>( ( static_cast<double>(param.nTcpConnTTL) / static_cast<double>(1000.0f) ) );
			selecttimeout.tv_usec = static_cast<LONG>( ( param.nTcpConnTTL - (selecttimeout.tv_sec * 1000) ) * 1000 );
		} 
		else if ( param.nTcpConnTTL == 1000 ) {
			selecttimeout.tv_sec  = 1;
			selecttimeout.tv_usec = 0;
		}
		else {
			selecttimeout.tv_sec  = 0;
			selecttimeout.tv_usec = static_cast<LONG>(param.nTcpConnTTL * 1000); //milli -> micro
		}

		cCnt.Start();
		int nRet = connect( hConnectSocket, (struct sockaddr*)&target, sizeof(target) );
		
		if ( nRet == SOCKET_ERROR ) {

			while ( 1 ) {

				BOOL bExitLoop = FALSE;
				int  nSelectReturn = 0;
				FD_ZERO(&writeFD);FD_ZERO(&exceptFD);
				FD_SET(hConnectSocket,&writeFD);FD_SET(hConnectSocket,&exceptFD);

				switch ( WSAGetLastError() )
				{

				case WSAEWOULDBLOCK:
					nSelectReturn = select( NULL, NULL, &writeFD, &exceptFD, &selecttimeout );

					if ( nSelectReturn > 0 ) {
						//check for sockets in fd sets
						if ( FD_ISSET(hConnectSocket,&writeFD) ) {
							//connect successful
							dDurationMS = cCnt.Stop() * 1000.0f;
						} 
						else if ( FD_ISSET(hConnectSocket,&exceptFD) ) {
							//connect not successful
							dDurationMS = cCnt.Stop() * 1000.0f;
						}
#ifdef _DEBUG
						else {
							//socket in fd_set but actual socket not found in either one of them => impossible
							TRACE(_T("VMTestThreadNetworkTCPConnThread(): should not end up here..."));
						}
#endif
						bExitLoop = TRUE;
					}
					else if ( nSelectReturn == 0 ){
						//select timed out
						bExitLoop = TRUE;
					}
					else if ( nSelectReturn == SOCKET_ERROR ){
						//some other more fancy type of error occured
						bExitLoop = TRUE;
					}
					break;

				default:
					//TODO: decide what to do here
					bExitLoop = TRUE;
					break;
				}

				//finished waiting...
				if ( bExitLoop ) {
					break;}
			}
		}
		closesocket(hConnectSocket);

		if ( param.bTcpConnTransmitData ){
			int nPacketSize = sizeof(SSBC_BASE_PACKET_HEADER) + sizeof(SSBC_NETWORK_TCPCON_TESTRESULT_PACKET);
			PBYTE pPacket = new byte[nPacketSize];
			ZeroMemory(pPacket,nPacketSize);

			((PSSBC_BASE_PACKET_HEADER)pPacket)->type = SSBC_NETWORK_TCPCON_TESTRESULT_PACKET_TYPE;
			((PSSBC_BASE_PACKET_HEADER)pPacket)->nPacketSize = nPacketSize;
			
			PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET pResult = ((PSSBC_NETWORK_TCPCON_TESTRESULT_PACKET)&pPacket[sizeof(SSBC_BASE_PACKET_HEADER)]);
			pResult->dDuration  = dDurationMS;

			g_cClientService.SendTestResult((PSSBC_BASE_PACKET_HEADER)pPacket);
		}

		//check if we are faster than nTcpConnInterval
		if (  (param.nTcpConnInterval > 0)
		   && (param.nTcpConnInterval > dwTimeout)
		   ) {
			UINT nDurationMS = static_cast<UINT>(dDurationMS);
			if ( nDurationMS < param.nTcpConnInterval ) {
				UINT nSleepMS = param.nTcpConnInterval - nDurationMS;
				//TODO: dont sleep too long here in once piece, 
				//      recheck regularly (every 25-50ms) if thread should end
				//      hangs on shutdown if "nSleepMS" is set too long
				Sleep(nSleepMS);
			}
		}
	}

	return 0;
}

UINT VMTestThreadNetworkPingThread(LPVOID lParam)
{
	USES_CONVERSION;

	PVMPERF_TEST_BASE_THREAD_PARAM    pThreadParam	= (PVMPERF_TEST_BASE_THREAD_PARAM)lParam;
	PVMPERF_TEST_NETWORK_THREAD_PARAM pNetParam		= (PVMPERF_TEST_NETWORK_THREAD_PARAM)pThreadParam->threadParam;
	VMPERF_TEST_NETWORK_PING_PARAM    param			= pNetParam->pingParam;
	DWORD                             dwTimeout		= param.nPingTTL;
    HANDLE                            hIcmpFile		= INVALID_HANDLE_VALUE;

	//find ip of target host
	unsigned long ipaddr = inet_addr( T2A( param.szPingHost ) );
	if ( ipaddr == INADDR_NONE ){ //must be a hostname
		struct hostent* pHost = gethostbyname( T2A( param.szPingHost ) );
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
    DWORD nReplySize     = sizeof( ICMP_ECHO_REPLY ) + param.nPingPayloadSize;
    PBYTE pReplyBuffer   = new BYTE[nReplySize];
	PBYTE pSendData      = new BYTE[param.nPingPayloadSize];
	memset(pSendData, 0xFC, param.nPingPayloadSize);
	
	if ( (hIcmpFile = IcmpCreateFile()) == INVALID_HANDLE_VALUE ) {
		delete pSendData;       pSendData = NULL;
		delete pReplyBuffer; pReplyBuffer = NULL;
		return 1;}

	DWORD dwRetVal = 0;
	CPerformanceCounter cCnt;
	while( 1 ) {

		if ( VMPerfTestThreadNetworkShallEnd(pThreadParam->pcStopMutex,pThreadParam->pbShallStop) )
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

				if ( param.bPingTransmitData ){
					int nPacketSize = sizeof(SSBC_BASE_PACKET_HEADER) + sizeof(SSBC_NETWORK_PING_TESTRESULT_PACKET);
					PBYTE pPacket = new byte[nPacketSize];
					ZeroMemory(pPacket,nPacketSize);

					((PSSBC_BASE_PACKET_HEADER)pPacket)->type = SSBC_NETWORK_PING_TESTRESULT_PACKET_TYPE;
					((PSSBC_BASE_PACKET_HEADER)pPacket)->nPacketSize = nPacketSize;
			
					PSSBC_NETWORK_PING_TESTRESULT_PACKET pResult = ((PSSBC_NETWORK_PING_TESTRESULT_PACKET)&pPacket[sizeof(SSBC_BASE_PACKET_HEADER)]);
					pResult->icmpReply = *pEchoReply;

					g_cClientService.SendTestResult((PSSBC_BASE_PACKET_HEADER)pPacket);
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
		if (  (param.nPingInterval > 0)
		   && (param.nPingInterval > dwTimeout)
		   ) {
			UINT nDurationMS = static_cast<UINT>(cCnt.Stop() * 1000.0f);
			if ( nDurationMS < param.nPingInterval ) {
				UINT nSleepMS = param.nPingInterval - nDurationMS;
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

UINT VMTestThreadNetwork(LPVOID lParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM    pParam    = (PVMPERF_TEST_BASE_THREAD_PARAM)   lParam;
	PVMPERF_TEST_NETWORK_THREAD_PARAM pNetParam = (PVMPERF_TEST_NETWORK_THREAD_PARAM)pParam->threadParam;

	CWinThread *pPingThread = NULL, *pTcpConnThread = NULL;

	if ( pNetParam->bEnablePingTest){
		//start ping thread
		pPingThread = AfxBeginThread(VMTestThreadNetworkPingThread,pParam,0,0,CREATE_SUSPENDED);
		pPingThread->m_bAutoDelete = FALSE;
		pPingThread->ResumeThread();
	}

	if ( pNetParam->bEnableTcpConnTest){
		//start tcp connection thread
		pTcpConnThread = AfxBeginThread(VMTestThreadNetworkTCPConnThread,pParam,0,0,CREATE_SUSPENDED);
		pTcpConnThread->m_bAutoDelete = FALSE;
		pTcpConnThread->ResumeThread();
	}

	if (pPingThread) {
		//wait for ping thread and delete it
		WaitForSingleObject(pPingThread->m_hThread,INFINITE);
		delete pPingThread;
		pPingThread = NULL;}
	
	if (pTcpConnThread) {
		//wait for tcp conn thread and delete it
		WaitForSingleObject(pTcpConnThread->m_hThread,INFINITE);
		delete pTcpConnThread;
		pTcpConnThread = NULL;}

	//wait till thread shall end
	while ( !VMPerfTestThreadNetworkShallEnd(pParam->pcStopMutex,pParam->pbShallStop) ){
		Sleep(500);}

	//delete parameters
	delete pNetParam;
	pNetParam = NULL;

	return 0;
}