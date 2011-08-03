// SolidSBCDatabaseLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SolidSBCDatabaseLib.h"
#include "SolidSBCResultDBConnectorMySQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSolidSBCResultDBConnector* g_pDatabase = NULL;

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

void InitDatabase(SSBC_DB_TYPE type)
{
	if ( g_pDatabase ) 
	{
		g_pDatabase->Disconnect();
		delete g_pDatabase;
	}

	switch(type)
	{
	case SSBC_DB_TYPE_MYSQL:
		g_pDatabase = (CSolidSBCResultDBConnector*) new CSolidSBCResultDBConnectorMySQL();
		break;
	default:
		g_pDatabase = NULL;
		break;
	}
}

CSolidSBCResultDBConnector* GetDatabaseConnection(void)
{
	return g_pDatabase;
}