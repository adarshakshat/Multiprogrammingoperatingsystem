#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED
#include <string>
#include<iostream>
#include <string.h>


class Storage{
    private:
		char memory[100][4];
		char ch;
		//Instruction Register and Register
		std::string IR,R;
		//Instruction Count
		int IC;
		//Toggle Variable
		bool C;
    public:
        void init();
        std::string getRow(int pos);
        void setRow(std::string str, int pos);
        void load();
        void setIC();
        void setIC(int pos);
        int getIC();
        void setIR(int IC);
        std::string getIR();
        void setR(int pos);
        std::string getR();
        void setC(bool value);
        bool getC();
};


#endif