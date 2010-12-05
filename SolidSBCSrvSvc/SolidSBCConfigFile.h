#pragma once

class CSolidSBCConfigFile
{
public:
	CSolidSBCConfigFile(void);
	~CSolidSBCConfigFile(void);
	int Init(void);
	int GetParamInt(CString strKeyName, LPINT pnValue);
	int GetParamStr(CString strKeyName, CString* pstrValue);

private:
	CFile m_cConfigFile;
	BOOL  m_bIsInitialized;
};