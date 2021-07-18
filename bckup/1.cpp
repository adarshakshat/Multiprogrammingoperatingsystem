#include "memory.h"
#include "VirtualMachine.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

ifstream inputfile;
ofstream outputfile;
int SI, PI, TI;
bool occupied_pages[30];

memory mymemory;
VirtualMachine virtualmachine;

//VirtualMachine Member functions

int VirtualMachine::operandToLine(std::string operand)
{
    return ((int)operand[0] - 48) * 10 + ((int)operand[1] - 48);
}
void VirtualMachine::limitsetter(std::string limits)
{
    p_id = limits.substr(0, 4);
    TTL = operandToLine(limits.substr(4, 2)) * 100 + operandToLine(limits.substr(6, 2));
    TLL = operandToLine(limits.substr(8, 2)) * 100 + operandToLine(limits.substr(10, 2));
    LLC = 0;
    TLC = 0;
    terminate_code = 0;
}

void VirtualMachine::incIC()
{
    int val;
    val = ICgetter();
    val++;
    ICsetter(val);
}

int VirtualMachine::translateAddress(int address)
{
    int page = mymemory.pointerToPageTable() + (address / 10);
    string value_page = mymemory.fetchRow(page);
    if (value_page[1] == '$')
    {
        PI = 3;
        return -1;
    }
    value_page = value_page.substr(2, 2);
    return (operandToLine(value_page) * 10 + (address % 10));
}
int VirtualMachine::translateAddress(std::string str)
{
    if (operandToLine(str) == -1)
    {
        PI = 2;
        return -2;
    }
    else
        translateAddress(operandToLine(str));
}

//VirtualMachine Setter functions
void VirtualMachine::ICsetter()
{
    IC[0] = '0';
    IC[1] = '0';
}
void VirtualMachine::ICsetter(int row)
{
    IC[1] = ((char)pos % 10) + 48;
    pos = pos / 10;
    IC[0] = ((char)pos % 10) + 48;
}
void VirtualMachine::IRsetter(int IC)
{
    string returned_value = "";
    returned_value = mymemory.fetchRow(IC);
    for (int i = 0; i < 4; i++)
        IR[i] = returned_value[i];
}
void VirtualMachine::Rsetter(int row)
{
    string returned_value = "";
    returned_value = mymemory.fetchRow(pos);
    for (int i = 0; i < 4; i++)
        R[i] = returned_value[i];
}
void VirtualMachine::Csetter(bool value)
{
    C = value;
}

//VirtualMachine Getter functions
int VirtualMachine::ICgetter()
{
    int val;
    val = ((int)IC[0] - 48) * 10 + ((int)IC[1] - 48);
    return val;
}

std::string VirtualMachine::IRgetter()
{
    string ret_IR = "";
    for (int i = 0; i < 4; i++)
        ret_IR += IR[i];
    return ret_IR;
}

std::string VirtualMachine::Rgetter()
{
    string ret_R = "";
    for (int i = 0; i < 4; i++)
    {
        ret_R += R[i];
    }
    return ret_R;
}
bool VirtualMachine::Cgetter()
{
    return C;
}

void VirtualMachine::GDCOMMAND(int row)
{

    string s;
    pos = (pos / 10) * 10;
    getline(inputfile, s);
    if (s.find("$END") != -1)
    {
        terminate = true;
        terminate_code = 1;
    }
    else
    {
        if (!s.empty() && s[s.size() - 1] == '\r')
            s.erase(s.size() - 1);
        int len = s.length(), start = 0, i;
        string s1;
        for (i = pos; start < len; i++)
        {
            if ((len - start) < 4)
                s1 = s.substr(start, (len - start));
            else
                s1 = s.substr(start, 4);
            start += 4;
            mymemory.setRow(s1, i);
        }
    }
    SI = 0;
}
void VirtualMachine::PDCOMMAND(int pos)
{
    int flag=0;
    pos = (pos / 10) * 10;
    string ans = "", temp = "";
    for (int i = pos; i < pos + 10; i++)
    {
        temp = mymemory.fetchRow(i);
        for (int j = 0; j < 4; j++)
        {
            if (temp[j] == '\0' || temp[j] == '$')
            {
                break;
                flag = 1;
            }
            ans += temp[j];
        }
        if (flag)
            break;
    }
    outputfile << ans << endl;
    /*
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

    */
}

void VirtualMachine::LRCOMMAND(int row)
{
    if (pos == -1)
    {
        run_mos = true;
    }
    else
    {
        Rsetter(pos);
        TLC++;
    }
    //mymemory.setRow(row);
}

void VirtualMachine::SRCOMMAND(int row)
{
    /*
    fetched_R = mymemory.getR();
    mymemory.setRow(fetched_R, row);
*/
    fetched_R = Rgetter();
    if (pos == -1)
    {
        run_mos = true;
    }
    else
    {
        mymemory.setRow(fetched_R, pos);
        TLC++;
    }
}

void VirtualMachine::CRCOMMAND(int row)
{

    fetched_R = Rgetter();
    if (pos == -1)
    {
        run_mos = true;
    }
    else
    {
        TLC++;
        compare_string = mymemory.fetchRow(pos);
        if (fetched_R.compare(compare_string) == 0)
            Csetter(true);
        else
            Csetter(false);
    }

    /*
    fetched_R = mymemory.getR();
    compare_string = mymemory.fetchRow(row);
    if (fetched_R.compare(compare_string) == 0)
        mymemory.setC(true);
    else
        mymemory.setC(false);

        */
}

void VirtualMachine::BTCOMMAND(int row)
{

    fetched_C = Cgetter();
    if (fetched_C)
    {
        //int get_physical_add=address_tranlation();
        ICsetter(operandToLine(operand));
    }
    TLC++;

    /*
    fetched_C = mymemory.getC();
    if (fetched_C)
        mymemory.setIC(row);

*/
}

void VirtualMachine::FETCH()
{
    ICsetter(0);
}
void VirtualMachine::DECODE()
{
    terminate = false;
    while (!terminate)
    {
        //fetch ic
        //fetch ir form the location specified by ic
        //get the value of operator and operand
        run_mos = false;
        fetched_IC = translateAddress(ICgetter());
        incIC();
        IRsetter(fetched_IC);
        fetched_IR = IRgetter();
        if ((fetched_IR.compare("H")) == 3)
            fetched_IR = "Hrrr";
        opreator = fetched_IR.substr(0, 2);
        operand = fetched_IR.substr(2, 2);
        pos = translateAddress(operandToLine(operand));
        if (translateAddress(operand) != -2 || !(operand.compare("rr")))
        {
            if (!(opreator.compare("LR"))) //ok
            {
                if (pos == -1)
    {
        run_mos = true;
    }
    else
    {
        Rsetter(pos);
        TLC++;
    }
                //set the contents of the register R from the given location of memory specified in the operand
                //cout<<"LR";

                //CALL MOS
            }
            else if (!(opreator.compare("SR"))) //ok
            {
                fetched_R = Rgetter();
    if (pos == -1)
    {
        run_mos = true;
    }
    else
    {
        mymemory.setRow(fetched_R, pos);
        TLC++;
    }
                //get the contents of register R
                //store them at the location of memory specified in the operand
                //cout<<"SR";
                //TLC++;
                //CALL MOS
            }
            else if (!(opreator.compare("CR"))) //ok
            {
                fetched_R = Rgetter();
    if (pos == -1)
    {
        run_mos = true;
    }
    else
    {
        TLC++;
        compare_string = mymemory.fetchRow(pos);
        if (fetched_R.compare(compare_string) == 0)
            Csetter(true);
        else
            Csetter(false);
    }
                //get the contents of register r
                //compare with given memory location
                //if the values of above two matches then set toggle register to true
                //else set the toggle register to false
                //cout<<"CR";

                //CALL MOS
            }
            else if (!(opreator.compare("BT"))) //ok
            {


    fetched_C = Cgetter();
    if (fetched_C)
    {
        //int get_physical_add=address_tranlation();
        ICsetter(operandToLine(operand));
    }
    TLC++;


                //if the value of toggle register is true
                //we change the value of ic
                //cout<<"BT";

                //CALL MOS
            }
            else if (!(opreator.compare("GD"))) //ok
            {
                if (pos != -1)
                {
                    TLC++;
                    SI = 1;
                }
                run_mos = true;
                MOS();
                //CALL MOS
            }
            else if (!(opreator.compare("PD"))) //ok
            {
                if (pos != -1)
                {
                    TLC++;
                    SI = 2;
                }
                run_mos = true;
                //CALL MOS
            }
            else if (!(opreator.compare("Hr"))) //ok
            {
                TLC++;
                fetched_IR = "H";
                SI = 3;
                run_mos = true;
                //call MOS for Terminate
            }
            else
            {
                PI = 1;
                run_mos = true;
            }
        }
        else
        {
            run_mos = true;
        }

        if (TLC > TTL)
        {
            TI = 2;
            run_mos = true;
            terminate = true;
            terminate_code = 3;
        }
        if (run_mos)
            MOS();
    }
}
void VirtualMachine::MOS()
{
    if (TI == 0 && SI == 1)
    {
        GDCOMMAND(pos);
    }
    else if (TI == 2 && SI == 1)
    {
        terminate = true;
        terminate_code = 3;
    }

    else if ((TI == 0 || TI == 2) && SI == 2)
    {
        LLC++;
        if (LLC > TLL)
        {
            terminate = true;
            terminate_code = 2;
        }
        //put the data from memory into the file specified
        else
        {
            int pos = translateAddress(operandToLine(operand));
            PDCOMMAND(pos);
            if (TI == 2)
            {
                terminate = true;
                terminate_code = 3;
            }
        }
        SI = 0;
    }
    else if ((TI == 0 || TI == 2) && SI == 3)
    {
        //make terminate true to end the program
        //as we encountered the halt instruction

        terminate = true;
        terminate_code = 0;
    }
    else if (TI == 0 && PI == 1)
    {
        terminate_code = 4;
        terminate = true;
    }
    else if (TI == 0 && PI == 2)
    {
        terminate_code = 5;
        terminate = true;
    }

    else if (TI == 0 && PI == 3)
    {
        if (!(opreator.compare("GD")) || !(opreator.compare("SR")))
        {
            int page_no = mymemory.pageAllocate();
            mymemory.pageTable((operandToLine(operand)) / 10, page_no);
            ICsetter(ICgetter() - 1);
        }
        else
        {
            terminate_code = 6;
            terminate = true;
        }
    }

    else if (TI == 2 && PI == 1)
    {
        terminate_code = 7;
        terminate = true;
    }

    else if (TI == 2 && PI == 2)
    {
        terminate_code = 8;
        terminate = true;
    }

    else if (TI == 2 && PI == 3)
    {
        terminate_code = 3;
        terminate = true;
    }
    if (terminate)
    {
        outputfile << p_id << " ";
        switch (terminate_code)
        {
        case 0:
            outputfile << "NO ERROR\n";
            break;
        case 1:
            outputfile << "OUT OF DATA\n";
            TLC--;
            break;
        case 2:
            outputfile << "LINE LIMIT EXCEEDED\n";
            break;
        case 3:
            outputfile << "TIME LIMIT EXCEEDED\n";
            TLC = TTL;
            break;
        case 4:
            outputfile << "OPERATION CODE ERROR\n";
            break;
        case 5:
            outputfile << "OPERAND ERROR\n";
            break;
        case 6:
            outputfile << "INVALID PAGE FAULT\n";
            break;
        case 7:
            outputfile << "TIME LIMIT EXCEEDED with OPERATION CODE ERROR\n";
            break;
        case 8:
            outputfile << "TIME LIMIT EXCEEDED with OPERAND ERROR\n";
            break;
        }
        outputfile << ICgetter() << " " << fetched_IR << " " << TLC << " " << LLC << endl;
        outputfile << endl
             << endl;
    }
}

void memory::init()
{
    memset(mymemory, '$', sizeof(char) * 300 * 4);
    memset(occupied_pages, false, sizeof(bool) * 30);
    page_table_ptr = rand() % 30;
    occupied_pages[page_table_ptr] = true;
    page_table_ptr *= 10;
    SI = PI = TI = 0;
}

int memory::pageAllocate()
{
    int page_no = rand() % 30;
    while (occupied_pages[page_no] == true)
        page_no = rand() % 30;
    occupied_pages[page_no] = true;
    return page_no;
}

void memory::pageTable(int row, int pgnum)
{
    ostringstream temp;
    temp << pgnum;
    string table_entry;
    if (pgnum < 10)
        table_entry = "$10" + temp.str();
    else
        table_entry = "$1" + temp.str();
    setRow(table_entry, page_table_ptr + row);
}

void memory::cardStore(string s, int mem_cnt)
{
    string word = "";
    int page_no = pageAllocate();
    pageTable(mem_cnt, page_no);
    page_no *= 10;
    for (int i = 0; i < s.length(); i += 4)
    {
        for (int j = 0; j < 4; j++)
        {
            word += s[i + j];
        }
        setRow(word, page_no);
        page_no++;
        word = "";
    }
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
    return page_table_ptr;
}
void run(char *filename)
{
    inputfile.open(filename);
    outputfile.open("out1.txt");
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
