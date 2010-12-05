#pragma once

#include "resource.h"

class CResultContextMenu : public CMenu
{
DECLARE_DYNCREATE(CResultContextMenu)

public:
	CResultContextMenu(void);
	~CResultContextMenu(void);

	void AddIdentities(CString strUUID, SSBC_RESULT_TYPE type, int nIdentity = -1);

private:
	int  FindMenuItem(LPCTSTR MenuString);
};

