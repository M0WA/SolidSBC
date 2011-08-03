#pragma once

#include "defines.h"
#include "SolidSBCDatabaseLib.h"

class SOLIDSBCDATABASELIB_API CSolidSBCResultDBConnector : public CObject
{
public:
	CSolidSBCResultDBConnector(void);
	~CSolidSBCResultDBConnector(void);

	void SetConfig(CString strServer, DWORD nPort, CString strDatabase, CString strUser, CString strPassword);

	//pure virtual functions (need to be implemented by derived child classes)
	virtual int Connect					(void)=0;
	virtual int Disconnect				(void)=0;
	
	virtual int GetConfigsForClient(const CString& strClientUUID, std::vector<CString>& vecXmlConfigs)=0;
	virtual int AddTestResult      (const CString& strClientUUID, const CString& strTestSQL)=0;
	virtual int AddClientHistory   (const CString& strClientUUID, SSBC_CLIENT_HISTORY_ACTION nAction)=0;

protected:
	int GetAddTestResultSQLString   (const CString& strClientUUID, const CString& strTestSQL, CStringArray& arSQLCmds);
	int GetAddClientHistorySQLString(const CString& strClientUUID, const SSBC_CLIENT_HISTORY_ACTION nAction, CStringArray& arSQLCmds);
	int GetConfigsForClientSQLString(const CString& strClientUUID, CStringArray& arSQLCmds);

	CMutex			m_DBConnMutex;
	CString			m_strServer;
	DWORD			m_nPort;
	CString			m_strDatabase;
	CString			m_strUser;
	CString			m_strPassword;
	SSBC_DB_TYPE	m_Type;

private:
	std::map<CString,CString> m_mapClientIdUuid;
};