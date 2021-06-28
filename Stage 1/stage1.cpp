#include<iostream>
#include<string.h>
#include<fstream>
#include<string>

using namespace std;

ifstream inputfile;
ofstream outputfile;
int SI;
class Storage
{
	private:
		char memory[100][4];
		char ch;
		string IR,R;
		int IC;
		bool C;
		
	public:
		void reset()
		{
			memset(memory,'$',sizeof(char)*100*4);
			IR=R="";
			IC=0;
			C=false;
		}
		string getRow(int pos)
		{
			string temp="";
			for(int i=0;i<4;i++)
				temp+=memory[pos][i];
			return temp;
		}
		void setRow(string s, int pos)
		{

			for(int i=0;i<4;i++)
				memory[pos][i]=s[i];
		}
		void getJob()
		{
			int flag=0;
			for(int i=0;i<100;i++)
			{
				for(int j=0;j<=3;j++)
				{
					inputfile>>ch;
					memory[i][j]=ch;
					if(memory[i][0]=='H')
					{
						flag=1;
						break;
					}
				}
				if(flag)
					break;
			}
		}
		void set_IC()
		{
			IC=0;
		}
		void set_IC(int pos)
		{
			IC=pos;
		}
		int get_IC()
		{
			return (IC++);
		}	
		void set_IR(int IC)
		{
			IR="";
			for(int i=0;i<4;i++)
			{
				IR+=memory[IC][i];
			}
		}
		string get_IR()
		{
			return IR;
		}
		void set_R(int pos)
		{
			R="";
			for(int i=0;i<4;i++)
				R+=memory[pos][i];
		}
		string get_R()
		{
			return R;
		}
		void set_C(bool value)
		{
			C=value;
		}
		bool get_C()
		{
			return C;
		}
};
Storage memory;



class VirtualMachine
{
	private:
		int fetched_IC;
		bool terminate,fetched_C;
		string fetched_IR,operand,opreator,fetched_R,compare_string;
	public:
		int operandtoline(string operand)
		{
			return ((int)operand[0]-48)*10+((int)operand[1]-48);
		}
		void FETCH()
		{
			memory.set_IC();
			DECODE();
		}		
		void DECODE()
		{
			terminate=false;
			while(!terminate)
			{
				fetched_IC = memory.get_IC();
				memory.set_IR(fetched_IC);
				fetched_IR=memory.get_IR();
				opreator=fetched_IR.substr(0,2);
				operand=fetched_IR.substr(2,2);
				if(!(opreator.compare("LR")))
				{
					int pos=operandtoline(operand);
					memory.set_R(pos);
				}
				else if (!(opreator.compare("SR")))
				{
					fetched_R=memory.get_R();
					int pos=operandtoline(operand);
					memory.setRow(fetched_R, pos);
				}
				else if (!(opreator.compare("CR")))
				{
					fetched_R=memory.get_R();
					int pos=operandtoline(operand);
					compare_string=memory.getRow(pos);
					if(fetched_R.compare(compare_string)==0)
						memory.set_C(true);
					else
						memory.set_C(false);
				}
				else if (!(opreator.compare("BT")))
				{
					fetched_C=memory.get_C();
					if(fetched_C)
					{
						int pos=operandtoline(operand);
						memory.set_IC(pos);
					}
				}
				else if (!(opreator.compare("GD")))
				{
					SI=1;
					EXCUTE();
				}
				else if (!(opreator.compare("PD")))
				{
					SI=2;
					EXCUTE();
				}
				else
				{
					SI=3;
					EXCUTE();
				}
			}
		}
		void EXCUTE()
		{
			if(SI==1)
			{
				string s;
				int pos=operandtoline(operand);
				pos=(pos/10)*10;
				getline(inputfile,s);
				if(!s.empty() && s[s.size()-1]=='\r')
					s.erase(s.size()-1);
				int len=s.length(),start=0,i;
				string s1;
				for(i=pos;start<len;i++)
				{
					if((len-start)<4)
						s1=s.substr(start,(len-start));
					else	
						s1=s.substr(start,4);
					start+=4;
					memory.setRow(s1,i);
				}
			}
			else if(SI==2)
			{
				int pos=operandtoline(operand),flag=0;
				pos=(pos/10)*10;
				string ans="",temp="";
				for(int i=pos;i<pos+10;i++)
				{
					temp=memory.getRow(i);
					for(int j=0;j<4;j++)
					{
						if(temp[j]=='\0' || temp[j]=='$')
						{
							break;
							flag=1;
						}
						ans+=temp[j];
					}
					if(flag)
						break;
				}
				outputfile<<ans<<endl;
			}
			else
			{
				terminate=true;
				outputfile<<endl<<endl;
			}
		}
};
VirtualMachine virtualmachine;

void run(char *filename){
    inputfile.open(filename);
	outputfile.open("output.txt");
	string s,s1;
	while(!(inputfile.eof()))
	{
		getline(inputfile,s);
		if(s.find("$AMJ")!=-1)
		{
			memory.reset();
			memory.getJob();
			continue;
		}
		else if(s.find("$DTA")!=-1)
			virtualmachine.FETCH();
		else if(s.find("$END")!=-1)
			continue;
	}
	inputfile.close();
	outputfile.close();
	
}

int main(int argc, char *argv[])
{   
    if (argc < 2) {
		cout  << argv[0] << " Requires an Input file Name in argument\n" << endl;
		return 0;
	}
    run(argv[1]);
	return 0;
}