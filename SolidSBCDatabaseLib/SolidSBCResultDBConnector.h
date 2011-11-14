#pragma once

#include "defines.h"
#include "SolidSBCDatabaseLib.h"

class SOLIDSBCDATABASELIB_API CSolidSBCResultDBConnector : public CObject
{
	////////////////////////////////////////////////////////////////////
	//
	//used by all
	//
public:
	CSolidSBCResultDBConnector(void);
	~CSolidSBCResultDBConnector(void);

	void SetConfig(CString strServer, DWORD nPort, CString strDatabase, CString strUser, CString strPassword);
	
	////////////////////////////////////////////////////////////////////
	//
	// pure virtual functions (need to be implemented by derived child classes)
	//
public:
	virtual int Connect	  (void)=0;
	virtual int Disconnect(void)=0;
	virtual bool ExecStmts(const std::string& sSql, const bool bMultipleStmts = false)=0;

	////////////////////////////////////////////////////////////////////
	//
	//used by client
	//
public:
	virtual int GetConfigsForClient(const CString& strClientUUID, std::vector<CString>& vecXmlConfigs)=0;
	virtual int AddTestResult      (const CString& strClientUUID, const CString& strTestSQL)=0;
	virtual int AddClientHistory   (const CString& strClientUUID, SSBC_CLIENT_HISTORY_ACTION nAction)=0;
	virtual int GetClients         (std::vector<std::pair<std::string,std::string > >& vecClients)=0;

protected:
	int GetAddTestResultSQLString   (const CString& strClientUUID, const CString& strTestSQL, CStringArray& arSQLCmds);
	int GetAddClientHistorySQLString(const CString& strClientUUID, const SSBC_CLIENT_HISTORY_ACTION nAction, CStringArray& arSQLCmds);
	int GetConfigsForClientSQLString(const CString& strClientUUID, CStringArray& arSQLCmds);
	int GetClientsSQLString         (CStringArray& arSQLCmds);
	
	////////////////////////////////////////////////////////////////////
	//
	//used by result viewer
	//
public:
	virtual int GetNameFromUuid(const CString& strUuid, CString& strName)=0;

	int GetAllUniqueClients(std::map<CString,CString>& mapClientIdUuid) { mapClientIdUuid = m_mapClientIdUuid ; return (int)m_mapClientIdUuid.size(); }

protected:
	int GetNameFromUuidSQLString(const CString& strUuid, CStringArray& arSQLCmds);
	
	////////////////////////////////////////////////////////////////////
	//
	//internal implementation of this class
	//
protected:
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