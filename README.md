# BuddyAllocator
Implementation of the Buddy Memory Allocation Algorithm in C++

# Use
int initMem(unsigned int n);
         Initialize a memory region of 2^n bytes.
         Return 0 on success and -1 on failure

unsigned char* allocBlock(unsigned long blockSize);
	 Allocates a block of blockSize bytes in the memory region.
	 Returns a pointer to the region on success.
	 Returns nullptr on failure.

int freeBlock(unsigned char* blockAddr, unsigned long blockSize);
    	Frees the block at the address blockAddr with size blockSize.
	Returns 0 on success.
	Assumes blockAddr is a valid address returned by allocBlock, and is blockSize bytes long.
	
int freeMem();
	Frees the memory region of 2^n bytes.
	Returns 0 on success.