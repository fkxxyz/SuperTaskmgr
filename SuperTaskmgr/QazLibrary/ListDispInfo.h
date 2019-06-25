
#pragma once
#include "DMAlloc.h"

class CListDispInfo {
public:
	//单元格属性
	typedef struct _DISP_CELL {
		HFONT hFont; //字体
		COLORREF crText; //字体颜色
		COLORREF crBack; //单元格背景色
		LPTSTR szText; //文字
	} DISP_CELL, *PDISP_CELL;

	//项目属性（一整行）
	typedef struct _DISP_ITEM {
		INT_PTR nImage; //图标号
		BOOL bChecked; //是否被勾选
		BOOL bSelected; //是否被选中
		DISP_CELL dcaCell[1]; //单元格的值
	} DISP_ITEM, *PDISP_ITEM;

	//列表属性（整个列表）
	typedef struct _DISP_LIST {
		int nColumnCount; //（标题个数）列数
		int nItemCount; //（项目个数）行数
		PDISP_ITEM pdiaItem[1]; //每一项的属性
	} DISP_LIST, *PDISP_LIST;

	//回调函数类型
	typedef BOOL (CALLBACK *LPFNGETCELLSTRING)(int,int,LPCTSTR *,LPTSTR);

	inline void free(){
		this->~CListDispInfo();
	}

	inline PDISP_LIST GetList(){return m_DispList.GetData();}

	//初始化列表（设置列数、行数）
	BOOL SetListSize(int nColumn,int nRow);
	BOOL SetAllCellsString(LPFNGETCELLSTRING);

private:
	//列表数据
	CDMAlloc<PDISP_LIST,8> m_DispList;

	//动态字符串储存区
	CNewAlloc<TCHAR,4096> m_StringData;
};


