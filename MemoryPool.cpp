#include "MemoryPool.h"

void* MemoryPool::memoryPool_malloc() {
	if (firstBlock == NULL) {
		firstBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock) + objSize * initObjCount);
		if (firstBlock == NULL) {
			perror("malloc is failed!");
			return NULL;
		}
		firstBlock->dataSize = objSize * initObjCount;
		firstBlock->freeNum = initObjCount - 1;
		firstBlock->visitedData = 1;
		firstBlock->next = NULL;
		return (void*)firstBlock->data;
	}
	else {
		MemoryBlock* pmb = firstBlock;
		while (pmb->freeNum <= 0 && pmb->next != NULL) {
			pmb = pmb->next;
		}
		if (pmb->freeNum <= 0 && pmb->next == NULL) {
			if (growObjCount == 0) {
				return NULL;
			}
			MemoryBlock* newBlock = (MemoryBlock*)malloc(sizeof(MemoryBlock) + objSize * growObjCount);
			if (newBlock == NULL) {
				perror("malloc is failed!");
				return NULL;
			}
			newBlock->dataSize = objSize * growObjCount;
			newBlock->freeNum = growObjCount - 1;
			newBlock->visitedData = 1;
			newBlock->next = NULL;
			pmb->next = newBlock;
			return (void*)newBlock->data;
		}
		else {
			unsigned int index = 0;
			while ((1 << index) & pmb->visitedData) {
				index++;
			}
			void* p = pmb->data + index * objSize;
			pmb->freeNum--;
			pmb->visitedData |= (1 << index);
			return p;
		}
	}
}

void MemoryPool::memoryPool_free(void* p) {
	MemoryBlock* pmb = firstBlock;
	if (pmb && (unsigned int)p < (unsigned int)(void*)pmb->data && (unsigned int)p >= (unsigned int)(void*)pmb->data + pmb->dataSize) {
		pmb = pmb->next;
	}
	if (pmb == NULL) {
		perror("free is failed! this pointer is not at this memory pool.");
		return;
	}
	unsigned int bytes = (unsigned int)p - (unsigned int)(void*)pmb->data;
	if (bytes % objSize != 0) {
		perror("free is failed! this pointer has a bad address between this memory pool.");
		return;
	}
	unsigned int index = bytes / objSize;
	if ((pmb->visitedData >> index) % 2 == 0) {
		perror("free is failed! this pointer address which is in this memory pool is not used.");
		return;
	}
	else {
		pmb->visitedData ^= (1 << index);
		pmb->freeNum++;
	}
	return;
}

void MemoryPool::clear() {
	MemoryBlock* pmb = firstBlock;
	if (pmb != NULL) {
		while (pmb->next != NULL) {
			MemoryBlock* p = pmb;
			pmb = pmb->next;
			free(p);
		}
		free(pmb);
	}
}