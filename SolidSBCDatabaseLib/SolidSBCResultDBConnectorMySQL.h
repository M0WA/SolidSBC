#pragma once

#include "SolidSBCResultDBConnector.h"
#include "defines.h"

class CSolidSBCResultDBConnectorMySQL : public CSolidSBCResultDBConnector
{
public:
	CSolidSBCResultDBConnectorMySQL				(void);
	virtual ~CSolidSBCResultDBConnectorMySQL	(void);
	virtual int Connect							(void);
	virtual int Disconnect						(void);
	virtual bool ExecStmts                      (const std::string& sSql);

	virtual int GetConfigsForClient(const CString& strClientUUID, std::vector<CString>& vecXmlConfigs);
	virtual int AddTestResult      (const CString& strClientUUID, const CString& strTestSQL);
	virtual int AddClientHistory   (const CString& strClientUUID, SSBC_CLIENT_HISTORY_ACTION nAction);

	virtual int GetNameFromUuid(const CString& strUuid, CString& strName);

private:
	void EnableMultistatment(bool bEnable = true);
};