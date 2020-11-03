
module fulladder(a,b,cin,cout,sum); 
input a,b,cin; 
output cout,sum; 
assign #6 cout= (b&cin | a&b | cin&a );
assign #6 sum=a^b^cin; 

endmodule 

module PartialProducts

endmodule

module CSA6bit(X,Y,Z,S,C)
wire [5:0]X,Y,Z;
wire [5:0]S,C;
C[0] = b'0;
fulladder f0(X[0],Y[0],Z[0],C[1],S[0] );
fulladder f1(X[1],Y[1],Z[1],C[2],S[1] );
fulladder f2(X[2],Y[2],Z[2],C[3],S[2] );
fulladder f4(X[3],Y[3],Z[3],C[4],S[3] );
fulladder f5(X[4],Y[4],Z[4],C[5],S[4] );
fulladder f6(X[5],Y[5],Z[5],b'0,S[5] );

endmodule

module CSA7bit(X,Y,Z,S,C)
wire [6:0]X,Y,Z;
wire [6:0]S,C;
C[0] = b'0;
fulladder f0(X[0],Y[0],Z[0],C[1],S[0] );
fulladder f1(X[1],Y[1],Z[1],C[2],S[1] );
fulladder f2(X[2],Y[2],Z[2],C[3],S[2] );
fulladder f4(X[3],Y[3],Z[3],C[4],S[3] );
fulladder f5(X[4],Y[4],Z[4],C[5],S[4] );
fulladder f6(X[5],Y[5],Z[5],C[6],S[5] );
fulladder f7(X[6],Y[6],Z[6],b'0,S[6] );

endmodule
