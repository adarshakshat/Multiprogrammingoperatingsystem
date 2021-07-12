#include <iostream>
#include <string.h>
#include <fstream>
#include <string>

#include"memory.h"
#include"cpu.h"

using namespace std;

ifstream inputfile;
//Output File Stream
ofstream outputfile;
int SI;
memory mymemory;


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
