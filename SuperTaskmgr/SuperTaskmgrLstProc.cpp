
#include "stdafx.h"
#include "SuperTaskmgr.h"
#include "SuperTaskmgrDlg.h"
#include "SuperTaskmgrLstProc.h"

//�����б��е���Ŀ
void CSuperTaskmgrDlg::OnNMClickLstproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	UINT flag;
	int i,l;
	int nItem=pNMListView->iItem;//����������к�
	m_LstProc.HitTest(pNMListView->ptAction,&flag);
	if (flag==LVHT_ONITEMSTATEICON){//����������˸�ѡ��

		//���á�һ����������ť�Ŀɵ���
		if (!pListPro[nItem]->Checked)
			m_OneKeyKill.EnableWindow(TRUE);
		else {
			l=m_LstProc.GetItemCount();
			for (i=0;i<l;i++){
				if (i!=nItem){
					if (m_LstProc.GetCheck(i))
						break;
				}
			}
			m_OneKeyKill.EnableWindow(i!=l);
		}
	}
	*pResult = 0;
}

//�Ҽ��б��е���Ŀ
void CSuperTaskmgrDlg::OnNMRclickLstproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	POINT pCur;
	GetCursorPos(&pCur);
	m_Menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN,pCur.x,pCur.y,this);
	*pResult = 0;
}

//��������б�ͷ
void CSuperTaskmgrDlg::OnLvnColumnclickLstproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	SortColIndex=pNMLV->iSubItem; //���õ�ǰ������к�
	SortColItem=setting.column[SortColIndex].cindex;
	RevSort= LastSortColIndex==SortColIndex ? -RevSort : 1; //�ٴε����ǰ��ʱ����
	RefreshList(); //��ʼ����
	LastSortColIndex=SortColIndex; //��¼��һ�ε������к�
	*pResult = 0;
}

//�Ҽ������б�ͷ
void CSuperTaskmgrDlg::OnLvnColumnRClickLstproc(int col){
	xRightColumn=col;
	POINT pCur;
	GetCursorPos(&pCur);
	m_MenuNotify.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN,pCur.x,pCur.y,this);
}

//���˫���б��е���Ŀ
void CSuperTaskmgrDlg::OnNMDblclkLstproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int scindex=setting.column[pNMListView->iSubItem].cindex;
	if ((scindex==cNetRecvSpeed || scindex==cNetSendSpeed || scindex==cNetTotalSpeed) &&
		pNMListView->iItem!=-1 &&
		pListPro[pNMListView->iItem]->NetSendSpeed!=-2l &&
		pListPro[pNMListView->iItem]->NetRecvSpeed!=-2l)
	{
		if (pListPro[pNMListView->iItem]->Selected){
			VERIFY(TdiFilter.GetProcessLimited(&stProcessLimited));
			int j;
			for (j=0;j<stProcessLimited.nProcess;j++){
				if (_tcsicmp(stProcessLimited.limit[j].szPath,pListPro[pNMListView->iItem]->path)==0){
					break;
				}
			}
			if (j==stProcessLimited.nProcess){
				memset(&stProcessLimited.limit[j],0,sizeof(stProcessLimited.limit[j]));
			}
			LimitSpeedDlg.SetProcess(&stProcessLimited.limit[j],pListPro[pNMListView->iItem]);
			RECT rWinRect;
			GetWindowRect(&rWinRect);
			LimitSpeedDlg.SetWindowPos(this,
				(rWinRect.right+rWinRect.left)/2-80,
				(rWinRect.bottom+rWinRect.top)/2-80,
				0,0,
				SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
		}
	} else {
		int n=m_LstProc.GetItemCount();
		for (int i=0;i<n;i++){
			m_LstProc.SetSelect(i,FALSE);
			m_LstProc.SetCheck(i,FALSE);
		}
		m_LstProc.RedrawItems(0,n);
		m_OneKeyKill.EnableWindow(FALSE);
		*pResult = 0;
	}
}

void CSuperTaskmgrDlg::OnUpdateHideThisColumn(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(xRightColumn>=0 && xRightColumn<setting.ColumnNumber);
}

//���ظ���
void CSuperTaskmgrDlg::OnHideThisColumn()
{
	int i;
	for (i=xRightColumn;i<setting.ColumnNumber-1;i++)
		setting.column[i]=setting.column[i+1];
	setting.ColumnNumber--;
	m_LstProc.DeleteColumn(xRightColumn);
	xRightColumn=0;
}

//ѡ����
void CSuperTaskmgrDlg::OnSelectColumn()
{
	//������ѡ���С��Ի���
	CSelectColumnDlg scd;
	scd.psetting=&setting;
	scd.DoModal();
	InitListCtrl();
	Refresh(TRUE);
	RefreshColumnCFG();
	if (!SaveCFG()){
		ShowError(m_hWnd,AppName,_T("�޷��������ã�\n"),NULL,GetLastError());
	}
}

//�б���ĳ��Ŀѡ�е�״̬���ı�
void CSuperTaskmgrDlg::OnLvnItemchangedLstproc(NMHDR *pNMHDR, LRESULT *pResult)
{
#define SEL_CHK_FLAG (LVIS_SELECTED | INDEXTOSTATEIMAGEMASK(1) | INDEXTOSTATEIMAGEMASK(2))
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if ((pNMLV->uOldState & SEL_CHK_FLAG) == (pNMLV->uNewState & SEL_CHK_FLAG))
		goto end;

	int nItem=pNMLV->iItem;

	if (nItem>=nListItemCount)
		goto end;

	if(
		(pNMLV->uOldState & LVIS_SELECTED) == LVIS_SELECTED &&
		(pNMLV->uNewState & LVIS_SELECTED) == 0
	){ //��Ŀ��ȡ��ѡ��
		pListPro[nItem]->Selected=FALSE;
	} else if(
		(pNMLV->uOldState & LVIS_SELECTED) == 0 &&
		(pNMLV->uNewState & LVIS_SELECTED) == LVIS_SELECTED
	) { //��Ŀ��ѡ��
		pListPro[nItem]->Selected=TRUE;
		pListPro[nItem]->Checked=TRUE;
		m_LstProc.SetCheck(nItem,TRUE);
		m_OneKeyKill.EnableWindow(TRUE);
	}

	//����ѡ״̬
	if(
		(pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) &&
		(pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))
	){ //��Ŀ��ȡ����ѡ
		pListPro[nItem]->Checked=FALSE;
	} else if(
		(pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) &&
		(pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2))
	) { //��Ŀ����ѡ
		pListPro[nItem]->Checked=TRUE;
		m_OneKeyKill.EnableWindow(TRUE);
	}

end:
	*pResult = 0;
}

//׼����ͼ�б���ĳһ��
void CSuperTaskmgrDlg::OnNMCustomdrawLstproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW &LVCD = *reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;
	switch (LVCD.nmcd.dwDrawStage){
	case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
		break;
	case CDDS_ITEMPREPAINT:
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{
			CListDispInfo::DISP_CELL &stDispCell=
				m_ListInfo.GetList()->pdiaItem[LVCD.nmcd.dwItemSpec]->dcaCell[LVCD.iSubItem];
			LVCD.clrText=stDispCell.crText;
			LVCD.clrTextBk=stDispCell.crBack;
			SelectObject(LVCD.nmcd.hdc,stDispCell.hFont);
			*pResult = CDRF_DODEFAULT;
			break;
		}
	case CDDS_POSTPAINT:
		*pResult = CDRF_SKIPDEFAULT; 
	}
}

void CSuperListCtrl::PaintEx(CDC *pDC){
	RECT rClientRect;
	GetClientRect(&rClientRect);
}


