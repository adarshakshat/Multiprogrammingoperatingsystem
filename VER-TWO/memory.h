#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED
#include <string>
#include<iostream>
#include <string.h>
class memory
{
private:
    /* data */
    char mem[300][4];
	char ch;
	int page_table_ptr;
public:
    //void reset()s
    void init();
    //string get_mem(int pos)
    std::string fetchRow(int row);
    //void setRow(string s, int pos)
    void setRow(std::string word,int row);
    //int get_page_table_ptr()
    int pointerToPageTable();
    //int allocate_page()
    int pageAllocate();
    //set_Page_Table
    void pageTable(int row,int pgnum);
    //store_card()
    void cardStore();
    //void print_mem()
    void memoryPrint();
};




#endif
