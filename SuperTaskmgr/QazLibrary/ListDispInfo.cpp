
#include "stdafx.h"
#include "ListDispInfo.h"

#ifdef _DEBUG
void CheckReplaceMem(LPVOID address,SIZE_T size,int old_char,int new_char){
	SIZE_T i;char *p=(char *)address;
	for (i=0;i<size;i++,p++){
		ASSERT(*p==(char)old_char);
		*p=(char)new_char;
	}
}
void CheckReplace00ToFF(LPVOID address,SIZE_T size){
	CheckReplaceMem(address,size,0x00,0xFF);
}
void CheckReplaceFFTo00(LPVOID address,SIZE_T size){
	CheckReplaceMem(address,size,0xFF,0x00);
}
template <typename T>
void FillCheckFF(T address){
	CheckReplace00ToFF(address,sizeof(*address));
}
#endif


BOOL CListDispInfo::SetListSize(int nColumn,int nRow){
	//计算所需申请的内存大小
	int nLineSize;
	int nNeedSize=
		sizeof(DISP_LIST)+(nRow-1)*sizeof(PDISP_ITEM)+nRow*( //整个列表所需总内存
			nLineSize=sizeof(DISP_ITEM)+(nColumn-1)*( //每一行所需内存
				sizeof(DISP_CELL) //每一单元所需内存
				)
			);

	m_DispList.DemandSize(nNeedSize);

	//用于检验各地址数据是否正确
#ifdef _DEBUG
	memset(m_DispList.GetData(),0,nNeedSize);
#endif

	DISP_LIST &stDispList=*m_DispList.GetData();

	//开始设置列表内基本数据
	stDispList.nColumnCount=nColumn;
	stDispList.nItemCount=nRow;
	int i;
	for (i=0;i<nRow;i++){
		stDispList.pdiaItem[i]=(PDISP_ITEM)((INT_PTR)(stDispList.pdiaItem+nRow)+i*nLineSize);
	}

	//开始检验各地址数据是否正确
#ifdef _DEBUG
	for (i=0;i<nRow;i++){
		FillCheckFF(&stDispList.pdiaItem[i]->nImage);
		FillCheckFF(&stDispList.pdiaItem[i]->bChecked);
		FillCheckFF(&stDispList.pdiaItem[i]->bSelected);
		for (int j=0;j<nColumn;j++){
			FillCheckFF(&stDispList.pdiaItem[i]->dcaCell[j].hFont);
			FillCheckFF(&stDispList.pdiaItem[i]->dcaCell[j].crText);
			FillCheckFF(&stDispList.pdiaItem[i]->dcaCell[j].crBack);
			FillCheckFF(&stDispList.pdiaItem[i]->dcaCell[j].szText);
		}
	}
	stDispList.nColumnCount=0;
	stDispList.nItemCount=0;
	for (i=0;i<nRow;i++){
		stDispList.pdiaItem[i]=0;
	}
	FillCheckFF(&stDispList.nColumnCount);
	FillCheckFF(&stDispList.nItemCount);
	for (i=0;i<nRow;i++){
		FillCheckFF(&stDispList.pdiaItem[i]);
	}

	CheckReplaceMem(m_DispList.GetData(),nNeedSize,0xFF,0x00);

	//检验完毕，还原数据
	stDispList.nColumnCount=nColumn;
	stDispList.nItemCount=nRow;
	for (i=0;i<nRow;i++){
		stDispList.pdiaItem[i]=(PDISP_ITEM)((INT_PTR)(stDispList.pdiaItem+nRow)+i*nLineSize);
	}
#endif
	return TRUE;
}

BOOL CListDispInfo::SetAllCellsString(LPFNGETCELLSTRING GetStringCallBack){
	DISP_LIST &stDispList=*m_DispList.GetData();
	LPCTSTR pNewString;
	TCHAR szBuffer[32767];
	int i,j;

	//重置空闲内存（删除所有现有的动态字符串）
	m_StringData.Reset();

	//开始获取每一单元格信息
	for (i=0;i<stDispList.nItemCount;i++){
		for (j=0;j<stDispList.nColumnCount;j++){
			LPTSTR &szDest=stDispList.pdiaItem[i]->dcaCell[j].szText;
			if (GetStringCallBack(i,j,&pNewString,szBuffer)) //返回值表示是否需要新内存
				szDest=m_StringData.NewData(pNewString,_tcslen(pNewString)+1);
			else
				szDest=(LPTSTR)pNewString;
		}
	}
	return TRUE;
}

