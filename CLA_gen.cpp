/***
Author : Jash Khatri (CS19S018)
Time of creation : 5 October 2020 at 15:26 IST
Code for : automatic generation of the N-bit Carry Look-ahead Adder.
***/


#include<iostream>
#include<bits/stdc++.h>
#include <math.h>
using namespace std;

int main(){
	
// below lines reads the input from the user. Based on the value of n entered by user the code will generate n-bit CLA
  int n;
	cin >>n;

// Output will be written to the CLA.v file which contains the code for generated N-bit CLA. 
  FILE *filePointer;
  filePointer = fopen("CLA.v", "w") ; 

  if( filePointer == NULL ) 
    { 
        printf( "file failed to open." ) ; 
    }

  //Below line generates the code to implement the circuit to obtain the propagate,generate,kill terms from the input bits.
  fprintf(filePointer,"module pandgterms(a,b,st1,st0);\n\ninput a,b; \noutput st1,st0; \nassign #2 st1=a&b; \nassign #2 st0=a|b; \n\nendmodule\n\n");
  
  //Below line generates the code to implement the Star operator which is an semi-group operator and operates on the propagate,generate,kill terms.
  fprintf(filePointer,"module starop(cs1,cs0,ps1,ps0,o1,o0); \ninput cs1,cs0,ps1,ps0; \noutput o1,o0; \n\nassign #4 o1 = cs1 | (cs0 & ps1); \nassign #4 o0 = cs1 | (cs0 & ps0); \n\nendmodule\n\n");
  
  //Below line generates the code to implement simple module which implements the 3 input XOR gate.
  fprintf(filePointer,"module bitXor(a,b,c,out); \ninput a,b,c; \noutput out; \n\nassign #6 out = a^b^c; \n\nendmodule\n\n");


  // code to generate the N-bit CLA module.
  fprintf(filePointer,"module CLA(X,Y,Co,S);\n");

//Code that generates the verilog code that defines input output and some wire which are used later. 
fprintf(filePointer,"input [%d:0]X,Y; \noutput [%d:0]S; \noutput Co; \nwire [%d:0]st1; \nwire [%d:0]st0;\n\n",n-1,n-1,n-1,n-1);

// Code to generate the propagate,generate,kill terms. This terms will be then feeded to the First level of the Star operators.
for(int i=0;i<n;i++){
  fprintf(filePointer,"pandgterms a%d(X[%d],Y[%d],st1[%d],st0[%d] );\n",i,i,i,i,i);
}

// Code to generate the First level of the Star operators in CLA circuit. 

fprintf(filePointer,"\nwire [%d:0]l01; \nwire [%d:0]l00; \n\nstarop b0(st1[0],st0[0],1'b0,1'b0,l01[0],l00[0]);\n",n-1,n-1);

for(int i=1;i<n-1;i++){
fprintf(filePointer,"starop b%d(st1[%d],st0[%d],st1[%d],st0[%d],l01[%d],l00[%d]);\n",i,i,i,i-1,i-1,i,i);
}

fprintf(filePointer,"starop b%d(st1[%d],st0[%d],st1[%d],st0[%d],l01[%d],l00[%d]);\n\n",n-1,n-1,n-1,n-2,n-2,n-1,n-1);

// Code to generate the rest of the levels of the Star operators in CLA circuit.
int x ;
int stop;
int k = 1;
int level = 1;

while(true){
x = (pow(2,k)-1);
stop = n - (pow(2,k)-1);
if(stop<=0)
  break;
fprintf(filePointer,"\n");
fprintf(filePointer,"wire [%d:0]l%d1; \nwire [%d:0]l%d0;\n\n",n-1,level,n-1,level);
for(int i=0;i<x;i++){
  fprintf(filePointer,"assign l%d1[%d] = l%d1[%d]; \n",level,i,level-1,i );
  fprintf(filePointer,"assign l%d0[%d] = l%d0[%d]; \n",level,i,level-1,i );
}

  int j=0;
    fprintf( filePointer,"starop c%d0(l%d1[%d],l%d0[%d],1'b0,1'b0,l%d1[%d],l%d0[%d]);\n",level,level-1,x,level-1,x,level,x,level,x );
  
  for(int i=x+1;i<n;i++){
    j++;
   fprintf( filePointer,"starop c%d%d(l%d1[%d],l%d0[%d],l%d1[%d],l%d0[%d],l%d1[%d],l%d0[%d]);\n",level,j,level-1,i,level-1,i, level-1, j-1, level-1,j-1 ,level,i,level,i );
  }

k++;
level++;
}

// Code to generate the rest of the levels of the Star operators in CLA circuit ends.


//Code to generate the XORs that takes bit Xi, Yi with the generated carry bits l(log_2 N)1[i] as inputs to get the sum bits S[i] for all i.
fprintf(filePointer,"\n");
fprintf(filePointer,"bitXor x0( X[0], Y[0], 1'b0, S[0] );\n");

for(int i=1;i<n;i++)
{
fprintf(filePointer,"bitXor x%d( X[%d], Y[%d], l%d1[%d], S[%d] );\n",i,i,i,level-1,i-1,i);
}

//Generates code which assign l(log_2 N)1[N-1] to the Cout that is carry out obtained after summing X and Y.
fprintf(filePointer,"assign Co = l%d1[%d];\n\n",level-1,n-1);

fprintf(filePointer,"endmodule\n");

fclose(filePointer);

return 0;
}