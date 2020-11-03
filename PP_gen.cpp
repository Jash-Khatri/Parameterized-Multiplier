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


	fprintf(filePointer,"endmodule\n");
	return 0;
}