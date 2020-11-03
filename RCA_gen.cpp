/***
Author : Jash Khatri (CS19S018)
Time of creation : 5 October 2020 at 15:26 IST
Code for : automatic generation of the N-bit Carry Ripple Adder.
***/


#include<iostream>
#include<bits/stdc++.h>
using namespace std;

int main(){


// below lines reads the input from the user. Based on the value of n entered by user the code will generate n-bit CRA
	int n;
	cin >>n;

// Output will be written to the RCA.v file which contains the code for generated N-bit Ripple Carry Adder. 
FILE *filePointer;
filePointer = fopen("RCA.v", "w") ; 

if( filePointer == NULL ) 
    { 
        printf( "file failed to open." ) ; 
    }

// This part of code generates the module that implements the fullAdder circuit where a,b,cin are 3 inputs to the full adder and cout and sum are the outputs from the full adder.
	fprintf(filePointer,"module fulladder(a,b,cin,cout,sum); \n");
	fprintf(filePointer,"input a,b,cin; \n");
	fprintf(filePointer,"output cout,sum; \n");
	fprintf(filePointer,"assign #6 cout= (b&cin | a&b | cin&a );\n");
	fprintf(filePointer,"assign #6 sum=a^b^cin; \nendmodule \n");
  
  fprintf(filePointer,"\n\n");

//Below line generates the verilog code for 16 bit RCA module
  	fprintf(filePointer,"//defining the %d bit RCA\n",n);

  	fprintf(filePointer,"module RCA(X,Y,Co,S);\n");
  	fprintf(filePointer,"input [%d:0]X,Y;\n",n-1);
  	fprintf(filePointer,"output [%d:0]S;\n",n-1);
  	fprintf(filePointer,"output Co;\n");

// code that generates several instances of fullAdders. 
// Each of the fullAdder module below takes X[i], Y[i] and wi (carry from the previous stage) as input and generates S[i] (sum bit) and w(i+1) (carry out)
 fprintf(filePointer,"\n");
  	fprintf(filePointer,"fulladder f1(X[0],Y[0],1'b0,w1,S[0]);\n");

  	for(int i=2;i<n;i++){
  	fprintf(filePointer,"fulladder f%d(X[%d],Y[%d],w%d,w%d,S[%d]);\n",i,i-1,i-1,i-1,i,i-1);
  	}

  	fprintf(filePointer,"fulladder f%d(X[%d],Y[%d],w%d,Co,S[%d]);\n",n,n-1,n-1,n-1,n-1);

  	fprintf(filePointer,"endmodule\n");

    fclose(filePointer);

return 0;
}