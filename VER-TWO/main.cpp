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
        mymemory.setRow(s1, i);
    }
}
void VirtualMachine::PDCOMMAND(int row)
{
    row = (row / 10) * 10;
    int flag = 0;
    string ans = "", temp = "";
    for (int i = row; i < row + 10; i++)
    {
        temp = mymemory.fetchRow(i);
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
    mymemory.setRow(row);
}

void VirtualMachine::SRCOMMAND(int row)
{
    fetched_R = mymemory.getR();
    mymemory.setRow(fetched_R, row);
}

void VirtualMachine::CRCOMMAND(int row)
{
    fetched_R = mymemory.getR();
    compare_string = mymemory.fetchRow(row);
    if (fetched_R.compare(compare_string) == 0)
        mymemory.setC(true);
    else
        mymemory.setC(false);
}

void VirtualMachine::BTCOMMAND(int row)
{
    fetched_C = mymemory.getC();
    if (fetched_C)
        mymemory.setIC(row);
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
        temp += mymemory[pos][i];
    return temp;
}
void memory::setRow(string s, int pos)
{
    for (int i = 0; i < 4; i++)
        mymemory[pos][i] = s[i];
}
int memory::pointerToPageTable()
{
    return tablePointer;
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
            //Load program Cards here
            continue;
        }
        else if (s.find("$DTA") != -1)
            //Run Commands
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
