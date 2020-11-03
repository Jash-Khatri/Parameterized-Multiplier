`include "CLA7bits.v"

module PP(X,Y,p1,p2,p3,p4);

input [3:0]X,Y;
output [6:0]p1,p2,p3,p4; 

assign #2 p1[0] = X[0] & Y[0];
assign #2 p1[1] = X[1] & Y[0];
assign #2 p1[2] = X[2] & Y[0];
assign #2 p1[3] = X[3] & Y[0];
assign #2 p1[4] = 1'b0;
assign #2 p1[5] = 1'b0;
assign #2 p1[6] = 1'b0;


assign #2 p2[0] = 1'b0;
assign #2 p2[1] = X[0] & Y[1];
assign #2 p2[2] = X[1] & Y[1];
assign #2 p2[3] = X[2] & Y[1];
assign #2 p2[4] = X[3] & Y[1];
assign #2 p2[5] = 1'b0;
assign #2 p2[6] = 1'b0;

assign #2 p3[0] = 1'b0;
assign #2 p3[1] = 1'b0;
assign #2 p3[2] = X[0] & Y[2];
assign #2 p3[3] = X[1] & Y[2];
assign #2 p3[4] = X[2] & Y[2];
assign #2 p3[5] = X[3] & Y[2];
assign #2 p3[6] = 1'b0;


assign #2 p4[0] = 1'b0;
assign #2 p4[1] = 1'b0;
assign #2 p4[2] = 1'b0;
assign #2 p4[3] = X[0] & Y[3];
assign #2 p4[4] = X[1] & Y[3];
assign #2 p4[5] = X[2] & Y[3];
assign #2 p4[6] = X[3] & Y[3];

endmodule 

module fulladder(a,b,cin,cout,sum); 
input a,b,cin; 
output cout,sum; 
assign #3 cout= (b&cin | a&b | cin&a );
assign #6 sum=a^b^cin; 
endmodule 


module CSA6(p1,p2,p3,sum,carry);

input [5:0]p1,p2,p3;
output [5:0]sum,carry;
wire extra;

assign carry[0] = 1'b0;
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],extra,sum[5]);

endmodule

module CSA7(p1,p2,p3,sum,carry);

input [6:0]p1,p2,p3;
output [6:0]sum,carry;
wire extra;

assign carry[0] = 1'b0;
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],extra,sum[6]);

endmodule


module CSAStages(A,B,Result,cout);

input [3:0]A,B;
wire [6:0]o1,o2,o3,o4;
output [7:0]Result;
output cout;

PP p1(A,B,o1,o2,o3,o4);

wire [5:0]l00,l01;
wire [6:0]l10,l11;
CSA6 c1(o1,o2,o3,l00,l01);
CSA7 c2(o4,l01,l00,l10,l11);

CLA r1(l10,l11,cout,Result);

endmodule



