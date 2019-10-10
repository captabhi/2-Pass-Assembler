#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdio>
#include<sstream>
using namespace std;
int progCounter;

struct registers{
	string name;
	string pneumonics;
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
			//cout<<"Found:";
			//flag=1;
			//cout<<fp.tellp()<<endl;
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
	return start.operand;
	else if(s==resb.name)
	return resb.operand;
	else if(s==byte.name)
	return byte.operand;
	else if(s==end.name)
	return end.operand;
	else
	return -1;
}
void addSymbol(string s)
{
	string ch;
	fstream fp;
	fp.open("symboltable.txt",ios::app);
	fp<<s<<"\t"<<progCounter<<"\t";
	progCounter+=1;	
	fp.close();	
}
void addSymbolWithoutAddress(string s)
{
	string ch;
	fstream fp;
	fp.open("symboltable.txt",ios::app);
	fp<<s<<"\t"<<"?"<<"\t";	
	fp.close();	
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
			fp.close();
			return 1;    // label found and address also defined no need to do anything	
			}
			
		}
	}
	 fp.close();
	 return -1;      //label not found anywhere add it to symbol table may be chance of error the lablel not decleraed
}
int searchRegisters(string s)
{
	if(register1.name==s||register2.name==s||register3.name==s)
	return 1;
	else return -1;
	            // not register check for label
}
void updateAddressSymbolTable(string s)
{
	fstream fp,fptemp;
	string ch,ch2;
	fp.open("symboltable.txt",ios::in);
	fptemp.open("symboltabletemp.txt",ios::out);
	while(!fp.eof())
	{
		fp>>ch;
		if(ch==s)
		{
		fptemp<<ch<<"\t";
		fp>>ch;	
		if(ch=="?"){
		   
		   fptemp<<progCounter<<"\t";
		}
		}
		else
		{
			fptemp<<ch<<"\t";
		}
	}
	fp.close();
	fptemp.close();
	cout<<"STATUS"<<remove("symboltable.txt")<<endl;
	cout<<"STATUS"<<rename("symboltabletemp.txt","symboltable.txt")<<endl;
	
}
int main()
{
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
	register1.pneumonics=10;
	register2.name="R2";
	register2.pneumonics=11;
	register3.name="R3";
	register3.pneumonics=12;
	//////////////////////////////////////////
	
	fstream fp,fpt;
	string word,startAddr;
	int srchRes,found=0;
	fstream fptemp1;
	fptemp1.open("symboltable.txt",ios::out);
	fptemp1.close();
	fptemp1.open("symboltabletemp.txt",ios::out);
	fptemp1.close();
	fp.open("code.txt");
	
	while(!fp.eof())
	{
		fp>>word;
		if(word==start.name)
		{
			fp>>startAddr;
			progCounter+=1;
			break;
		}
	}
	stringstream geek(startAddr);
	geek>>progCounter;
    cout<<progCounter;	
    while(!fp.eof())
    {
    	found=0;
		fp>>word;
    	srchRes=searchDir(word);
    	cout<<"Searching Assembly Directive for : "<<word<<endl;
    	if(srchRes!=-1)
    	{
    		progCounter+=1;   //for AssemblerDirective
			if(srchRes==2)
    		{
    			string temp1,temp2;
				fp>>temp1;
				fp>>temp2;
				if(temp2!="?")
				{
					int srchRes1=searchSymbolTable(temp1);
					if(srchRes1==-1)
					{
					addSymbol(temp1);
					cout<<"ADDING SYMBOL for: "<<temp1<<endl;	
					}
					else if(srchRes1==1)
					{
						cout<<"Variable already presnet, Removing Error code\n";
					}
				}
				else
				{
					int srchRes1=searchSymbolTable(temp1);
					if(srchRes1==-1)
					{
					addSymbolWithoutAddress(temp1);
					cout<<"ADDING SYMBOL WITHOUT ADDRESS for : "<<temp1<<endl;	
					}
					else if(srchRes1==1)
					{
						cout<<"Variable already presnet, Removing Error code\n";
					}
					
				}
				
						
			}
			if(srchRes==0)
			{
				break;
			}
			continue;	 
		}
		srchRes=searchOptable(word);
		cout<<"Searching Optable for : "<<word<<endl;
		if(srchRes!=-1)
		{
			progCounter+=1;
			while(srchRes>0)
			{
				string temp;
				int srchRes1;
				fp>>temp;
				if(temp[0]==',')
				continue;
				if(temp[0]!='#')
				{
				
				srchRes1=searchRegisters(temp);
				cout<<"Searching Registers for : "<<temp<<endl;
				if(srchRes1==-1)
				{
				 int srchRes2=searchSymbolTable(temp);
				 cout<<"Searching Symbol Talbe for : "<<temp<<endl;
				if(srchRes2==-1)
				{
					addSymbol(temp);
					cout<<"ADDING SYMBOL WITH error code ADDRESS for : "<<temp<<endl;
				}
				else if(srchRes2==0)
				{
					updateAddressSymbolTable(temp);
					cout<<"Updating  ADDRESS for : "<<temp<<endl;
				}	
				}
			}
			else
			cout<<"Immidiate Operand: "<<temp<<endl;
			 progCounter+=1;
			 srchRes--;			
			}
			
		}
		else
		{
			int len=word.length(),i;
			len--;
			if(word[len]==':')
			{
				cout<<"Label Found:";
				char temp[len];
				for( i=0;i<len;i++)
				{
					temp[i]=word[i];
				}
				temp[i]='\0';
				cout<<temp<<"\n";
				string str(temp);
				addSymbol(str);
			}
			}	
	}		
}
