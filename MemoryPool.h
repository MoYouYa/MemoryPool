#pragma once
#include <corecrt_malloc.h>
#include <stdio.h>
struct MemoryBlock {
	unsigned int dataSize;
	unsigned int freeNum;
	unsigned int visitedData;//�����ʹ�õ��ڴ�,ͬʱҲ������һ�� �������Դ洢��obj����Ϊsizeof(unsigned int)*8 (unsigned int ��λ��)
	MemoryBlock* next;
	char data[];
};

class MemoryPool {
	unsigned int objSize;
	unsigned int initObjCount;//��ʼ����Կ��ٶ�һ����ڴ�
	unsigned int growObjCount;//����ʼ����ڴ�ľ�֮�󣬺������ٵ����������Сһ�㣬����������ô��
	MemoryBlock* firstBlock;

public:
	MemoryPool(unsigned int _objSize=sizeof(char), unsigned int _initObjCount = 1, unsigned int _growObjCount = 1) {
		objSize = _objSize >= sizeof(char) ? _objSize : sizeof(char);
		initObjCount = _initObjCount >= 1 ? (_initObjCount <= sizeof(unsigned int)*8 ? _initObjCount : sizeof(unsigned int)*8) : 1;
		growObjCount = _growObjCount <= sizeof(unsigned int)*8 ? _growObjCount : sizeof(unsigned int)*8;
		firstBlock = NULL;
	}

	void* memoryPool_malloc();

	void memoryPool_free(void* p);

	void clear();

	~MemoryPool() {
		clear();
	}
};