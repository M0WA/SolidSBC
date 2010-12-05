#pragma once

class CSolidSBCClientConfigProfile
{
public:
	CSolidSBCClientConfigProfile(void);
	~CSolidSBCClientConfigProfile(void);

//profile settings
	UINT m_nProfileID;
	CString m_strName;

//harddrive settings
	BOOL m_bHDCfgEnable;
	BOOL m_bHDCfgRandomRead;
	BOOL m_bHDCfgRandomWrite;
	ULONG m_nHDCfgReadMax;
	ULONG m_nHDCfgWriteMax;
	UINT  m_nHDCfgReadWriteDelay;
	BOOL  m_bHDCfgTransmitData;

//cpu settings
	BOOL   m_bCpuCfgEnable;
	BOOL   m_bCpuCfgRandom;
	UINT   m_nCpuCfgMaxRand;
	double m_fCpuCfgSleepMS;
	UINT   m_nCpuCfgThreadCnt;

//cpumeasure settings
	BOOL	m_bCpuMeasureCfgEnable;
	UINT	m_nCpuMeasureCfgAdditionCount;
	UINT	m_nCpuMeasureCfgDivisionCount;
	UINT	m_nCpuMeasureCfgTransmitData;
	UINT	m_nCpuMeasureCfgAutoFixMultipliers;
	double	m_dCpuMeasureCfgFixMultipliersThreshold;

//memory settings
	BOOL m_bMemCfgEnable;
	BOOL m_bMemCfgRandomize;
	UINT m_nMemCfgMinMem;
	UINT m_nMemCfgMaxMem;
	BOOL m_bMemCfgTransmitData;

//network settings
	BOOL  m_bNetCfgEnablePingTest;
	BOOL  m_bNetCfgEnableTcpConnTest;
	UINT  m_nNetCfgPingInterval;
	CString m_szNetCfgPingHost;
	UINT  m_nNetCfgPingTTL;
	UINT  m_nNetCfgPingPayloadSize;
	BOOL  m_bNetCfgPingTransmitData;
	UINT  m_nNetCfgTcpConnInterval;
	CString m_szNetCfgTcpConnHost;
	USHORT m_nNetCfgTcpConnPort;
	UINT m_nNetCfgTcpConnTTL;
	BOOL m_bNetCfgTcpConnTransmitData;
};
