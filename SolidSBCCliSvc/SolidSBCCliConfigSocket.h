#pragma once

class CSolidSBCCliConfigSocket : public CSolidSBCSocketClient
{
public:
	CSolidSBCCliConfigSocket();
	~CSolidSBCCliConfigSocket();

	void SetClientUUID(char* pszClientUUID) { m_pszClientUUID = pszClientUUID; }

	virtual bool OnConnect(bool bSuccess);
	virtual bool OnRead();

	int GetTestConfigXMLs(std::vector<CString>& vecTestConfigs) { vecTestConfigs = m_vecTestConfigs; 
																  return (int) m_vecTestConfigs.size(); }

private:
	bool SendConfigRequest(void);
	bool ReceiveConfigResponse(void);

	std::vector<CString> m_vecTestConfigs;
	char*                m_pszClientUUID;
};


