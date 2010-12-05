#pragma once

#include <Winsvc.h>
#include "SolidSBCSrvServiceWnd.h"
#include "SolidSBCConfigFile.h"

class CSolidSBCServerService
{
public:
	CSolidSBCServerService(void);
	~CSolidSBCServerService(void);
	int  InstallService(void);
	int  DeleteService(void);
	bool IsServiceInstalled(void);
	int StartServerService(void);
	void SetImagePath(CString strImagePath);

private:
	void LoadParameters(void);
	void SaveParameters(void);

	CSolidSBCConfigClientSocket m_cConfigClientSocket;
	CSolidSBCResultClientSocket m_cResultClientSocket;
};
