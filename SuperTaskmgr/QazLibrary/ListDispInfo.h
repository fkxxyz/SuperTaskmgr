
#pragma once
#include "DMAlloc.h"

class CListDispInfo {
public:
	//��Ԫ������
	typedef struct _DISP_CELL {
		HFONT hFont; //����
		COLORREF crText; //������ɫ
		COLORREF crBack; //��Ԫ�񱳾�ɫ
		LPTSTR szText; //����
	} DISP_CELL, *PDISP_CELL;

	//��Ŀ���ԣ�һ���У�
	typedef struct _DISP_ITEM {
		INT_PTR nImage; //ͼ���
		BOOL bChecked; //�Ƿ񱻹�ѡ
		BOOL bSelected; //�Ƿ�ѡ��
		DISP_CELL dcaCell[1]; //��Ԫ���ֵ
	} DISP_ITEM, *PDISP_ITEM;

	//�б����ԣ������б�
	typedef struct _DISP_LIST {
		int nColumnCount; //���������������
		int nItemCount; //����Ŀ����������
		PDISP_ITEM pdiaItem[1]; //ÿһ�������
	} DISP_LIST, *PDISP_LIST;

	//�ص���������
	typedef BOOL (CALLBACK *LPFNGETCELLSTRING)(int,int,LPCTSTR *,LPTSTR);

	inline void free(){
		this->~CListDispInfo();
	}

	inline PDISP_LIST GetList(){return m_DispList.GetData();}

	//��ʼ���б�����������������
	BOOL SetListSize(int nColumn,int nRow);
	BOOL SetAllCellsString(LPFNGETCELLSTRING);

private:
	//�б�����
	CDMAlloc<PDISP_LIST,8> m_DispList;

	//��̬�ַ���������
	CNewAlloc<TCHAR,4096> m_StringData;
};


