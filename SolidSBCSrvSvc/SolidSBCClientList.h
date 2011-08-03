#pragma once

#include "SolidSBCConfigFile.h"

class CSolidSBCClientList
{
public:
	CSolidSBCClientList(void);
	~CSolidSBCClientList(void);
	int Init(void);
	void Empty(void);
			
	int AddResultClient(const CSolidSBCClientResultInfo& clientInfo);
	int DeleteResultClient(int nClientID);
	int AddTestResult(const CSolidSBCClientResultInfo& clientInfo, const CString& strTestSQL);

	int GetConfigsForClient(const CString& strClientUUID, std::vector<CString>& vecXmlConfigs);

	int GetClientResultInfoBySockAddr(struct sockaddr_in nClientAddr, CSolidSBCClientResultInfo** ppInfo);
	int GetClientResultInfoByClientID(UINT nClientID, CSolidSBCClientResultInfo** ppInfo);
	int GetClientResultInfos(std::vector<CSolidSBCClientResultInfo>* pClientResultInfos){m_cClientResultInfoMutex.Lock();*pClientResultInfos = m_vClientResultInfos;m_cClientResultInfoMutex.Unlock();return 0;};

private:
	int	InitDBConnection(void);

	std::vector<CSolidSBCClientResultInfo>      m_vClientResultInfos;
	CMutex										m_cClientResultInfoMutex;
	CSolidSBCConfigFile							m_cConfigFile;
	CSolidSBCResultDBConnector*					m_pResultDBConnector;
};

extern CSolidSBCClientList g_cClientList;
