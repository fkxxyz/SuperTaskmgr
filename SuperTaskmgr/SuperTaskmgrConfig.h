
#pragma once

#include "SuperTaskmgrProcess.h"

#define ExistFile(x) (GetFileAttributes(x)!=0xFFFFFFFF)
#define _tcseqr(x,y) (_tcscmp(x,y)==0)
#ifdef UNICODE
	#define _tcstoi(x) _wtoi(x)
#else
	#define _tcstoi(x) atoi(x)
#endif

typedef struct {
	ENUM_COLUMN cindex; //作为检验
	LPTSTR name; //列表头名称
	int xDefaultWidth; //默认宽度
	int LvcFormat; //显示的格式
	INT_PTR lpvDispOffset; //显示的字符串在PROCESS结构体中的偏移地址
	INT_PTR lpvDataOffset; //数据在PROCESS结构体中的偏移地址
	LPTSTR (*lpfunConv)(LPTSTR,LPVOID,LPPROCESS); //格式转换的函数地址
	int (*lpfunCmp)(LPVOID,LPVOID); //比较大小的函数
	BOOL bUpdateWhenRefresh; //是否是进程的动态属性（每次刷新都要更新此属性）
} COLUMN_ITEM_STRUCT;

typedef struct {
	ENUM_COLUMN cindex;
	INT width;
} COLUMN_SETTING;

#define MAX_LISTITEM_NUMBER 1024

typedef struct {
	int left;
	int top;
	int width;
	int height;
	int RefreshSpeed;
	BOOL ShowInFront;
	BOOL HideWhenMinimize;
	BOOL ShowFloatingWindow;
	BOOL ShowSystemProcess;

	double dwFloatWindowX, dwFloatWindowY;  //悬浮窗位置  百分比
	int iCloseToBorder;  // 0 不贴边   1~4  左、上、右、下
	int bAlpha;  // 透明度 0~255

	int ColumnNumber;
	COLUMN_SETTING column[cENUM_COLUMN_END];

	LPTSTR WhiteList[MAX_LISTITEM_NUMBER];int nWhiteList;
	LPTSTR BlackList[MAX_LISTITEM_NUMBER];int nBlackList;
	LPTSTR SystemProcess[MAX_LISTITEM_NUMBER];int nSystemProcess;
} SETTING;

#define MIN_SETTING_WIDTH 192
#define MIN_SETTING_HEIGHT 256

