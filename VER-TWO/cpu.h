#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <string>
#include <iostream>
#include <string.h>

class VirtualMachine
{
private:
  enum commandslist
  {
    GD,
    PD,
    LR,
    SR,
    CR,
    BT,
    TERMINATE
  } commands;
  int fetched_IC, terminate_code, TLL, TTL, LLC, TLC, pos;
  bool terminate, fetched_C;
  std::string fetched_IR, operand, opreator, fetched_R, compare_string, p_id;
  char IR[4], R[4], IC[2];
  bool C, run_mos;

public:
  //Getters
  int ICgetter();
  std::string IRgetter();
  std::string Rgetter();
  bool Cgetter();
  //Setters
  void ICsetter();
  void ICsetter(int row);
  void IRsetter(int IC);
  void Rsetter(int row);
  void Csetter(bool value);
  //Member functions
  int operandToLine(std::string operand);
  void limitsetter(std::string limits);
  void incIC();
  int translateAddress(int address);
  int translateAddress(std::string str);
  void GDCOMMAND(int row);
  void PDCOMMAND(int row);
  void SRCOMMAND(int row);
  void LRCOMMAND(int row);
  void CRCOMMAND(int row);
  void BTCOMMAND(int row);
  void FETCH();
  void DECODE();
  void MOS();
};

#endif