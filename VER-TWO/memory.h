#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#include <string>
#include<iostream>
#include <string.h>
class memory
{
private:
    /* data */
    char mymemory[300][4];
	char ch;
	int tablePointer;
public:
    void init();
    std::string fetchRow(int row);
    void setRow(std::string word,int row);
    int pointerToPageTable();
    int pageAllocate();
    void pageTable(int row,int pgnum);
    void cardStore();
};




#endif
