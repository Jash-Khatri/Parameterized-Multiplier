#include<iostream>
#include<bits/stdc++.h>
#include<math.h>
using namespace std;

int main(){

	int n;
	cin >> n;    // input bit for multiplicand and multiplier

 	FILE *filePointer;
  	filePointer = fopen("PP.v", "w") ; 

  if( filePointer == NULL ) 
    { 
        printf( "file failed to open." ) ; 
    }

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

	fprintf(filePointer,"module fulladder(a,b,cin,cout,sum); \n");
	fprintf(filePointer,"input a,b,cin; \n");
	fprintf(filePointer,"output cout,sum; \n");
	fprintf(filePointer,"assign #6 cout= (b&cin | a&b | cin&a );\n");
	fprintf(filePointer,"assign #6 sum=a^b^cin; \nendmodule \n");
  
  	fprintf(filePointer,"\n\n");


  	// generating 2n bit CSA for the unoptimized version. It can generate 2n+1 bit output in worst case. 
  	fprintf(filePointer,"module CSA(p1,p2,p3,sum,carry);\n\n");
  	fprintf(filePointer,"input [%d:0]p1,p2,p3;\n",2*n-1);
  	fprintf(filePointer,"output [%d:0]sum,carry;\n",2*n);
  	fprintf(filePointer,"assign carry[0] = 1'b0;\n");

  	for(int i=0;i<2*n;i++){
  		fprintf(filePointer,"fulladder f%d(p1[%d],p2[%d],p3[%d],carry[%d],sum[%d]);\n",i+1,i,i,i,i+1,i);
  	}

  	fprintf(filePointer,"\n");
  	fprintf(filePointer,"endmodule\n\n"); 

	fprintf(filePointer,"module CSAStages(A,B,Result,cout);\n\n");  	 

	fprintf(filePointer,"input [%d:0]A,B;\n",n-1);

	fprintf(filePointer,"wire [%d:0]",2*n-2);
	for(int i=0;i<n;i++){
		if(i == 0)
		fprintf(filePointer,"l0%d", i+1);
		else
		fprintf(filePointer,",l0%d", i+1);
	}
	fprintf(filePointer,";\n\n");

	fprintf(filePointer,"output [%d:0]Result;\n",2*n-1);	
	fprintf(filePointer,"output cout;\n");

	fprintf(filePointer,"\nPP p1(A,B,");	
	for(int i=0;i<n;i++){
		if(i == 0)
		fprintf(filePointer,"l0%d", i+1);
		else
		fprintf(filePointer,",l0%d", i+1);
	}
	fprintf(filePointer,");\n");	

	int val = n;
	int numofremain;
	int numofremainprev;
	int numofCSAs;
	int numofCSAsprev;
	int CSAnum = 1;
	int stage = 0;
	int flag = 0;

	numofremainprev = 0;
	numofremain = val % 3;
	numofCSAs = val / 3;
	numofCSAsprev = 0;

	while( val > 2 || numofremain > 0 || numofremainprev > 0 ){
	//level 1
		cout << val << numofremainprev << numofremain << "\n";
	// first finish of the previous remain ones
	flag = 0;
	val = val - numofremain;

	if( (numofremain + numofremainprev) % 3 == 0){
		val -= 3;
		fprintf(filePointer,"wire [%d:0]l%d1,l%d2;\n",2*n-1,stage+1,stage+1);
		if(numofremainprev == 1)
		fprintf(filePointer,"CSA c%d(l%d%d,l%d%d,l%d%d,l%d%d,l%d%d);\n",CSAnum,stage,1,stage,2,stage-1,numofCSAsprev*3+1,stage+1,1,stage+1,2);
		if(numofremainprev == 2)
		fprintf(filePointer,"CSA c%d(l%d%d,l%d%d,l%d%d,l%d%d,l%d%d);\n",CSAnum,stage,1,stage-1,numofCSAsprev*3+1,stage-1,numofCSAsprev*3+2,stage+1,1,stage+1,2);
		CSAnum++;
		val += 2;

		numofremainprev = 0;
		numofremain = 0;
		flag = 1;
	}

	if(!flag){
	for(int i=0;i<numofCSAs;i++){
		val -= 3;
		fprintf(filePointer,"wire [%d:0]l%d%d,l%d%d;\n",2*n-1,stage+1,2*i+1,stage+1,2*i+2);
		fprintf(filePointer,"CSA c%d(l%d%d,l%d%d,l%d%d,l%d%d,l%d%d);\n",CSAnum,stage,3*i+1,stage,3*i+2,stage,3*i+3,stage+1,2*i+1,stage+1,2*i+2);
		CSAnum++;
		val += 2;
		}
	}
	else{
		for(int i=1;i<numofCSAs+1;i++){
		val -= 3;
		fprintf(filePointer,"wire [%d:0]l%d%d,l%d%d;\n",2*n-1,stage+1,2*i+1,stage+1,2*i+2);
		fprintf(filePointer,"CSA c%d(l%d%d,l%d%d,l%d%d,l%d%d,l%d%d);\n",CSAnum,stage,3*i+1,stage,3*i+2,stage,3*i+3,stage+1,2*i+1,stage+1,2*i+2);
		CSAnum++;
		val += 2;
		}
	}

	numofremainprev = numofremain;
	numofremain = val % 3;
	numofCSAsprev = numofCSAs;
	numofCSAs = val / 3; 
	stage++;

	}

	fprintf(filePointer,"\nendmodule\n");
	return 0;
}
