#pragma once
#include <corecrt_malloc.h>
#include <stdio.h>
struct MemoryBlock {
	unsigned int dataSize;
	unsigned int freeNum;
	unsigned int visitedData;//标记已使用的内存,同时也限制了一个 块最多可以存储的obj数量为sizeof(unsigned int)*8 (unsigned int 的位数)
	MemoryBlock* next;
	char data[];
};

class MemoryPool {
	unsigned int objSize;
	unsigned int initObjCount;//初始块可以开辟多一点的内存
	unsigned int growObjCount;//当初始块的内存耗尽之后，后续开辟的其他块可以小一点，增长不用那么快
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