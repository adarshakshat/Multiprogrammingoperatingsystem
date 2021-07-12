#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <string>
#include<iostream>
#include <string.h>

class cpu{
    private:
		int fetched_IC,terminate_code,TLL,TTL,LLC,TLC,pos;
		bool terminate,fetched_C;
		std::string fetched_IR,operand,opreator,fetched_R,compare_string,p_id;
		char IR[4],R[4],IC[2];
		bool C,run_mos;
    public:
    int operandToLine(std::string operand);
    void setLimits(std::string limits);
    void setIC();
    void setIC(int row);
    int getIC();
    void incIC();
    void setIR(int IC);
    std::string getIR();
    void setR(int row);
    std::string getR();
    void setC(bool value);
    bool getC();
    int translateAddress(int address);
    int translateAddress(std::string str);
    void startexe();
    void mos();
};


#endif