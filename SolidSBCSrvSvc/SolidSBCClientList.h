#pragma once

#include "SolidSBCXMLFile.h"
#include "SolidSBCResultDBConnectorMySQL.h"
#include "SolidSBCConfigFile.h"

class CSolidSBCClientList
{
public:
	CSolidSBCClientList(void);
	~CSolidSBCClientList(void);
	int Init(void);
	void Empty(void);

	int AddResultRequest(SSBC_CONN_RES_REQUEST_PACKET packet);

	int AddResultClient(CSolidSBCClientResultInfo clientInfo);
	int DeleteResultClient(int nClientID);

	SSBC_PROFILE_REPLY_PACKET GetProfileReplyPacket(UINT nProfileID);

	CString GetProfileNameByID(UINT nProfileID);
	int GetProfileIDByName(CString strProfileName, UINT* pnProfileID);
	int GetClientResultInfoBySockAddr(struct sockaddr_in nClientAddr, CSolidSBCClientResultInfo** ppInfo);
	int GetClientResultInfoByClientID(UINT nClientID, CSolidSBCClientResultInfo** ppInfo);
	int GetClientResultInfos(std::vector<CSolidSBCClientResultInfo>* pClientResultInfos){m_cClientConfResultMutex.Lock();*pClientResultInfos = m_vClientResultInfos;m_cClientConfResultMutex.Unlock();return 0;};
	int GetClientConfigProfiles(std::vector<CSolidSBCClientConfigProfile>* pClientConfigProfiles){m_cClientConfProfilesMutex.Lock();*pClientConfigProfiles = m_vClientConfProfiles;m_cClientConfProfilesMutex.Unlock();return 0;};

	void ChangeClientProfile(UINT nClientID, UINT nNewProfileID);
	
	int AddHDResult				(PSSBC_HD_TESTRESULT pResult);
	int AddCPUMeasureResult		(PSSBC_CPUMEASURE_TESTRESULT pResult);
	int AddMemResult			(PSSBC_MEM_TESTRESULT pResult);
	int AddNetPingResult		(PSSBC_NET_PING_TESTRESULT pResult);
	int AddNetTCPConResult		(PSSBC_NET_TCPCON_TESTRESULT pResult);

private:
	CString GetProfileFileNamePath(void);
	int		InitDBConnection(void);
	int		InitProfiles(void);

	std::vector<CSolidSBCClientResultInfo>      m_vClientResultInfos;
	CMutex										m_cClientConfResultMutex;
	std::vector<CSolidSBCClientConfigProfile>   m_vClientConfProfiles;
	CMutex							            m_cClientConfProfilesMutex;
	CSolidSBCXMLFile				            m_xmlProfileConfigFile;
	CSolidSBCConfigFile							m_cConfigFile;
	CSolidSBCResultDBConnector*					m_pResultDBConnector;
};

extern CSolidSBCClientList g_cClientList;
