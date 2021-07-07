#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include "cpu.h"
#include "storage.h"
using namespace std;
//input File Stream
ifstream inputfile;
//Output File Stream
ofstream outputfile;
int SI;

Storage memory;
VirtualMachine virtualmachine;

void VirtualMachine::GDCOMMAND(int row)
{
    string s;
    row = (row / 10) * 10;
    getline(inputfile, s);
    if (!s.empty() && s[s.size() - 1] == '\r')
        s.erase(s.size() - 1);
    int len = s.length(), start = 0, i;
    string s1;
    for (i = row; start < len; i++)
    {
        if ((len - start) < 4)
            s1 = s.substr(start, (len - start));
        else
            s1 = s.substr(start, 4);
        start += 4;
        memory.setRow(s1, i);
    }
}
void VirtualMachine::PDCOMMAND(int row)
{
    row = (row / 10) * 10;
    int flag = 0;
    string ans = "", temp = "";
    for (int i = row; i < row + 10; i++)
    {
        temp = memory.getRow(i);
        for (int j = 0; j < 4; j++)
        {

            if (temp[j] == '\0' || temp[j] == '0')
            {
                flag = 1;
                break;
            }
            ans += temp[j];
        }
        if (flag)
            break;
    }
    outputfile << ans << endl;
}

void VirtualMachine::LRCOMMAND(int row)
{
    memory.setR(row);
}

void VirtualMachine::SRCOMMAND(int row)
{
    fetched_R = memory.getR();
    memory.setRow(fetched_R, row);
}

void VirtualMachine::CRCOMMAND(int row)
{
    fetched_R = memory.getR();
    compare_string = memory.getRow(row);
    if (fetched_R.compare(compare_string) == 0)
        memory.setC(true);
    else
        memory.setC(false);
}

void VirtualMachine::BTCOMMAND(int row)
{
    fetched_C = memory.getC();
    if (fetched_C)
        memory.setIC(row);
}

int VirtualMachine::operandtoline(std::string operand)
{
    return ((int)operand[0] - 48) * 10 + ((int)operand[1] - 48);
}

void VirtualMachine::FETCH()
{
    memory.setIC();
    DECODE();
}

void VirtualMachine::DECODE()
{
    enum commandslist command;
    terminate = false;
    while (!terminate)
    {
        fetched_IC = memory.getIC();
        memory.setIR(fetched_IC);
        fetched_IR = memory.getIR();
        opreator = fetched_IR.substr(0, 2);
        operand = fetched_IR.substr(2, 2);
        if (!(opreator.compare("LR")))
        {
            mos(LR, operandtoline(operand));
        }
        else if (!(opreator.compare("SR")))
        {
            mos(SR, operandtoline(operand));
        }
        else if (!(opreator.compare("CR")))
        {

            mos(CR, operandtoline(operand));
        }
        else if (!(opreator.compare("BT")))
        {

            mos(BT, operandtoline(operand));
        }
        else if (!(opreator.compare("GD")))
        {
            mos(GD, operandtoline(operand));
        }
        else if (!(opreator.compare("PD")))
        {
            mos(PD, operandtoline(operand));
        }
        else
        {
            mos(TERMINATE, 0);
        }
    }
}

void VirtualMachine::mos(enum commandslist command, int operand)
{
    switch (command)
    {
    case GD:
        GDCOMMAND(operand);
        /* code */
        break;
    case LR:
        LRCOMMAND(operand);
        break;
    case PD:
        PDCOMMAND(operand);
        break;
    case SR:
        SRCOMMAND(operand);
        break;
    case CR:
        CRCOMMAND(operand);
        break;
    case BT:
        BTCOMMAND(operand);
        break;
    case TERMINATE:
        terminate = true;
        outputfile << endl
                   << endl;
    default:
        break;
    }
}

void Storage::init()
{
    memset(memory, '0', sizeof(char) * 100 * 4);
    IR = R = "";
    IC = 0;
    C = false;
}
//Get word from a row
string Storage::getRow(int pos)
{
    string temp = "";
    for (int i = 0; i < 4; i++)
        temp += memory[pos][i];
    return temp;
}
//Set values of a row
void Storage::setRow(string s, int pos)
{
    for (int i = 0; i < 4; i++)
        memory[pos][i] = s[i];
}
void Storage::load()
{
    int flag = 0;
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j <= 3; j++)
        {
            inputfile >> ch;
            memory[i][j] = ch;
            if (memory[i][0] == 'H')
            {
                flag = 1;
                break;
            }
        }
        if (flag)
            break;
    }
}
void Storage::setIC()
{
    IC = 0;
}
void Storage::setIC(int pos)
{
    Storage::IC = pos;
}
int Storage::getIC()
{
    return (IC++);
}
void Storage::setIR(int IC)
{
    IR = "";
    for (int i = 0; i < 4; i++)
    {
        IR += memory[IC][i];
    }
}
string Storage::getIR()
{
    return IR;
}
void Storage::setR(int pos)
{
    R = "";
    for (int i = 0; i < 4; i++)
        R += memory[pos][i];
}
string Storage::getR()
{
    return R;
}
void Storage::setC(bool value)
{
    C = value;
}
bool Storage::getC()
{
    return C;
}

void run(char *filename)
{
    inputfile.open(filename);
    outputfile.open("output.txt");
    string s, s1;
    while (!(inputfile.eof()))
    {
        getline(inputfile, s);
        if (s.find("$AMJ") != -1)
        {
            memory.init();
            memory.load();
            continue;
        }
        else if (s.find("$DTA") != -1)
            virtualmachine.FETCH();
        else if (s.find("$END") != -1)
            continue;
    }
    inputfile.close();
    outputfile.close();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << argv[0] << " Requires an Input file Name in argument\n"
             << endl;
        return 0;
    }
    run(argv[1]);
    return 0;
}