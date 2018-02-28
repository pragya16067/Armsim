#include <stdlib.h>
#include <stdio.h>

using namespace std;

unsigned int Mem[1001];//Instruction Register
unsigned int curr_instruct;

unsigned int Reg[16];//Register File
int N,Z;//For branching
unsigned int func;//The F bits of instruction
unsigned int imm;//The immediate flag of instruction
unsigned int opcode;//The opcode of instruction
unsigned int operandnum1;//The operand1 i.e. value in the one of the source registers
unsigned int result;//The result of the operation
unsigned int operandnum2;//The operand2 i.e. the value in the other source register
unsigned int dest ;//The destination register
unsigned int DataMem[16][1001];//Data Memory
unsigned int flag;//flag to check branch

void writeToMem(unsigned int mem[],unsigned int address,unsigned int data)
{	
	mem[address]=data;
}
unsigned int readMem()
{
	return Mem[Reg[15]];
}
void fetch()
{	
	cout<<endl;
	curr_instruct=readMem();
	printf("FETCH: Fetch instruction 0x%x from address 0x%x\n", curr_instruct, 4*Reg[15]);
	Reg[15]+=1;

}
void decode()
{
	if(curr_instruct==0xef000011)
	{
		cout<<"Memory : no memory operation"<<endl;
		cout<<"EXIT";
		exit(0);
	}
	if(curr_instruct==0xef00006b || curr_instruct == 0xef000000)//if print command
	{
		cout<<"DECODE: PRINT: Printing value from R1 "<<Reg[1];
	}
	if(curr_instruct==0xef00006c)//when user input required of integer
	{
		int a =0;
		cin>>a;
		dest =0;
		result =a;
		cout<<"INPUT: Taking User input "<<a<<"into R0";
	}
	func=(curr_instruct&0x0c000000)>>26;
	//cout<<func<<endl;
	unsigned int SourceReg, DestReg, operand2;

    if(func==0)
    {
	    cout<<"DECODE: Operation is ";
	    opcode = (curr_instruct&0x01e00000)>>21;
	    if(opcode==0)
	    { 
	      cout<<"AND";
	    }
	    else if(opcode==4)
	    { 
	      cout<<"ADD";
	    }
	    else if(opcode==2)
	    { 
	      cout<<"SUB";
	    }
	    else if(opcode==10)
	    { 
	      cout<<"CMP";
	    }
	    else if(opcode==12)
	    { 
	      cout<<"OR";
	    }
	    else if(opcode==13)
	    { 
	      cout<<"MOV";
	    }
	    else if(opcode==15)
	    { 
	      cout<<"MNV";
	    }
	

		SourceReg = (curr_instruct&0x000f0000)>>16;
	    DestReg   = (curr_instruct&0x0000f000)>>12;
	    imm = (curr_instruct&0x02000000)>>25;
	    operandnum1 = Reg[SourceReg];
	    cout<<", First operand R"<<SourceReg;
    
	    if(imm!=0)
	    {
	    	operand2 = (curr_instruct&0xff);
	      	
	      		cout<<", immediate Second operand ="<<operand2;

	        operandnum2 = operand2;
	    }
	    else
	    {
	    	operand2 = (curr_instruct&0xf);
	      	if(SourceReg==0 ) 
				cout<<", operand is R"<<operand2;
	      	else 
	      		cout<<", Second operand is R"<<operand2;
	      	operandnum2 = Reg[operand2];
	    }
	    dest = DestReg;
	    if(DestReg!=0)
	    {
	    	cout<<", Destinantion register is R"<<DestReg;
	    }
	    
	    //cout<<"immediate"<<imm<<" R2:"<<Reg[2]<<" R3:"<<Reg[3];
	    if(SourceReg!=0)
	    {	cout<<endl;
	    	cout<<"Read registers ";
	      	printf("R%d = %d", SourceReg, Reg[SourceReg]);
	      	// immediate (imm) code follows
	      	if(imm==0)
	      	{
	      		cout<<",";
	      	}
	    }
	    if(imm ==0)
	    {
	    	if(SourceReg==0)
	    	{
	    		cout<<endl<<"Read registers ";
	    	}
	    	printf("R%d = %d", operand2, Reg[operand2]);
	    }
	}

	else if(func==1)//LDR and STR
	{
		cout<<"DECODE: Operation is";
		opcode = (curr_instruct&0x03f00000)>>20;
		if(opcode == 24)
		{
			cout<<"STR";
		}
		else
		{
			cout<<"LDR";
		}

		SourceReg= (curr_instruct&0x000f0000)>>16;
    	DestReg = (curr_instruct&0x0000f000)>>12;
    	operandnum2 = curr_instruct&(0xfff);
    	dest= DestReg;
    	operandnum1 = SourceReg;

    	if(SourceReg!=0)
    	{
    		cout<<", base address at R"<<SourceReg;
    	}
    	if(DestReg!=0)
    	{
    		cout<<", destination register R"<<DestReg;
    	}

    	printf("with offset 0x%x",operandnum2);
    	cout<<endl;

    	printf("Read Register R%d = %x", SourceReg, Reg[SourceReg]);

    	if(opcode==24) 
    		printf(", R%d = %d",DestReg, Reg[DestReg]);

    }
    else if(func==2)//BRANCH
    {

	    opcode = (curr_instruct&0xf0000000)>>28;
	    operandnum2 = (curr_instruct&0xffffff);
	    printf("DECODE: Operation is Branch ");
	    if(opcode==0)
	    { 
	      cout<<"Equal, ";
	    }
	    if(opcode==1)
	    { 
	      cout<<"Not Equal, ";
	    }
	    if(opcode==11)
	    { 
	      cout<<"Less Than, ";
	    }
	    if(opcode==13)
	    { 
	      cout<<"Less Than or Equal, ";
	    }
	    if(opcode==12)
	    { 
	      cout<<"Greater Than, ";
	    }
	    if(opcode==10)
	    { 
	      cout<<"Greater Than or Equal, ";
	    }
	    if(opcode==14)
	    { //ALWAYS B
	      cout<<"Always, ";
	    }
	    
	    printf("with offset 0x%x\n", operandnum2);

    }

    
    cout<<endl;
}




void execute()
{
	cout<<"EXECUTE: ";
	//cout<<"func valllllllllllllll:"<<func;
	//if print command or user input
	if(curr_instruct==0xef00006b || curr_instruct == 0xef000000 || curr_instruct== 0xef00006c || curr_instruct == 0xef00006a)
	{
		cout<<"No execute operation"<<endl;
	}

	if(!func)
	{
		if(opcode==13)
		{ //MOV
			printf("No execute operation\n");
			result = operandnum2;
		}
		else if(opcode==4)
		{ //ADD
			printf("ADD %d and %d\n", operandnum1, operandnum2);
			result = operandnum1+operandnum2;
		}
		else if(opcode==2)
		{ //SUB
			printf("SUB %d and %d\n", operandnum1, operandnum2);
			result = operandnum1-operandnum2;
		}
		else if(opcode==0)
		{ //AND
			printf("AND %d and %d\n", operandnum1, operandnum2);
			result = operandnum1&operandnum2;
		}
		else if(opcode==12)
		{ //OR
			printf("OR %d and %d\n", operandnum1, operandnum2);
			result = operandnum1|operandnum2;
		}
		else if(opcode==15)
		{ //MNV
			printf("MNV %d\n", operandnum2);
			result = ~operandnum2;
		}
		if(opcode==10)
		{ 
			//CMP
			printf("CMP %d and %d\n", operandnum1, operandnum2);
			Z=0, N=0;
			if(operandnum1-operandnum2<0)
			{
				cout<<" N updated to 1"<<endl;N=1;
			}
			if(operandnum1-operandnum2==0)
			{
				cout<<"Z updated to 1"<<endl;Z=1;
			}
		}
	}
    else if(func ==2)
    {    	
	    cout<<"func2";
	    result = 0;
	    flag =0;
	    
	    if(opcode==0 && Z==1)
	    { //EQ
	      	flag = 1;
	    }
	    if(opcode==1 && Z==0)
	    { //NE	    
	      	flag = 1;
	      	//cout<<"FFFlagggggggg: "<<flag;
	    }
	    if(opcode==11 && N==1)
	    { //LT
	      	flag = 1;
	    }
	    if(opcode==13 &&(N==1 || Z==1))
	    { //LE
	      	flag = 1;
	    }
	    if(opcode==12 && N==0)
	    { //GT
	      	flag = 1;
	    }
	    if(opcode==10 && N==0 && Z==1)
	    { //GE
	      	flag = 1;
	    }
	    if(opcode==14)
	    { //AL
	      	flag = 1;
	    }

	    if(flag==1)
	    {
	    	signed int off;
	    	int broken = ((operandnum2>>23)&&1);
	    	if(broken)
	    	{
	    		off = ((0xff000000)|(operandnum2));
	    	}
	    	else
	    	{
	    		off = operandnum2;
	    	}
	    	Reg[15]+=(1+off);
	    	cout<<"Updating PC to 0x"<<Reg[15]<<endl;
	    }
	    else
	    {
	    	cout<<"No execute operation";
	    }
	    cout<<endl;
    }
	else if(func ==1)
		cout<<"No execute operation"<<endl;	

}


void WB()
{
	printf("WRITEBACK: ");
	//if user input
	if(curr_instruct==0xef00006b || curr_instruct == 0xef000000)//if print command
	{
		cout<<"No WriteBack operation";
	}
	if(curr_instruct==0xef00006c && dest==0)
	{
		Reg[dest] = result;
	    printf("write %d to R%d", result, dest);
	}
  	if(!func)
  	{
	    if(dest!=0)
	    {
	      Reg[dest] = result;
	      printf("write %d to R%d", result, dest);
	    }
	    else
	      cout<<"No WriteBack operation";
    }
    else if(func == 1 && opcode ==25)
    {
    	Reg[dest] = result;
    	printf("write %d to R%d", result, dest);
    }
    else if((func == 1 && opcode==24) || func ==2 )
    {
    	cout<<"No WriteBack Operation";
    }
    cout<<endl;

}
void readMemFile(const char* filename)
{
	//SP initialised here
	Reg[13] = 1000;
	FILE *f;
	f=fopen(filename,"r");
	unsigned int address,instruction;
	if(f==NULL)
	{
		printf("Cannot open mem file\n");
		exit(1);
	}
	else
	{
		while(fscanf(f,"%X %X",&address,&instruction)!=EOF)
		{
			address=address/4;
			cout<<address<<" "<<instruction<<endl;
			writeToMem(Mem,address,instruction);
		}
	}
}
//For LDR and STR
void mem()
{
	cout<<"MEMORY: ";
	//if user input
	if(curr_instruct==0xef00006b || curr_instruct == 0xef000000 || curr_instruct== 0xef00006c || curr_instruct == 0xef00006a)//if Print Command
	{
		cout<<"No Memory operation";
	}
	if (func==1)
	{
		if(opcode==25)
		{	
			result=DataMem[operandnum1][operandnum2/4];
			printf("Read %d from address 0x%x", result, &DataMem[operandnum1][operandnum2/4]);
		}

		if(opcode==24)
		{
			DataMem[(int)operandnum1][(int)operandnum2/4] = Reg[dest];
			printf("Write %d to address 0x%x", Reg[dest], &DataMem[operandnum1][operandnum2/4]);
		}

	}	

	if(func == 2 || func ==0)
	{
		cout<<" No Memory operation";
	}

	cout<<endl;
}
void run()
{
	while(!false)
	{
		fetch();
		decode();
		execute();
		mem();
		WB();
	}
}
