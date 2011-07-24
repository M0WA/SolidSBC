#include "StdAfx.h"
#include "SolidSBCNetTcpTest.h"
#include "SolidSBCNetTcpResult.h"
#include "SolidSBCNetTcpConfig.h"

UINT SolidSBCNetTcpTest(LPVOID lpParam)
{
	USES_CONVERSION;
	
	PSSBC_TEST_THREAD_PARAM pParam  = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCNetTcpConfig*  pConfig	= (CSolidSBCNetTcpConfig*)pParam->pTestConfig;
	DWORD                            dwTimeout	= pConfig->GetTTL();
	CPerformanceCounter cCnt;
	
	//preparing sockaddr for target
	struct sockaddr_in target;
	ZeroMemory(&target,sizeof(target));
	target.sin_family = AF_INET;
	target.sin_port   = htons(pConfig->GetPort() );

	//find ip of target host
	target.sin_addr.s_addr = inet_addr( T2A( pConfig->GetHost() ) );
	if ( target.sin_addr.s_addr == INADDR_NONE ){ //must be a hostname
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

			if ( ( target.sin_addr.s_addr = inet_addr( ip ) ) == INADDR_NONE ) {
				//TODO: notificate user that someting went wrong
				return 3;}
		}
		else {
			//TODO: notificate user that someting went wrong
			return 2;}
	}

	while ( 1 ) {

		if ( CSolidSBCTestThread::ShallThreadEnd(pParam) )
			break;

		fd_set writeFD;
		fd_set exceptFD;
		timeval selecttimeout;
		double dDurationMS = 0.0f;

		SOCKET hConnectSocket = socket(AF_INET,SOCK_STREAM,0);
		unsigned long iNonBlockMode = 1;
		ioctlsocket(hConnectSocket, FIONBIO, &iNonBlockMode); // put socket in non-blocking state

		//convert ttl to timeval struct
		if ( pConfig->GetTTL() > 1000 ) {
			selecttimeout.tv_sec  = static_cast<LONG>( ( static_cast<double>(pConfig->GetTTL()) / static_cast<double>(1000.0f) ) );
			selecttimeout.tv_usec = static_cast<LONG>( ( pConfig->GetTTL() - (selecttimeout.tv_sec * 1000) ) * 1000 );
		} 
		else if ( pConfig->GetTTL() == 1000 ) {
			selecttimeout.tv_sec  = 1;
			selecttimeout.tv_usec = 0;
		}
		else {
			selecttimeout.tv_sec  = 0;
			selecttimeout.tv_usec = static_cast<LONG>(pConfig->GetTTL() * 1000); //milli -> micro
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
							TRACE(_T("SolidSBCTestNetTcp(): should not end up here..."));
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

		if ( pConfig->GetTransmitData() ){

			CSolidSBCNetTcpResult* pResult = new CSolidSBCNetTcpResult();
			pResult->SetDuration(dDurationMS);

			CSolidSBCTestThread::AddResult(pParam, (CSolidSBCTestResult*)pResult);
		}

		//check if we are faster than nTcpConnInterval
		if (  (pConfig->GetInterval() > 0)
		   && (pConfig->GetInterval() > dwTimeout)
		   ) {
			UINT nDurationMS = static_cast<UINT>(dDurationMS);
			if ( nDurationMS < pConfig->GetInterval() ) {
				UINT nSleepMS = pConfig->GetInterval() - nDurationMS;
				//TODO: dont sleep too long here in once piece, 
				//      recheck regularly (every 25-50ms) if thread should end
				//      hangs on shutdown if "nSleepMS" is set too long
				Sleep(nSleepMS);
			}
		}
	}
	return 0;
}