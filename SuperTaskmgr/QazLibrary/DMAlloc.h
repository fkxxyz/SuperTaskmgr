
/*
	DMAlloc.h
	��Ч�ʵĶ�̬�ڴ���� ���ɷ�ֹƵ�����롢�ͷ��ڴ棩
*/

#pragma once
#include <string.h>
#include <assert.h>

#ifndef TRACE
#define TRACE(f,b) (0)
#endif

//��̬�ڴ�ռ��� ��ѭ����ȡ��ȷ����С�����ݵ�ָ�����ڴ�ռ�ʱʹ�ã�
template <class BufferType,int m_NewReqMultiple=2> //m_NewReqMultiple Ϊ���뱶��������ǰ�����ڴ治��ʱ����ı�����
class CDMAlloc { 
	BufferType m_pBuffer; //�ڴ滺����
	size_t m_NowSize; //��ǰ��������С

public:
	CDMAlloc::CDMAlloc(size_t init_size=0){
		m_NowSize=0;
		m_pBuffer=NULL;

		if (init_size)
			DemandSize(init_size);
	}
	CDMAlloc::~CDMAlloc(){if (m_pBuffer) delete m_pBuffer;}

	size_t GetSize(){return m_NowSize;}
	BufferType GetData(){return m_pBuffer;}
	operator BufferType(){return GetData();}

	bool DemandSize(size_t size){
		assert(size>0);
		if ((
			size<m_NowSize //��ǰ�������ڴ��㹻
				) || (
			m_pBuffer ? delete m_pBuffer : 0, //����������ͷž��ڴ�
			TRACE(_T("CDMAlloc new char [%d]\n"),size*m_NewReqMultiple),
			m_pBuffer=(BufferType) new char [m_NowSize=size*m_NewReqMultiple] //���������µĸ�����ڴ�
				)
			)
			return true;
		m_NowSize=0;
		return false;
	}
};

#define SYSTEM_BIT (sizeof(size_t)*8)

//��̬����ָ���� �������ڴ��澲̬���ݣ��������ݵ�ָ�룩
template <class BufferType,int m_MinElemNumber,int m_NewReqMultiple=2>
class CNewAlloc{
	BufferType *paElemBuffer[SYSTEM_BIT]; //���Ԫ�ؿ������
	size_t aElemArrayNumber[SYSTEM_BIT]; //ÿ��Ԫ�ؿ��Ԫ�ظ���
	BufferType *paNewStart[SYSTEM_BIT]; //Ԫ�ؿ�������ÿ��Ԫ�ؿ�Ŀ�������ָ��
	int npBufferCount; //��ǰ�ѷ��䵽������±�

public:
	CNewAlloc():npBufferCount(1){
		aElemArrayNumber[0]=m_MinElemNumber;
		paNewStart[0]=paElemBuffer[0]=new BufferType[m_MinElemNumber];
	}
	~CNewAlloc(){
		for (int i=0;i<npBufferCount;i++)
			delete []paElemBuffer[i];
	}

	void Reset(){
		for (int i=0;i<npBufferCount;i++)
			paNewStart[i]=paElemBuffer[i];
	}
	BufferType* NewData(const BufferType* pElem=NULL,size_t nElemNumber=1){
		int k;
		for (k=0;k<npBufferCount;k++){ //Ѱ�ҿ����ڴ�
			if (
				(
					(aElemArrayNumber[k]*sizeof(BufferType)) //��ǰԪ�ؿ���������ڴ�
						-
					((size_t)paNewStart[k]-(size_t)paElemBuffer[k]) //��ǰԪ�ؿ����������ڴ�
				) > nElemNumber*sizeof(BufferType) //����ǰԪ�ؿ�����ʣ���ڴ���������ڴ�
				)
				break;
		}
		if (k==npBufferCount){ //��δ�ҵ���λ
			do {
				assert(npBufferCount<SYSTEM_BIT);
				aElemArrayNumber[npBufferCount]=aElemArrayNumber[npBufferCount-1]*m_NewReqMultiple;
				//�������ڴ�
				if (!(
					paNewStart[npBufferCount]=paElemBuffer[npBufferCount]=new BufferType[aElemArrayNumber[npBufferCount]]
				)){
					assert(0);
					return NULL;
				}
				TRACE(_T("CNewAlloc new BufferType [%d]\n"),aElemArrayNumber[npBufferCount]);
			}while (aElemArrayNumber[npBufferCount++]<nElemNumber);

			k=npBufferCount-1;
		}

		//�Ѿ��п��������ڴ� paNewStart[k]
		BufferType *pSpaceMem=paNewStart[k];
		BufferType *result=pSpaceMem;

		//ֱ�Ӹ���Ԫ�ؿ鵽�����ڴ�����
		if (pElem)
			memcpy_s(
				pSpaceMem,
				(size_t)paElemBuffer[k]+aElemArrayNumber[k]*sizeof(BufferType)-(size_t)pSpaceMem,
				pElem,
				nElemNumber*sizeof(BufferType));

		//��С���������ڴ�
		paNewStart[k]+=nElemNumber;
		return (BufferType*)result;
	}
};



