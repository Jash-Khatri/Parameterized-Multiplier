/***
Author : Jash Khatri (CS19S018)
Time of creation : 14 November 2020 at 15:26 IST
Code for : automatic generation of the parameterized and pipelined wallace tree multiplier
***/
/***
In nutshell the code works as follows:
First we generate the partial products. we then add all the generated partial products(items) to the queue.
Then each time we just remove first three items from the queue and give it to CSA and generated output is again added to the queue at its rear.
This process is continued till we are just left with 2 items in the queue. At this point we remove those two items from queue and add them using CLA.
During this process we check if we need to add the register stage before any of the CSA stage for the pipelining purpose.
We also optimize the CSA size for each CSA used in the CSA tree by neglecting the bits with just zero's in it. 
Comments are added to the code where needed in order to make it self-explainatory.
**/

#include<iostream>
#include<bits/stdc++.h>
#include<math.h>
using namespace std;

// Instances of this structure along with queue(used in algorithm) will be used to generate the CSA tree. 
struct item{
	int stage;  // stage to which the instance belong
	int num;	// gives a unique number to instances
	int relbits;	// number of relevent bit in the instance. WIll help to reduce the size of CSA by droping unneccessary bits.
	int shift;		// records the amount of shift the input carrys. Will help to determine if input instance is shifted or not. If yes then how by much amount.
};

// this functions determines the output size of the two outputs of CSA i.e. rv[0] and rv[1] given the inputs sizes a,b,c 
void sizefinder(int *rv,int a,int b,int c){
	
	int temp;
	int valmax;
	temp = max(a,b);
	valmax = max(temp,c);

	if(a == valmax && b != valmax && c != valmax){
		rv[0] = valmax;
		rv[1] = valmax;
		//return rv;
	}
	else if(a != valmax && b == valmax && c != valmax){
		rv[0] = valmax;
		rv[1] = valmax;
		//return rv;
	}
	else if(a != valmax && b != valmax && c == valmax){
		rv[0] = valmax;
		rv[1] = valmax;
		//return rv;
	}
	else{
		rv[0] = valmax;
		rv[1] = valmax+1;
	}

}

int main(){

	int n;		// size of multiplicand and multiplier
	int m;		// size of the CLA attached to the CSA tree at the end.
	int kstage;  // Number of stages after which to put the register stage for pipelining.
	cin >> n;    // input bit for multiplicand and multiplier.
	cin >> kstage;	 // put register stage after each k CSAstages.
	m = 2*n;

	// Opening the file PP.v. This file will contain the self-generate code for the pipelined parameterized wallace tree multiplier.
 	FILE *filePointer;
  	filePointer = fopen("multiplier.v", "w") ; 

  if( filePointer == NULL ) 
    { 
        printf( "file failed to open." ) ; 
    }

    // code to geneate the m bit CLA starts.
      //Below line generates the code to implement the circuit to obtain the propagate,generate,kill terms from the input bits.
  fprintf(filePointer,"module pandgterms(a,b,st1,st0);\n\ninput a,b; \noutput st1,st0; \nassign #2 st1=a&b; \nassign #2 st0=a|b; \n\nendmodule\n\n");
  
  //Below line generates the code to implement the Star operator which is an semi-group operator and operates on the propagate,generate,kill terms.
  fprintf(filePointer,"module starop(cs1,cs0,ps1,ps0,o1,o0); \ninput cs1,cs0,ps1,ps0; \noutput o1,o0; \n\nassign #4 o1 = cs1 | (cs0 & ps1); \nassign #4 o0 = cs1 | (cs0 & ps0); \n\nendmodule\n\n");
  
  //Below line generates the code to implement simple module which implements the 3 input XOR gate.
  fprintf(filePointer,"module bitXor(a,b,c,out); \ninput a,b,c; \noutput out; \n\nassign #6 out = a^b^c; \n\nendmodule\n\n");


  // code to generate the N-bit CLA module.
  fprintf(filePointer,"module CLA(X,Y,Co,S);\n");

//Code that generates the verilog code that defines input output and some wire which are used later. 
fprintf(filePointer,"input [%d:0]X,Y; \noutput [%d:0]S; \noutput Co; \nwire [%d:0]st1; \nwire [%d:0]st0;\n\n",m-1,m,m-1,m-1);

// Code to generate the propagate,generate,kill terms. This terms will be then feeded to the First level of the Star operators.
for(int i=0;i<m;i++){
  fprintf(filePointer,"pandgterms a%d(X[%d],Y[%d],st1[%d],st0[%d] );\n",i,i,i,i,i);
}

// Code to generate the First level of the Star operators in CLA circuit. 

fprintf(filePointer,"\nwire [%d:0]l01; \nwire [%d:0]l00; \n\nstarop b0(st1[0],st0[0],1'b0,1'b0,l01[0],l00[0]);\n",m-1,m-1);

for(int i=1;i<m-1;i++){
fprintf(filePointer,"starop b%d(st1[%d],st0[%d],st1[%d],st0[%d],l01[%d],l00[%d]);\n",i,i,i,i-1,i-1,i,i);
}

fprintf(filePointer,"starop b%d(st1[%d],st0[%d],st1[%d],st0[%d],l01[%d],l00[%d]);\n\n",m-1,m-1,m-1,m-2,m-2,m-1,m-1);

// Code to generate the rest of the levels of the Star operators in CLA circuit.
int x ;
int stop;
int k1 = 1;
int level = 1;

while(true){
x = (pow(2,k1)-1);
stop = m - (pow(2,k1)-1);
if(stop<=0)
  break;
fprintf(filePointer,"\n");
fprintf(filePointer,"wire [%d:0]l%d1; \nwire [%d:0]l%d0;\n\n",m-1,level,m-1,level);
for(int i=0;i<x;i++){
  fprintf(filePointer,"assign l%d1[%d] = l%d1[%d]; \n",level,i,level-1,i );
  fprintf(filePointer,"assign l%d0[%d] = l%d0[%d]; \n",level,i,level-1,i );
}

  int j=0;
    fprintf( filePointer,"starop c%d0(l%d1[%d],l%d0[%d],1'b0,1'b0,l%d1[%d],l%d0[%d]);\n",level,level-1,x,level-1,x,level,x,level,x );
  
  for(int i=x+1;i<m;i++){
    j++;
   fprintf( filePointer,"starop c%d%d(l%d1[%d],l%d0[%d],l%d1[%d],l%d0[%d],l%d1[%d],l%d0[%d]);\n",level,j,level-1,i,level-1,i, level-1, j-1, level-1,j-1 ,level,i,level,i );
  }

k1++;
level++;
}

// Code to generate the rest of the levels of the Star operators in CLA circuit ends.


//Code to generate the XORs that takes bit Xi, Yi with the generated carry bits l(log_2 N)1[i] as inputs to get the sum bits S[i] for all i.
fprintf(filePointer,"\n");
fprintf(filePointer,"bitXor x0( X[0], Y[0], 1'b0, S[0] );\n");

for(int i=1;i<m;i++)
{
fprintf(filePointer,"bitXor x%d( X[%d], Y[%d], l%d1[%d], S[%d] );\n",i,i,i,level-1,i-1,i);
}

fprintf(filePointer,"assign S[%d] = l%d1[%d];\n\n",m,level-1,m-1);
//Generates code which assign l(log_2 N)1[N-1] to the Cout that is carry out obtained after summing X and Y.
fprintf(filePointer,"assign Co = l%d1[%d];\n\n",level-1,m-1);

fprintf(filePointer,"endmodule\n\n\n\n");


// CLA gen end.....

// module to generate the partial products starts here. This module basically ANDs the bits of multiplicands and multiplier and adds the neccessary shifts...
// p1,p2...,pn will the hold the individual partial product. 
	fprintf(filePointer,"module PP(X,Y");

	for(int i=1;i<=n;i++){
		fprintf(filePointer,",p%d",i);
	}
	fprintf(filePointer,");\n\n");

	fprintf(filePointer,"input [%d:0]X,Y; \n",n-1);

	fprintf(filePointer,"output [%d:0]", 2*n-2 );

	for(int i=1;i<=n;i++){
		if(i == 1)
		fprintf(filePointer,"p%d",i);
		else
		fprintf(filePointer,",p%d",i);
	}
	fprintf(filePointer,";\n\n");

	int k=0;
	int pnum =1;
	while(k<n){
	
	for(int i=0;i<k;i++){
		fprintf(filePointer,"assign #2 p%d[%d] = 1'b0;\n",pnum,i);
	}
	int j=0;
	for(int i=k;i<n+k;i++){
		fprintf(filePointer,"assign #2 p%d[%d] = X[%d] & Y[%d];\n",pnum,i,j,k);
		j++;
	}
	for(int i=n+k;i<2*n-1;i++){
		fprintf(filePointer,"assign #2 p%d[%d] = 1'b0;\n",pnum,i);
	}

	fprintf(filePointer,"\n\n");
	k++;
	pnum++;
	}

	fprintf(filePointer,"endmodule\n\n");

// module to generate partial product ends here...

// module to generate the full adder starts here..
	fprintf(filePointer,"module fulladder(a,b,cin,cout,sum); \n");
	fprintf(filePointer,"input a,b,cin; \n");
	fprintf(filePointer,"output cout,sum; \n");
	fprintf(filePointer,"assign #6 cout= (b&cin | a&b | cin&a );\n");
	fprintf(filePointer,"assign #6 sum=a^b^cin; \nendmodule \n");
  
  	fprintf(filePointer,"\n\n");
// module to generate full adder ends here...

	// This part of code generates all the required bit length CSAs for the CSA stages. 
	//For example for N=4 it will generate CSA which can add 3 six bit numbers and 3 seven bit numbers....  
  	for(int j=(n+2);j<(2*n+1);j++){

  	fprintf(filePointer,"module CSA%dbits(p1,p2,p3,sum,carry);\n\n",j);
  	fprintf(filePointer,"input [%d:0]p1,p2,p3;\n",2*n-1);
  	fprintf(filePointer,"output [%d:0]sum,carry;\n",2*n);
  	fprintf(filePointer,"assign carry[0] = 1'b0;\n");

  	for(int i=0;i<j;i++){
  		fprintf(filePointer,"fulladder f%d(p1[%d],p2[%d],p3[%d],carry[%d],sum[%d]);\n",i+1,i,i,i,i+1,i);
  	}

  	fprintf(filePointer,"\n");
  	// adjust sums
  	for(int i=j;i<(2*n+1);i++ ){
  		fprintf(filePointer,"assign sum[%d] = 1'b0;\n",i);
  	}
  	//adjust carries
  	fprintf(filePointer,"\n");
  	for(int i=j+1;i<(2*n+1);i++ ){
  		fprintf(filePointer,"assign carry[%d] = 1'b0;\n",i);
  	}

  	fprintf(filePointer,"\n");
  	fprintf(filePointer,"endmodule\n\n"); 

	}

	// CSA generation code ends....

	//code to generate all the required shifter modules. This will allow us to shift the array of bits to the left and while shifting the MSB bits will be droped.
	// This will be helpfull during the space optimization when we need to decrease the size of CSA.
	// This will be needed because let say 18 bit CSA might just add three 18 bit number with shifts (+3) (+3) (+6). 
	//But before CSA addition of this three number we need to shift the last number by 3 bits to make all the shifts equal.    
	for(int i=1;i<n;i++){
		if(i%3 == 0){
			//generate the particular shifter module here...
			fprintf(filePointer,"\n");
			fprintf(filePointer,"module shifter%dbits(a,b);\n\n",i);
			fprintf(filePointer,"input [%d:0]a;\n",2*n-1);
			fprintf(filePointer,"output [%d:0]b;\n\n",2*n-1);

			//shift the bits and assign to b
			for(int j=(2*n-1-i);j>=0;j--){
				fprintf(filePointer,"assign b[%d+%d] = a[%d];\n",j,i,j); 
			}
			// make lsb bits of b as zeros
			for(int j=0;j<i;j++){
				fprintf(filePointer,"assign b[%d] = 1'b0;\n",j); 
			}
			fprintf(filePointer,"\n");
			 fprintf(filePointer,"endmodule\n\n"); 
		}

	}

	//Code to generate shifter module ends...

  	// code to generate the CSA stages + CLA at the end point starts here....
	fprintf(filePointer,"module CSAStages(A,B,clk,Result,cout);\n\n");  	 

	// A and B are multiplicand and multiplier respectively and clk is clock input to the circuit.
	fprintf(filePointer,"input [%d:0]A,B;\n",n-1);
	fprintf(filePointer,"input clk;\n");

	//wires l01...l0n will store all the partial products.. 
	fprintf(filePointer,"wire [%d:0]",2*n-2);
	for(int i=0;i<n;i++){
		if(i == 0)
		fprintf(filePointer,"l0%d", i+1);
		else
		fprintf(filePointer,",l0%d", i+1);
	}
	fprintf(filePointer,";\n\n");

	// results of multiplication of A and B will be stored in the Result.
	// cout will store the carry geneated by the CLA which is attached at the end. cout can be ignored. Just added for testing purposes
	fprintf(filePointer,"output [%d:0]Result;\n",2*n);	
	fprintf(filePointer,"output cout;\n\n");

	// instiantiate the module that calcuate the partial products from A and B.
	fprintf(filePointer,"\nPP p1(A,B,");	
	for(int i=0;i<n;i++){
		if(i == 0)
		fprintf(filePointer,"l0%d", i+1);
		else
		fprintf(filePointer,",l0%d", i+1);
	}
	fprintf(filePointer,");\n");	

	// This queue will be used to geneate the CSA tree. each time 3 inputs wiil be dequeued and 2 inputs will be enqueued to generate the CSA tree in the below algorithm using this queue.
	queue <item> q1;
	item a1;
	int ctr = 0;

	//Push all the initial level of CSA in pipeline also set the stage,num,relbits and shifts for initial level
	for(int i=0 ; i<n ; i++ ){
		a1.stage = 0;
		a1.num = i+1;
		
		if(i%3 == 0)
			a1.relbits = n;
		if(i%3 == 1)
			a1.relbits = n+1;
		if(i%3 == 2)
			a1.relbits = n+2;

		a1.shift = ctr;

		if(i%3 == 2)
			ctr += 3;

		if(i >= (n/3)*3 )
		{
			a1.shift = 0;
			a1.relbits = i+n;
		}

		//cout << a1.shift << " " << a1.relbits << "\n";
		q1.push(a1);
	}

	item i1,i2,i3;  // used to hold first three dequeued items from the queue.
	item o1,o2;  // used to hold two items that will be enqueued back to the queue. basically this is how queue will help to generalize the 3-to-2 reducer to geneate CSA tee.
	int stage;	// stores the max stage of the three input items.
	int minstage; // stores the min stage of the three input items.
	int bitcsa;		// stores the bit length of the CSA that needs to be used at some particular point in the CSA tree.
	int maxshift;	// stores which item out of i1,i2,i3 holds the maximum shift   
	int minshift;	// stores which item out of i1,i2,i3 holds the minimum shift
	int temp;		// used to store the temporary values
	int CSAnum=1;	// used to give unique ids to the generated instances of the CSA modules
	int counter=1;	// used to give unique ids to the items present in the queue. This id will be stored as the part of item.num
	int rval[2];	// This array stores the result of sizefinder function. The array is passed as reference to this function
	int shiftcount=1; // used to provide the unique ids to the instances of the shifter modules

	int flags1;		// this flag tells us if 1st input of all 3 inputs of CSA is shifted or not. 1 if shifted 0 if not
	int flags2;		// this flag tells us if 2nd input of all 3 inputs of CSA is shifted or not. 1 if shifted 0 if not
	int flags3;		// this flag tells us if 3rd input of all 3 inputs of CSA is shifted or not. 1 if shifted 0 if not
	int flagprev1;	// this flag tells us that if previous stage of 1st input of CSA was the stage where we need to add the pipeline register. 1 if yes 0 if no
	int flagprev2;	// this flag tells us that if previous stage of 2nd input of CSA was the stage where we need to add the pipeline register. 1 if yes 0 if no
	int flagprev3;	// this flag tells us that if previous stage of 3rd input of CSA was the stage where we need to add the pipeline register. 1 if yes 0 if no
	int shouldaddreg;	// this flag tell us if we need to add the register stage before the particular CSA stage or not. 1 if yes 0 if no

	// if n=2 then this while loop will not execute.. (exactly what is needed). 
	// This while loop will execute and will halt only when CSA tree reduces the N numbers (N partial products) to the two numbers. this two numbers are then added using the CLA.  
	while(q1.size() != 2){
		// pop the first three items in the queue and store it in the i1,i2,i3
		i1 = q1.front();
		q1.pop();
		i2 = q1.front();
		q1.pop();
		i3 = q1.front();
		q1.pop();

		// reset all the flags before each iteration
		flags1=0;
		flags2=0;
		flags3=0;
		flagprev1=0;
		flagprev2=0;
		flagprev3=0;
		shouldaddreg=0;
		int tempcount=0;

		// if all the three inputs to the CSA has same amount of shift then we can simply add them with out any further shifting.
		if(i1.shift == i2.shift && i2.shift == i3.shift){
			sizefinder(rval,i1.relbits,i2.relbits,i3.relbits);
			//cout << i1.shift << "\n\n";

			temp = max(i1.stage,i2.stage);
			stage = max(temp,i3.stage);

			temp = min(i1.stage,i2.stage);
			minstage = min(temp,i3.stage);

			temp = max(i1.relbits,i2.relbits);
			bitcsa = max(temp,i3.relbits);
			//bitcsa += i1.shift;

			// o1 is the first output of the CSA. its parameters are set appropriately
			o1.stage = stage+1;
			o1.num = counter;
			o1.relbits = rval[0];
			o1.shift = i1.shift;
			counter++;
			// o2 is the second output of the CSA. its parameters are set appropriately
			o2.stage = stage+1;
			o2.num = counter;
			o2.relbits = rval[1];
			o2.shift = i1.shift;
			counter++;

			// set the should add reg flag if stage value is multipler of kstage. that is add the register stage at each kstage of CSA tree.
			if( stage % kstage == 0 && stage != 0)
				shouldaddreg = 1;
			
			}

			// if all the three inputs to the CSA have different amount of shifts then make the shifts of all the three inputs equal before performing addition using CSA.
			else{
			sizefinder(rval,i1.relbits,i2.relbits,i3.relbits);
			//cout << i1.shift <<" " << i2.shift << " "<< i3.shift << "\n\n";

			temp = max(i1.stage,i2.stage);
			stage = max(temp,i3.stage);
			
			temp = min(i1.stage,i2.stage);
			minstage = min(temp,i3.stage);

			temp = max(i1.shift,i2.shift);
			maxshift = max(i3.shift,temp);
			
			temp = min(i1.shift,i2.shift);
			minshift = min(i3.shift,temp);

			// if  i1's shift is more then 0 ajdust its shift to zero
			if(i1.shift > 0){
				int surplus = i1.shift;
				fprintf(filePointer,"wire [%d:0]l%d%dx;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"shifter%dbits s%d(l%d%d,l%d%dx);\n",surplus,shiftcount,i1.stage,i1.num,i1.stage,i1.num);
				shiftcount++;
				i1.shift = 0;
				flags1 = 1;
				//fprintf(filePointer,"l%d%d <= l%d%d <<  %d;\n",i1.stage,i1.num,i1.stage,i1.num,surplus);
			}
			// if  i2's shift is more then 0 adjust its shift to zero
			if(i2.shift > 0){
				int surplus = i2.shift;
				fprintf(filePointer,"wire [%d:0]l%d%dx;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"shifter%dbits s%d(l%d%d,l%d%dx);\n",surplus,shiftcount,i2.stage,i2.num,i2.stage,i2.num);
				shiftcount++;
				i2.shift = 0;
				flags2 = 1;
				//fprintf(filePointer,"l%d%d <= l%d%d <<  %d;\n",i2.stage,i2.num,i2.stage,i2.num,surplus);
			}
			// if  i1's shift is more then 0 adjust its shift to zero
			if(i3.shift > 0){
				int surplus = i3.shift;
				fprintf(filePointer,"wire [%d:0]l%d%dx;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"shifter%dbits s%d(l%d%d,l%d%dx);\n",surplus,shiftcount,i3.stage,i3.num,i3.stage,i3.num);
				shiftcount++;
				i3.shift = 0;
				flags3 = 1;
				//fprintf(filePointer,"l%d%d <= l%d%d <<  %d;\n",i3.stage,i3.num,i3.stage,i3.num,surplus);
			}

			temp = max(i1.relbits,i2.relbits);
			bitcsa = max(temp,i3.relbits) + maxshift;

			// o1 is the first output of the CSA. its parameters are set appropriately
			o1.stage = stage+1;
			o1.num = counter;
			o1.relbits = rval[0]+maxshift;
			o1.shift = 0;
			counter++;
			// o2 is the second output of the CSA. its parameters are set appropriately
			o2.stage = stage+1;
			o2.num = counter;
			o2.relbits = rval[1]+maxshift;
			o2.shift = 0;
			counter++;

			if( stage % kstage == 0 && stage != 0)
				shouldaddreg = 1;

			}

			//push the first output of the CSA to the rear of the queue
		q1.push(o1);
			//push the second output of the CSA to the rear of the queue			
		q1.push(o2);

			// set the CSA bit length to 2*n just in case it exceeds then 2*n
		if(bitcsa > 2*n)
			bitcsa = 2*n;

			// if the shouldaddreg flag is set then we need to add the register stage before the CSA stage 
			// after that we instantiate the CSA module to do actual CSA addition.
		if(shouldaddreg){
			//fprintf(filePointer,"reg [%d:0]l%d%d,l%d%d;\n",2*n-1,o1.stage,o1.num,o2.stage,o2.num);

			fprintf(filePointer,"wire [%d:0]l%d%d,l%d%d;\n",2*n-1,o1.stage,o1.num,o2.stage,o2.num);
			
			if( !flags1 && !flags2 && !flags3){
			
			fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);

			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);

			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

				fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
				i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

			if( !flags1 && !flags2 && flags3){
			
			fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			}

			if( !flags1 && flags2 && !flags3){

			fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

			if( !flags1 && flags2 && flags3){
		
			fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

			if( flags1 && !flags2 && !flags3){
		
			fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

			if( flags1 && !flags2 && flags3){
		
			fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

			if( flags1 && flags2 && !flags3){
		
			fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%d <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");

		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

			if( flags1 && flags2 && flags3){
			fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
			fprintf(filePointer,"reg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);

			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i1.stage,i1.num);
			}
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i2.stage,i2.num);
			}
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"reg [%d:0]r%d%dtemp;\n",2*n-1,i3.stage,i3.num);
			}

			fprintf(filePointer,"always @(negedge clk)\n begin\n");
			
			if(stage > i1.stage && i1.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i1.stage,i1.num,i1.stage,i1.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
			
			if(stage > i2.stage && i2.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i2.stage,i2.num,i2.stage,i2.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
			
			if(stage > i3.stage && i3.stage != 0 && ( (stage-1) % kstage == 0 ) ){
				fprintf(filePointer,"r%d%dtemp <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"r%d%dx <= r%d%dtemp;\n",i3.stage,i3.num,i3.stage,i3.num);
			}
			else
			fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
			
			fprintf(filePointer,"end\n\n");
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
			}

		}
		// adding the register stage before the CSA stage and instianting the CSA module to do actual CSA addition ends here..

		// if the shouldaddreg flag is not set then check if the previous stage was the stage where we should add the register stage. if so then add the registers at the particular places need.
		// then after that we instantiate the CSA module to do actual CSA addition.
		else{
		fprintf(filePointer,"wire [%d:0]l%d%d,l%d%d;\n",2*n-1,o1.stage,o1.num,o2.stage,o2.num);

		if( !flags1 && !flags2 && !flags3){

			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}
			
			if(!flagprev1 && !flagprev2 && !flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && !flagprev2 && flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)
			fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		}

		if( !flags1 && !flags2 && flags3){
		
			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}

		if(!flagprev1 && !flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		if(!flagprev1 && !flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
	
		if(!flagprev1 && flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		if(!flagprev1 && flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		if(flagprev1 && !flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		if(flagprev1 && !flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		if(flagprev1 && flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		if(flagprev1 && flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
		}


		if( !flags1 && flags2 && !flags3){
		
		if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}

			if(!flagprev1 && !flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && !flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)	
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		}

		if( !flags1 && flags2 && flags3){

			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}
			if(!flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
	
			if(!flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],l%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%d[%d:%d],r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],l%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%d[%d:%d],r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		}

		if( flags1 && !flags2 && !flags3){
			
			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}

			if(!flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
	
			if(!flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%d[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
		}

		if( flags1 && !flags2 && flags3){

			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}

			if(!flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%d[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%d[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		}

		if( flags1 && flags2 && !flags3){

			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%d;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%d <= l%d%d;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}

			if(!flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%dx[%d:%d],r%d%d[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);
		}
				
		if( flags1 && flags2 && flags3){

			if( minstage % kstage ==0 && stage % kstage !=0 && minstage != 0){
			
				if( i1.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i1.stage,i1.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i1.stage,i1.num,i1.stage,i1.num);
				fprintf(filePointer,"end\n\n");
				flagprev1 = 1;
				}

				if(i2.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i2.stage,i2.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i2.stage,i2.num,i2.stage,i2.num);
				fprintf(filePointer,"end\n\n");
				flagprev2 = 1;
				}
				if(i3.stage == minstage){
				fprintf(filePointer,"\nreg [%d:0]r%d%dx;\n",2*n-1,i3.stage,i3.num);
				fprintf(filePointer,"always @(negedge clk)\n begin\n");
				fprintf(filePointer,"r%d%dx <= l%d%dx;\n",i3.stage,i3.num,i3.stage,i3.num);
				fprintf(filePointer,"end\n\n");
				flagprev3 = 1;
				}

			}
			if(!flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],l%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(!flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(l%d%dx[%d:%d],r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && !flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],l%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && !flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

			if(flagprev1 && flagprev2 && flagprev3)
		fprintf(filePointer,"CSA%dbits c%d(r%d%dx[%d:%d],r%d%dx[%d:%d],r%d%dx[%d:%d],l%d%d,l%d%d);\n",bitcsa,CSAnum,i1.stage,i1.num,2*n-1,i1.shift,
			i2.stage,i2.num,2*n-1,i2.shift,i3.stage,i3.num,2*n-1,i3.shift,o1.stage,o1.num,o2.stage,o2.num);

		}


		}
		// end of the else case here..

		// CSAnum stores the unique id that is given to each instance of CSA module. Increment it after we instantiate the CSA module once.  
		CSAnum++;
	}	// end of the while loop


	// once while loop is finished we are left the two item in the queue. This two items are then given to CLA and its addition gices the actual multiplication result.
	// empty the last two elements from the queue and give it to CLA...
	item feedcla1 = q1.front(); 
	q1.pop();
	item feedcla2 = q1.front(); 
	q1.pop();

	// check if we need to add the register stage before the final CLA addition based on the kstage value. If yes then add register stage and then do CLA addition. 
	if(feedcla1.stage % kstage == 0 && feedcla2.stage % kstage == 0 ){

	fprintf( filePointer, "reg [%d:0]r%d%d;\n",2*n-1,feedcla1.stage,feedcla1.num );
	fprintf( filePointer, "reg [%d:0]r%d%d;\n",2*n-1,feedcla2.stage,feedcla2.num );
	fprintf( filePointer, "always @(negedge clk)\nbegin\n" );
	fprintf( filePointer, "r%d%d <= l%d%d;\n",feedcla1.stage,feedcla1.num,feedcla1.stage,feedcla1.num );
	fprintf( filePointer, "r%d%d <= l%d%d;\n",feedcla2.stage,feedcla2.num,feedcla2.stage,feedcla2.num );
	fprintf( filePointer, "end\n\n" );
	fprintf(filePointer,"CLA r1(r%d%d,r%d%d,cout,Result);",feedcla1.stage,feedcla1.num,feedcla2.stage,feedcla2.num);	
	}
	// else just do the CLA addition
	else
	fprintf(filePointer,"CLA r1(l%d%d,l%d%d,cout,Result);",feedcla1.stage,feedcla1.num,feedcla2.stage,feedcla2.num);	

	// end the module..
	fprintf(filePointer,"\nendmodule\n");
	// code to generate the CSA stages + CLA at the end, ends here....

	return 0;
}
