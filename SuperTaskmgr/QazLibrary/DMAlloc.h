
/*
	DMAlloc.h
	高效率的动态内存分配 （可防止频繁申请、释放内存）
*/

#pragma once
#include <string.h>
#include <assert.h>

#ifndef TRACE
#define TRACE(f,b) (0)
#endif

//动态内存空间类 （循环读取不确定大小的数据到指定的内存空间时使用）
template <class BufferType,int m_NewReqMultiple=2> //m_NewReqMultiple 为申请倍数（当当前缓冲内存不足时扩大的倍数）
class CDMAlloc { 
	BufferType m_pBuffer; //内存缓冲区
	size_t m_NowSize; //当前缓冲区大小

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
			size<m_NowSize //当前缓冲区内存足够
				) || (
			m_pBuffer ? delete m_pBuffer : 0, //如果不够则释放旧内存
			TRACE(_T("CDMAlloc new char [%d]\n"),size*m_NewReqMultiple),
			m_pBuffer=(BufferType) new char [m_NowSize=size*m_NewReqMultiple] //重新申请新的更大的内存
				)
			)
			return true;
		m_NowSize=0;
		return false;
	}
};

#define SYSTEM_BIT (sizeof(size_t)*8)

//静态数据指针类 （仅用于储存静态数据，返回数据的指针）
template <class BufferType,int m_MinElemNumber,int m_NewReqMultiple=2>
class CNewAlloc{
	BufferType *paElemBuffer[SYSTEM_BIT]; //存放元素块的数组
	size_t aElemArrayNumber[SYSTEM_BIT]; //每个元素块的元素个数
	BufferType *paNewStart[SYSTEM_BIT]; //元素块数组中每个元素块的空闲区域指针
	int npBufferCount; //当前已分配到的最大下标

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
		for (k=0;k<npBufferCount;k++){ //寻找空闲内存
			if (
				(
					(aElemArrayNumber[k]*sizeof(BufferType)) //当前元素块数组的总内存
						-
					((size_t)paNewStart[k]-(size_t)paElemBuffer[k]) //当前元素块数组已用内存
				) > nElemNumber*sizeof(BufferType) //若当前元素块数组剩余内存大于所需内存
				)
				break;
		}
		if (k==npBufferCount){ //若未找到空位
			do {
				assert(npBufferCount<SYSTEM_BIT);
				aElemArrayNumber[npBufferCount]=aElemArrayNumber[npBufferCount-1]*m_NewReqMultiple;
				//申请新内存
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

		//已经有空闲区域内存 paNewStart[k]
		BufferType *pSpaceMem=paNewStart[k];
		BufferType *result=pSpaceMem;

		//直接复制元素块到空闲内存区域
		if (pElem)
			memcpy_s(
				pSpaceMem,
				(size_t)paElemBuffer[k]+aElemArrayNumber[k]*sizeof(BufferType)-(size_t)pSpaceMem,
				pElem,
				nElemNumber*sizeof(BufferType));

		//减小空闲区域内存
		paNewStart[k]+=nElemNumber;
		return (BufferType*)result;
	}
};



