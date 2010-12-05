#include "StdAfx.h"
#include "ResultContextMenu.h"

IMPLEMENT_DYNCREATE(CResultContextMenu, CMenu)

CResultContextMenu::CResultContextMenu(void)
{
}

CResultContextMenu::~CResultContextMenu(void)
{
}

int CResultContextMenu::FindMenuItem(LPCTSTR MenuString)
{
   int count = GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
      if (GetMenuString(i, str, MF_BYPOSITION) &&
         (_tcscmp(str, MenuString) == 0))
         return i;
   }

   return -1;
}

void CResultContextMenu::AddIdentities(CString strUUID, SSBC_RESULT_TYPE type, int nIdentity)
{
	std::vector<SSBC_CLIENT_IDENTITY> identities = g_cDatabaseConnection.GetClientIdentities(strUUID);
	int nFilterItem   = FindMenuItem(_T("Filter"));
	CResultContextMenu* pFilterSub = (CResultContextMenu*)GetSubMenu( nFilterItem );

	if ( (nFilterItem != -1) && (pFilterSub) ){
		int nSingleItem = pFilterSub->FindMenuItem(_T("Single Run"));
		CMenu* pSingleRun = pFilterSub->GetSubMenu( nSingleItem );

		if  ( (nSingleItem != -1) && (pSingleRun) ){

			pSingleRun->DeleteMenu( ID_SINGLERUN_NA, MF_BYCOMMAND );

			std::vector<SSBC_CLIENT_IDENTITY>::iterator iter;
			int nCount = 0;
			for ( iter = identities.begin(); iter < identities.end(); iter++ ){

				int nResultCount = 0;
				switch ( type )
				{
				case SSBC_HARDDRIVE:
					nResultCount = g_cDatabaseConnection.GetHDResultCount(strUUID,nCount);
					break;
				case SSBC_MEMORY:
					nResultCount = g_cDatabaseConnection.GetMemResultCount(strUUID,nCount);
					break;
				case SSBC_CPUMEASURE:
					nResultCount = g_cDatabaseConnection.GetCPUMeasureResultCount(strUUID,nCount);
					break;
				case SSBC_PING:
					nResultCount = g_cDatabaseConnection.GetPingResultCount(strUUID,nCount);
					break;
				case SSBC_TCPCON:
					nResultCount = g_cDatabaseConnection.GetTCPResultCount(strUUID,nCount);
					break;
				default:
					break;
				}

				if ( nResultCount ) {
					CString strItem = _T("");
					strItem.Format(_T("id: %d, %d results, start: %s, stop %s"),(*iter).nClientID,nResultCount,(*iter).strBeginTime,(*iter).strEndTime);

					int nWindowMessage = nCount + SSBC_RESULT_CLIENT_CONTEXT_WM_START_INDEX;
					pSingleRun->AppendMenu(MF_STRING,nWindowMessage, strItem );
				}

				nCount++;
			}

		}
	}
}