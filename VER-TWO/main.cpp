#include "memory.h"
#include "cpu.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>

using namespace std;

ifstream inputfile;
ofstream outputfile;
int SI;
memory mymemory;

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

int VirtualMachine::operandToLine(std::string operand)
{
    return ((int)operand[0] - 48) * 10 + ((int)operand[1] - 48);
}

void VirtualMachine::FETCH()
{
    ICsetter(0);
}
void VirtualMachine::DECODE()
{
}
void VirtualMachine::MOS()
{
}

string memory::fetchRow(int pos)
{
    string temp = "";
    for (int i = 0; i < 4; i++)
        temp += mem[pos][i];
    return temp;
}
void memory::setRow(string s, int pos)
{
    for (int i = 0; i < 4; i++)
        mem[pos][i] = s[i];
}
int memory::pointerToPageTable()
{
    return page_table_ptr;
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
