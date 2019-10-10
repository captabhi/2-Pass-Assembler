#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdio>
#include<sstream>

using namespace std;

int progCounter;
struct registers{
	string name;
	int pneumonics;
};
registers register1,register2,register3;

struct AssemDirective{
	string name;
	int operand;
	int value;
};
AssemDirective start,end,resb,byte;
int searchOptable(string s)
{

	string ch;
	fstream fp;
	int value;
	int flag=0;
	fp.open("opcodetable.txt");
	while(!fp.eof())
	{
		fp>>ch;	
		if(ch==s)
		{
			fp>>ch;
			fp>>ch;
			stringstream geek(ch);
			geek>>value;
			fp.close();
			return value;
		}
			
	}
	fp.close();
	return -1;
	
}
int searchDir(string s)
{
	if(s==start.name)
	return start.value;
	else if(s==resb.name)
	return resb.value;
	else if(s==byte.name)
	return byte.value;
	else if(s==end.name)
	return end.value;
	else
	return -1;
}
int searchSymbolTable(string s)
{
	string ch;
	fstream fp;
	fp.open("symboltable.txt",ios::in);
	while(!fp.eof())
	{
		fp>>ch;
		if(ch==s)
		{
			string temp;
			fp>>temp;
			if(temp=="?")
			{
				fp.close();
				return 0;    // label Found but address not defined call function to assign address
			}
			
			else
			{
			int value;
			stringstream geek(temp);
			geek>>value;
			return value; 
			fp.close();   // label found and address also defined no need to do anything	
			}
			
		}
	}
	 fp.close();
	 return -1;      //label not found anywhere add it to symbol table may be chance of error the lablel not decleraed
}
int searchRegisters(string s)
{
	if(register1.name==s)
	return register1.pneumonics;
	else if(register2.name==s)
	return register2.pneumonics;
	else if(register3.name==s)
	return register3.pneumonics;
	else return -1;
	            // not register check for label
}

int main()
{
	fstream fc,fpt,fobj;
	string word,startAddr;
	int srchRes,found=0;
	fc.open("code.txt",ios::in);
	////////////////////////////////////////
	start.name="START";
	start.operand=1;
	start.value=1008;
	end.name="END";
	end.operand=0;
	end.value=1118;
	resb.name="RESB";
	resb.operand=2;
	resb.value=1058;
	byte.name="BYTE";
	byte.operand=2;
	byte.value=1062;
	/////////////////////////////////////////
	register1.name="R1";
	register1.pneumonics=2000;
	register2.name="R2";
	register2.pneumonics=3000;
	register3.name="R3";
	register3.pneumonics=4000;
	//////////////////////////////////////////
	fobj.open("objectfile.txt",ios::out);
	if(fc.is_open())
	{
	
	while(!fc.eof())
	{
		fc>>word;
	//	cout<<word<<"\t";
		if(word==start.name)
		{
			fc>>startAddr;
			stringstream geek(startAddr);
			geek>>progCounter;
			fobj<<progCounter<<"\t";
			progCounter+=1;
		}
		else if(word==end.name)
		{
			break;
			
		}
		else 
		{
			if(searchDir(word)!=-1)
			{
				string temp;
				getline(fc,temp);
			}
			int val=searchOptable(word);
			if(val!=-1)
			{
				fobj<<val<<"\t";
				cout<<word<<"\t";
				continue;
			}
			val=searchSymbolTable(word);
			if(val!=-1||0)
			{
				fobj<<val<<"\t";
				cout<<word<<"\t";
				continue;
			}
			val=searchRegisters(word);
			if(val!=-1)
			{
				fobj<<val<<"\t";
				cout<<word<<"\t";
				continue;
			}
			if(word[0]=='#')
			{
			 char temp[20];
			int i;
			for( i=1;i<word.length();i++)
			{
			temp[i-1]=word[i];
			}
			temp[i-1]='\0';
			cout<<temp;
			fobj<<temp<<"\t";
			 //cout<<word<<"\t";
			 continue;	
			}
		}
	}
	fc.close();
	fobj.close();
}
else
cout<<"not open";
}
