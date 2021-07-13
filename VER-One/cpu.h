#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

class VirtualMachine{

	private:
		int fetched_IC;
		bool terminate,fetched_C;
		std::string fetched_IR,operand,opreator,fetched_R,compare_string;
		enum commandslist{GD,PD,LR,SR,CR,BT,TERMINATE}commands;
	public:
    void GDCOMMAND(int row);
    void PDCOMMAND(int row);
    void LRCOMMAND(int row);
    void SRCOMMAND(int row);
    void CRCOMMAND(int row);
    void BTCOMMAND(int row);
    int operandtoline(std::string operand);
    void FETCH();
    void DECODE();
    void mos(enum commandslist command,int operand);
};

#endif