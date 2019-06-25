
// SuperListCtrl.h
#pragma once

#include "afxwin.h"
#include "afxcmn.h"

#include "MFCDoubleBuffer.h"

class CSuperListCtrl : public CListCtrl {
	LVCOLUMN stColumn;

protected:
	void PaintEx(CDC *pDC);  // 请在其它 cpp 文件中给出此函数的定义，可以绘图附加的样式

public:
	CSuperListCtrl(){stColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH;}


	BOOL SetColumnCount(int nCount);
	BOOL SetColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth);

	BOOL SetItemCount(int nItems);

	BOOL SetSelect(int nItem, BOOL bSelect = TRUE);
	BOOL GetSelect(int nItem) const;

	BOOL SetCheck(int nItem, BOOL bCheck = TRUE);
	BOOL GetCheck(int nItem) const;

	UINT GetCheckedCount() const;

	template <int nCheckMaxLen>
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};



// ================ member functions' definition ================

inline BOOL CSuperListCtrl::SetColumnCount(int nCount){
	int i;
	int ColN=GetHeaderCtrl()->GetItemCount();
	if (ColN<nCount){
		for (i=ColN;i<nCount;i++)
			if (InsertColumn(i,_T(""))==-1)
				return FALSE;
	} else if (ColN>nCount){
		for (i=ColN-1;i>=nCount;i--)
			if (!DeleteColumn(i))
				return FALSE;
	}
	return TRUE;
}


inline BOOL CSuperListCtrl::SetColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth){
	stColumn.pszText=(LPTSTR)lpszColumnHeading;
	stColumn.fmt=nFormat;
	stColumn.cx=nWidth;
	return CListCtrl::SetColumn(nCol,&stColumn);
}


inline BOOL CSuperListCtrl::SetItemCount(int nItems){
	ASSERT((GetStyle() & LVS_OWNERDATA) == 0);

	int i;
	int n=GetItemCount();
	if (n>nItems){
		for (i=n-1;i>=nItems;i--)
			if (!DeleteItem(i))
				return FALSE;
	} else if (n<nItems){
		for (i=n;i<nItems;i++)
			if (InsertItem(i,_T(""))==-1)
				return FALSE;
	}
	return TRUE;
}

inline BOOL CSuperListCtrl::SetSelect(int nItem, BOOL bSelect){
	return GetSelect(nItem)==bSelect || SetItemState(nItem, bSelect ? LVIS_SELECTED : 0 ,LVIS_SELECTED);
}

inline BOOL CSuperListCtrl::GetSelect(int nItem) const{
	return GetItemState(nItem,LVIS_SELECTED)!=0;
}

inline BOOL CSuperListCtrl::SetCheck(int nItem, BOOL bCheck){
	ASSERT((GetStyle() & LVS_OWNERDATA) == 0);
	return GetCheck(nItem)==bCheck || CListCtrl::SetCheck(nItem,bCheck);
}

inline BOOL CSuperListCtrl::GetCheck(int nItem) const{
	ASSERT((GetStyle() & LVS_OWNERDATA) == 0);
	return CListCtrl::GetCheck(nItem);
}

inline UINT CSuperListCtrl::GetCheckedCount() const{
	ASSERT((GetStyle() & LVS_OWNERDATA) == 0);
	UINT result=0;
	int i;
	int n=GetItemCount();
	for (i=0;i<n;i++)
		if (GetCheck(i))
			result++;
	return result;
}

template <int nCheckMaxLen>
inline BOOL CSuperListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText){
	ASSERT(nCheckMaxLen>0);
	ASSERT((GetStyle() & LVS_OWNERDATA) == 0);

	TCHAR szText[nCheckMaxLen];
	GetItemText(nItem,nSubItem,szText,nCheckMaxLen);

	if (_tcscmp(szText,lpszText)==0)
		return TRUE;

	return CListCtrl::SetItemText(nItem,nSubItem,lpszText);
}
inline BOOL CSuperListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText){
	ASSERT((GetStyle() & LVS_OWNERDATA) == 0);
	return CListCtrl::SetItemText(nItem,nSubItem,lpszText);
}


