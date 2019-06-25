

#ifdef __CSuperTaskmgrDlg
inline void CSuperTaskmgrDlg::ListUpdateMenu(BOOL &bSelAllVar,INT_PTR iProStOffset){
	bSelAllVar=TRUE;
	if (m_LstProc.GetSelectedCount()){
		int i;
		BEGIN_ENUM_SELECTED_ITEM(&m_LstProc,i)
			if (!(*(BOOL *)((INT_PTR)pListPro[i]+iProStOffset))){
				bSelAllVar=FALSE;
				break;
			}
		END_ENUM_SELECTED_ITEM
	}
}
#endif

#ifndef __SuperTaskmgr_Command
#define __SuperTaskmgr_Command

#define BEGIN_ENUM_SELECTED_ITEM(pList,iItem) \
	{ \
		int ItemCount=(pList)->GetItemCount(); \
		for (iItem=0;iItem<ItemCount;iItem++) \
			if ((pList)->GetItemState((int)iItem,LVIS_SELECTED) & LVIS_SELECTED) { \

#define END_ENUM_SELECTED_ITEM \
			} \
	}

#endif



