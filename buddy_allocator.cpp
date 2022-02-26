
#include "buddy_allocator.h"

#include <iostream>
#include <cstdlib>
#include <list>

using namespace std;

static unsigned char *mem = nullptr;
static list<unsigned char*> *free_lists = nullptr;
static unsigned long sizeMem;
static unsigned int n;

int initMem(unsigned int nsize){
  n = nsize;
  sizeMem = 1;
  // Allocate mem (resembles heap)
  for(int i = 0; i < n; i++) sizeMem *= 2;
  mem = new unsigned char[sizeMem];
  if(mem == nullptr) return -1;
  // Allocate n+1 free_lists
  free_lists = new list<unsigned char*>[n+1];
  if(free_lists == nullptr) return -1;
      // free_lists[x] is 2^x sized blocks, x:[0, n]
  free_lists[n].push_back(mem);
  return 0;
}

unsigned char* allocBlock(unsigned long blockSize){
  unsigned int chunkSize = 1;
  unsigned int listIndex = 0;
  // Determine size needed
  while(chunkSize < blockSize){
    listIndex++;
    chunkSize *= 2;
  }
  
  // If won't fit in memory
  if(listIndex > n) return nullptr;
  
  // Find block address
  if(free_lists == nullptr) return nullptr;
  while(free_lists[listIndex].empty()){
    // If no small piece, break bigger
    unsigned int i;
    for(i = listIndex + 1; i <= n; i++){
      if(!free_lists[i].empty()){
	unsigned char* toSplit = free_lists[i].front();
	free_lists[i].pop_front();
	free_lists[i-1].push_back(toSplit);
	unsigned long splitSize = 1;
	for(unsigned int j = 0; j < i-1; j++) splitSize *= 2;
	free_lists[i-1].push_back(toSplit + splitSize);
	break;
      }
    }
    // No block big enough found, can't allocate
    if(i == n+1) return nullptr;
  }
  // Return found block
  unsigned char* blockAddr = free_lists[listIndex].front();
  free_lists[listIndex].pop_front();
  return blockAddr;
}

int freeBlock(unsigned char* blockAddr, unsigned long blockSize){
  unsigned int chunkSize = 1;
  unsigned int listIndex = 0;
  // Determine size needed
  while(chunkSize < blockSize){
    listIndex++;
    chunkSize *= 2;
  }

  // Insert addr back into free_lists (sorted)
  list<unsigned char*>::iterator it = free_lists[listIndex].begin();
  for(; it != free_lists[listIndex].end() && *it < blockAddr; it++);
  if(it == free_lists[listIndex].end()){
    free_lists[listIndex].push_back(blockAddr);
  }
  else {
     free_lists[listIndex].insert(it, blockAddr);
  }

  // Coalesce blocks all the way to free_lists[n]
  for(unsigned int i = listIndex; i < n; i++){
    unsigned long splitSize = 1;
    for(unsigned int j = 0; j < i-1; j++) splitSize *= 2;
    it = free_lists[i].begin();
    for(; it != free_lists[i].end() && next(it) != free_lists[i].end(); i++){
      // If two consecutive blocks
      if(*it + splitSize  == *next(it)){
	unsigned char* coalesced = *it;
	free_lists[i].erase(it);
	free_lists[i].erase(next(it));
	// Insert coalesced into free_lists[i+1]
	list<unsigned char*>::iterator adder = free_lists[i+1].begin();
	for(; adder != free_lists[i+1].end() && *adder < coalesced; adder++);
	if(adder == free_lists[i+1].end()){
	  free_lists[i+1].push_back(coalesced);
	}
      	else {
	  free_lists[i+1].insert(adder, coalesced);
	}
	break;
      }
    }
    // If reached here, no two consecutive blocks, so stop coalescing
    if(it == free_lists[i].end()){
      break;
    }
    // else, continue coalescing in higher free_lists
  }
  
  return 0;
}

int freeMem(){
  delete mem;
  delete[] free_lists;
  return 0;
}
