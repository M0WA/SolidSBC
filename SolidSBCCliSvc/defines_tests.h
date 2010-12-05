#pragma once

#define VMPERFORMER_TEST_HARDDRIVE_TMP_WRITE_FILE	_T("C:\\tmpwrite.dat")
#define VMPERFORMER_TEST_HARDDRIVE_TMP_READ_FILE	_T("C:\\tmpread.dat")

#define VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_WRITE	1024 //4MB
#define VMPERF_TEST_HARDDRIVE_THREAD_BLOCKSIZE_READ		1024 //4MB

//parameter typedefs for worker threads
typedef struct {
	LPVOID  threadParam;
	CMutex* pcStopMutex;
	LPBOOL  pbShallStop;
} VMPERF_TEST_BASE_THREAD_PARAM,*PVMPERF_TEST_BASE_THREAD_PARAM ;

typedef struct {
	BOOL   bRandomize;
	UINT   nMaxRand;
	double fSleepTime;
	UINT   nThreadCnt;
} VMPERF_TEST_CPU_THREAD_PARAM,*PVMPERF_TEST_CPU_THREAD_PARAM ;

typedef struct {
	UINT   nDivisionCnt;
	UINT   nAdditionCnt;
	BOOL   bTransmitData;
	UINT   nAutoFixMultiplier;
	double dFixMultiplierTreshold;
} VMPERF_TEST_CPUMEASURE_THREAD_PARAM,*PVMPERF_TEST_CPUMEASURE_THREAD_PARAM ;

typedef struct {
	BOOL  bRandomize;
	ULONG nMinMemory;
	ULONG nMaxMemory;
	BOOL  bTransmitData;
} VMPERF_TEST_MEM_THREAD_PARAM,*PVMPERF_TEST_MEM_THREAD_PARAM ;

typedef struct {
	BOOL  bRandomRead;
	BOOL  bRandomWrite;
	ULONG ulReadMax;
	ULONG ulWriteMax;
	UINT  nReadWriteDelay;
	BOOL  bTransmitData;
} VMPERF_TEST_HARDDRIVE_THREAD_PARAM,*PVMPERF_TEST_HARDDRIVE_THREAD_PARAM ;

typedef struct {
	UINT	nPingInterval;
	TCHAR	szPingHost[SSBC_PROFILE_MAX_SERVER_NAME];
	UINT	nPingTTL;
	UINT    nPingPayloadSize;
	BOOL	bPingTransmitData;
} VMPERF_TEST_NETWORK_PING_PARAM, *PVMPERF_TEST_NETWORK_PING_PARAM;

typedef struct {
	UINT	nTcpConnInterval;
	TCHAR	szTcpConnHost[SSBC_PROFILE_MAX_SERVER_NAME];
	USHORT  nTcpConnPort;
	UINT	nTcpConnTTL;
	BOOL	bTcpConnTransmitData;
} VMPERF_TEST_NETWORK_TCPCONN_PARAM, *PVMPERF_TEST_NETWORK_TCPCONN_PARAM;

typedef struct {
	BOOL        bEnablePingTest;
	BOOL        bEnableTcpConnTest;
	VMPERF_TEST_NETWORK_PING_PARAM		pingParam;
	VMPERF_TEST_NETWORK_TCPCONN_PARAM	tcpconParam;
} VMPERF_TEST_NETWORK_THREAD_PARAM, *PVMPERF_TEST_NETWORK_THREAD_PARAM;

typedef struct {
	LPVOID* pParam;
	UINT    nSize;
} VMPERF_TEST_RESULT_MSG, *PVMPERF_TEST_RESULT_MSG;