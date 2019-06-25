
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
	ENUM_COLUMN cindex; //��Ϊ����
	LPTSTR name; //�б�ͷ����
	int xDefaultWidth; //Ĭ�Ͽ��
	int LvcFormat; //��ʾ�ĸ�ʽ
	INT_PTR lpvDispOffset; //��ʾ���ַ�����PROCESS�ṹ���е�ƫ�Ƶ�ַ
	INT_PTR lpvDataOffset; //������PROCESS�ṹ���е�ƫ�Ƶ�ַ
	LPTSTR (*lpfunConv)(LPTSTR,LPVOID,LPPROCESS); //��ʽת���ĺ�����ַ
	int (*lpfunCmp)(LPVOID,LPVOID); //�Ƚϴ�С�ĺ���
	BOOL bUpdateWhenRefresh; //�Ƿ��ǽ��̵Ķ�̬���ԣ�ÿ��ˢ�¶�Ҫ���´����ԣ�
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

	double dwFloatWindowX, dwFloatWindowY;  //������λ��  �ٷֱ�
	int iCloseToBorder;  // 0 ������   1~4  ���ϡ��ҡ���
	int bAlpha;  // ͸���� 0~255

	int ColumnNumber;
	COLUMN_SETTING column[cENUM_COLUMN_END];

	LPTSTR WhiteList[MAX_LISTITEM_NUMBER];int nWhiteList;
	LPTSTR BlackList[MAX_LISTITEM_NUMBER];int nBlackList;
	LPTSTR SystemProcess[MAX_LISTITEM_NUMBER];int nSystemProcess;
} SETTING;

#define MIN_SETTING_WIDTH 192
#define MIN_SETTING_HEIGHT 256

