#include <iostream>
#include "buddy_allocator.h"

using namespace std;

int main(int argc, char* argv[]){
  initMem(3);
  unsigned char* addr = allocBlock(9);
  cout << (long long) addr << endl;
  addr = allocBlock(8);
  cout << (long long) addr << endl;
  unsigned char* addr2 = allocBlock(1);
  cout << (long long) addr2 << endl;
  freeBlock(addr, 8);
  addr = allocBlock(8);
  cout << (long long) addr << endl;
  freeBlock(addr, 8);
  addr2 = allocBlock(4);
  unsigned char *addr3 = allocBlock(4);
  cout << (long long) addr2 << endl;
  cout << (long long) addr3 << endl;
  return 0;
}
