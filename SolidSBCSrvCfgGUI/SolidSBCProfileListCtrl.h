#pragma once

#include "resource.h"
#include "SolidSBCProfileListEditCtrl.h"
#include "XMLFile.h"

// CProfileListCtrl
class CProfileListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CProfileListCtrl)

public:
	CProfileListCtrl();
	virtual ~CProfileListCtrl();
	int			InitStructure(CButton* pButton);
	int			SetActiveProfile(CString strProfileName);
	int			SetActiveProfile(UINT nProfileID);
	inline BOOL IsDirty(void){return m_bIsChanged;};
	int			SaveProfile(void);
	int			InitNewProfile(void);

	std::vector<CSolidSBCClientConfigProfile> m_vClientConfProfiles;

protected:
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

private:
	int		HitTestEx(CPoint &point, int *col) const;
	CEdit*	EditSubLabel( int nItem, int nCol );
	void	SetClean(void);
	int     InitProfiles(void);
	CString GetProfileFileNamePath(void);
	CString GetFilePath(void);

	BOOL	CheckBOOL(CString strToBoolean, BOOL bMsgBoxOnError = TRUE);
	BOOL	CheckINT(CString strToInteger, BOOL bMsgBoxOnError = TRUE);
	BOOL	CheckFLOAT(CString strToFloat, BOOL bMsgBoxOnError = TRUE);
	BOOL	 m_bIsInitialized;
	BOOL	 m_bIsChanged;
	CString	 m_strOldItemText;
	CButton* m_pApplyButton;

	CXMLFile m_xmlProfileConfigFile;
};

