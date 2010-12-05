#pragma once

//////////////////////////////////////////////////////////////////////
//definitions for array sizes
//////////////////////////////////////////////////////////////////////
#define SSBC_PROFILE_MAX_CLIENT_NAME			256
#define SSBC_PROFILE_MAX_PROFILE_NAME			512
#define SSBC_PROFILE_MAX_SERVER_NAME			512
#define SSBC_PROFILE_MAX_PEER_ADDR				20

//////////////////////////////////////////////////////////////////////
//configuration channel packets
//////////////////////////////////////////////////////////////////////
typedef struct {
	UINT    nProfileID;
	TCHAR	szClient[SSBC_PROFILE_MAX_CLIENT_NAME];
} SSBC_PROFILE_REQUEST_PACKET, *PSSBC_PROFILE_REQUEST_PACKET;

typedef struct {
	//profile settings
	UINT    nProfileID;
	TCHAR	szProfileName[SSBC_PROFILE_MAX_PROFILE_NAME];

	//harddrive settings
	BOOL bHDCfgEnable;
	BOOL bHDCfgRandomRead;
	BOOL bHDCfgRandomWrite;
	UINT nHDCfgMaxRead;
	UINT nHDCfgMaxWrite;
	UINT nHDCfgReadWriteDelay;
	BOOL bHDCfgTransmitData;

	//cpu settings
	BOOL   bCpuCfgEnable;
	BOOL   bCpuCfgRandom;
	UINT   nCpuCfgMaxRand;
	DOUBLE fCpuCfgSleepMS;
	UINT   nCpuCfgThreadCnt;

	//cpumeasure settings
	BOOL	bCpuMeasureCfgEnable;
	UINT	nCpuMeasureCfgAdditionCount;
	UINT	nCpuMeasureCfgDivisionCount;
	UINT	nCpuMeasureCfgTransmitData;
	UINT	nCpuMeasureCfgAutoFixMultipliers;
	DOUBLE	dCpuMeasureCfgFixMultipliersThreshold;

	//memory settings
	BOOL bMemCfgEnable;
	BOOL bMemCfgRandomize;
	UINT nMemCfgMinMem;
	UINT nMemCfgMaxMem;
	BOOL bMemCfgTransmitData;

	//network settings
	BOOL        bNetCfgEnablePingTest;
	BOOL        bNetCfgEnableTcpConnTest;
	UINT		nNetCfgPingInterval;
	TCHAR		szNetCfgPingHost[SSBC_PROFILE_MAX_SERVER_NAME];
	UINT		nNetCfgPingTTL;
	UINT		nNetCfgPingPayloadSize;
	BOOL		bNetCfgPingTransmitData;
	UINT		nNetCfgTcpConnInterval;
	TCHAR		szNetCfgTcpConnHost[SSBC_PROFILE_MAX_SERVER_NAME];
	USHORT		nNetCfgTcpConnPort;
	UINT		nNetCfgTcpConnTTL;
	BOOL		bNetCfgTcpConnTransmitData;

} SSBC_PROFILE_REPLY_PACKET, *PSSBC_PROFILE_REPLY_PACKET;

