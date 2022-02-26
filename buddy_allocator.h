#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

int initMem(unsigned int n);
unsigned char* allocBlock(unsigned long blockSize);
int freeBlock(unsigned char* blockAddr, unsigned long blockSize);
int freeMem();

#endif
