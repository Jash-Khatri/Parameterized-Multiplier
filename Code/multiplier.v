/***
Author : Jash Khatri (CS19S018)
Time of creation : 14 November 2020 at 15:26 IST
Chip for : parameterized and pipelined wallace tree multiplier with N=8 and K=2
***/

/**
* Chip Name : CSAStages

* Inputs   : A,B, clk(clock)

* Outputs  : cout, Result

* Function : {cout, Result} = A*B
*/
// All the delays are calculated assuming that all the gates are implemented using NAND gates and invertors


//This module implements the circuit to obtain the propagate,generate,kill terms from the input bits.
module pandgterms(a,b,st1,st0);

input a,b; 
output st1,st0; 
assign #2 st1=a&b; 
assign #2 st0=a|b; 

endmodule

//This module implements the Star operator which is an semi-group operator and operates on the propagate,generate,kill terms.
module starop(cs1,cs0,ps1,ps0,o1,o0); 
input cs1,cs0,ps1,ps0; 
output o1,o0; 

assign #4 o1 = cs1 | (cs0 & ps1); 
assign #4 o0 = cs1 | (cs0 & ps0); 

endmodule

//This is a simple module which implements the 3 input XOR gate.
module bitXor(a,b,c,out); 
input a,b,c; 
output out; 

assign #6 out = a^b^c; 

endmodule

//This is the module which implements the 16-bit carry look-ahead adder.
module CLA(X,Y,Co,S);
input [15:0]X,Y; 
output [16:0]S; 
output Co; 
wire [15:0]st1; 
wire [15:0]st0;

//Below given lines are used to obtain the propagate,generate,kill terms. This terms will be then feeded to the First level of the Star operators.
pandgterms a0(X[0],Y[0],st1[0],st0[0] );
pandgterms a1(X[1],Y[1],st1[1],st0[1] );
pandgterms a2(X[2],Y[2],st1[2],st0[2] );
pandgterms a3(X[3],Y[3],st1[3],st0[3] );
pandgterms a4(X[4],Y[4],st1[4],st0[4] );
pandgterms a5(X[5],Y[5],st1[5],st0[5] );
pandgterms a6(X[6],Y[6],st1[6],st0[6] );
pandgterms a7(X[7],Y[7],st1[7],st0[7] );
pandgterms a8(X[8],Y[8],st1[8],st0[8] );
pandgterms a9(X[9],Y[9],st1[9],st0[9] );
pandgterms a10(X[10],Y[10],st1[10],st0[10] );
pandgterms a11(X[11],Y[11],st1[11],st0[11] );
pandgterms a12(X[12],Y[12],st1[12],st0[12] );
pandgterms a13(X[13],Y[13],st1[13],st0[13] );
pandgterms a14(X[14],Y[14],st1[14],st0[14] );
pandgterms a15(X[15],Y[15],st1[15],st0[15] );

wire [15:0]l01; 
wire [15:0]l00; 

// First level of the Star operators in CLA circuit. 
starop b0(st1[0],st0[0],1'b0,1'b0,l01[0],l00[0]);
starop b1(st1[1],st0[1],st1[0],st0[0],l01[1],l00[1]);
starop b2(st1[2],st0[2],st1[1],st0[1],l01[2],l00[2]);
starop b3(st1[3],st0[3],st1[2],st0[2],l01[3],l00[3]);
starop b4(st1[4],st0[4],st1[3],st0[3],l01[4],l00[4]);
starop b5(st1[5],st0[5],st1[4],st0[4],l01[5],l00[5]);
starop b6(st1[6],st0[6],st1[5],st0[5],l01[6],l00[6]);
starop b7(st1[7],st0[7],st1[6],st0[6],l01[7],l00[7]);
starop b8(st1[8],st0[8],st1[7],st0[7],l01[8],l00[8]);
starop b9(st1[9],st0[9],st1[8],st0[8],l01[9],l00[9]);
starop b10(st1[10],st0[10],st1[9],st0[9],l01[10],l00[10]);
starop b11(st1[11],st0[11],st1[10],st0[10],l01[11],l00[11]);
starop b12(st1[12],st0[12],st1[11],st0[11],l01[12],l00[12]);
starop b13(st1[13],st0[13],st1[12],st0[12],l01[13],l00[13]);
starop b14(st1[14],st0[14],st1[13],st0[13],l01[14],l00[14]);
starop b15(st1[15],st0[15],st1[14],st0[14],l01[15],l00[15]);


wire [15:0]l11; 
wire [15:0]l10;

// Second level of the Star operators in CLA circuit. 
assign l11[0] = l01[0]; 
assign l10[0] = l00[0]; 
starop c10(l01[1],l00[1],1'b0,1'b0,l11[1],l10[1]);
starop c11(l01[2],l00[2],l01[0],l00[0],l11[2],l10[2]);
starop c12(l01[3],l00[3],l01[1],l00[1],l11[3],l10[3]);
starop c13(l01[4],l00[4],l01[2],l00[2],l11[4],l10[4]);
starop c14(l01[5],l00[5],l01[3],l00[3],l11[5],l10[5]);
starop c15(l01[6],l00[6],l01[4],l00[4],l11[6],l10[6]);
starop c16(l01[7],l00[7],l01[5],l00[5],l11[7],l10[7]);
starop c17(l01[8],l00[8],l01[6],l00[6],l11[8],l10[8]);
starop c18(l01[9],l00[9],l01[7],l00[7],l11[9],l10[9]);
starop c19(l01[10],l00[10],l01[8],l00[8],l11[10],l10[10]);
starop c110(l01[11],l00[11],l01[9],l00[9],l11[11],l10[11]);
starop c111(l01[12],l00[12],l01[10],l00[10],l11[12],l10[12]);
starop c112(l01[13],l00[13],l01[11],l00[11],l11[13],l10[13]);
starop c113(l01[14],l00[14],l01[12],l00[12],l11[14],l10[14]);
starop c114(l01[15],l00[15],l01[13],l00[13],l11[15],l10[15]);

wire [15:0]l21; 
wire [15:0]l20;

// Third level of the Star operators in CLA circuit. 
assign l21[0] = l11[0]; 
assign l20[0] = l10[0]; 
assign l21[1] = l11[1]; 
assign l20[1] = l10[1]; 
assign l21[2] = l11[2]; 
assign l20[2] = l10[2]; 
starop c20(l11[3],l10[3],1'b0,1'b0,l21[3],l20[3]);
starop c21(l11[4],l10[4],l11[0],l10[0],l21[4],l20[4]);
starop c22(l11[5],l10[5],l11[1],l10[1],l21[5],l20[5]);
starop c23(l11[6],l10[6],l11[2],l10[2],l21[6],l20[6]);
starop c24(l11[7],l10[7],l11[3],l10[3],l21[7],l20[7]);
starop c25(l11[8],l10[8],l11[4],l10[4],l21[8],l20[8]);
starop c26(l11[9],l10[9],l11[5],l10[5],l21[9],l20[9]);
starop c27(l11[10],l10[10],l11[6],l10[6],l21[10],l20[10]);
starop c28(l11[11],l10[11],l11[7],l10[7],l21[11],l20[11]);
starop c29(l11[12],l10[12],l11[8],l10[8],l21[12],l20[12]);
starop c210(l11[13],l10[13],l11[9],l10[9],l21[13],l20[13]);
starop c211(l11[14],l10[14],l11[10],l10[10],l21[14],l20[14]);
starop c212(l11[15],l10[15],l11[11],l10[11],l21[15],l20[15]);

wire [15:0]l31; 
wire [15:0]l30;

// Fourth level of the Star operators in CLA circuit. 
assign l31[0] = l21[0]; 
assign l30[0] = l20[0]; 
assign l31[1] = l21[1]; 
assign l30[1] = l20[1]; 
assign l31[2] = l21[2]; 
assign l30[2] = l20[2]; 
assign l31[3] = l21[3]; 
assign l30[3] = l20[3]; 
assign l31[4] = l21[4]; 
assign l30[4] = l20[4]; 
assign l31[5] = l21[5]; 
assign l30[5] = l20[5]; 
assign l31[6] = l21[6]; 
assign l30[6] = l20[6]; 
starop c30(l21[7],l20[7],1'b0,1'b0,l31[7],l30[7]);
starop c31(l21[8],l20[8],l21[0],l20[0],l31[8],l30[8]);
starop c32(l21[9],l20[9],l21[1],l20[1],l31[9],l30[9]);
starop c33(l21[10],l20[10],l21[2],l20[2],l31[10],l30[10]);
starop c34(l21[11],l20[11],l21[3],l20[3],l31[11],l30[11]);
starop c35(l21[12],l20[12],l21[4],l20[4],l31[12],l30[12]);
starop c36(l21[13],l20[13],l21[5],l20[5],l31[13],l30[13]);
starop c37(l21[14],l20[14],l21[6],l20[6],l31[14],l30[14]);
starop c38(l21[15],l20[15],l21[7],l20[7],l31[15],l30[15]);

wire [15:0]l41; 
wire [15:0]l40;

// Fifth level of the Star operators in CLA circuit. This is final level hence l41[i] will give the generated carry bits for the CLA.
assign l41[0] = l31[0]; 
assign l40[0] = l30[0]; 
assign l41[1] = l31[1]; 
assign l40[1] = l30[1]; 
assign l41[2] = l31[2]; 
assign l40[2] = l30[2]; 
assign l41[3] = l31[3]; 
assign l40[3] = l30[3]; 
assign l41[4] = l31[4]; 
assign l40[4] = l30[4]; 
assign l41[5] = l31[5]; 
assign l40[5] = l30[5]; 
assign l41[6] = l31[6]; 
assign l40[6] = l30[6]; 
assign l41[7] = l31[7]; 
assign l40[7] = l30[7]; 
assign l41[8] = l31[8]; 
assign l40[8] = l30[8]; 
assign l41[9] = l31[9]; 
assign l40[9] = l30[9]; 
assign l41[10] = l31[10]; 
assign l40[10] = l30[10]; 
assign l41[11] = l31[11]; 
assign l40[11] = l30[11]; 
assign l41[12] = l31[12]; 
assign l40[12] = l30[12]; 
assign l41[13] = l31[13]; 
assign l40[13] = l30[13]; 
assign l41[14] = l31[14]; 
assign l40[14] = l30[14]; 
starop c40(l31[15],l30[15],1'b0,1'b0,l41[15],l40[15]);

// Finally XOR the input bit Xi, Yi with the generated carry bits l41[i] to get the sum bits S[i] for all i.
bitXor x0( X[0], Y[0], 1'b0, S[0] );
bitXor x1( X[1], Y[1], l41[0], S[1] );
bitXor x2( X[2], Y[2], l41[1], S[2] );
bitXor x3( X[3], Y[3], l41[2], S[3] );
bitXor x4( X[4], Y[4], l41[3], S[4] );
bitXor x5( X[5], Y[5], l41[4], S[5] );
bitXor x6( X[6], Y[6], l41[5], S[6] );
bitXor x7( X[7], Y[7], l41[6], S[7] );
bitXor x8( X[8], Y[8], l41[7], S[8] );
bitXor x9( X[9], Y[9], l41[8], S[9] );
bitXor x10( X[10], Y[10], l41[9], S[10] );
bitXor x11( X[11], Y[11], l41[10], S[11] );
bitXor x12( X[12], Y[12], l41[11], S[12] );
bitXor x13( X[13], Y[13], l41[12], S[13] );
bitXor x14( X[14], Y[14], l41[13], S[14] );
bitXor x15( X[15], Y[15], l41[14], S[15] );
assign S[16] = l41[15];

// l41[N-1] is the Cout that is carry out obtained after summing X and Y.
assign Co = l41[15];

endmodule


// This is the module which generates the Partial products p1...pn. It takes X and Y as input which are multiplicand and multiplier respectively.
module PP(X,Y,p1,p2,p3,p4,p5,p6,p7,p8);

input [7:0]X,Y; 
output [14:0]p1,p2,p3,p4,p5,p6,p7,p8;

// generate p1.
assign #2 p1[0] = X[0] & Y[0];
assign #2 p1[1] = X[1] & Y[0];
assign #2 p1[2] = X[2] & Y[0];
assign #2 p1[3] = X[3] & Y[0];
assign #2 p1[4] = X[4] & Y[0];
assign #2 p1[5] = X[5] & Y[0];
assign #2 p1[6] = X[6] & Y[0];
assign #2 p1[7] = X[7] & Y[0];
assign #2 p1[8] = 1'b0;
assign #2 p1[9] = 1'b0;
assign #2 p1[10] = 1'b0;
assign #2 p1[11] = 1'b0;
assign #2 p1[12] = 1'b0;
assign #2 p1[13] = 1'b0;
assign #2 p1[14] = 1'b0;

// generate p2.
assign #2 p2[0] = 1'b0;
assign #2 p2[1] = X[0] & Y[1];
assign #2 p2[2] = X[1] & Y[1];
assign #2 p2[3] = X[2] & Y[1];
assign #2 p2[4] = X[3] & Y[1];
assign #2 p2[5] = X[4] & Y[1];
assign #2 p2[6] = X[5] & Y[1];
assign #2 p2[7] = X[6] & Y[1];
assign #2 p2[8] = X[7] & Y[1];
assign #2 p2[9] = 1'b0;
assign #2 p2[10] = 1'b0;
assign #2 p2[11] = 1'b0;
assign #2 p2[12] = 1'b0;
assign #2 p2[13] = 1'b0;
assign #2 p2[14] = 1'b0;

// generate p3.
assign #2 p3[0] = 1'b0;
assign #2 p3[1] = 1'b0;
assign #2 p3[2] = X[0] & Y[2];
assign #2 p3[3] = X[1] & Y[2];
assign #2 p3[4] = X[2] & Y[2];
assign #2 p3[5] = X[3] & Y[2];
assign #2 p3[6] = X[4] & Y[2];
assign #2 p3[7] = X[5] & Y[2];
assign #2 p3[8] = X[6] & Y[2];
assign #2 p3[9] = X[7] & Y[2];
assign #2 p3[10] = 1'b0;
assign #2 p3[11] = 1'b0;
assign #2 p3[12] = 1'b0;
assign #2 p3[13] = 1'b0;
assign #2 p3[14] = 1'b0;

// generate p4.
assign #2 p4[0] = 1'b0;
assign #2 p4[1] = 1'b0;
assign #2 p4[2] = 1'b0;
assign #2 p4[3] = X[0] & Y[3];
assign #2 p4[4] = X[1] & Y[3];
assign #2 p4[5] = X[2] & Y[3];
assign #2 p4[6] = X[3] & Y[3];
assign #2 p4[7] = X[4] & Y[3];
assign #2 p4[8] = X[5] & Y[3];
assign #2 p4[9] = X[6] & Y[3];
assign #2 p4[10] = X[7] & Y[3];
assign #2 p4[11] = 1'b0;
assign #2 p4[12] = 1'b0;
assign #2 p4[13] = 1'b0;
assign #2 p4[14] = 1'b0;

// generate p5.
assign #2 p5[0] = 1'b0;
assign #2 p5[1] = 1'b0;
assign #2 p5[2] = 1'b0;
assign #2 p5[3] = 1'b0;
assign #2 p5[4] = X[0] & Y[4];
assign #2 p5[5] = X[1] & Y[4];
assign #2 p5[6] = X[2] & Y[4];
assign #2 p5[7] = X[3] & Y[4];
assign #2 p5[8] = X[4] & Y[4];
assign #2 p5[9] = X[5] & Y[4];
assign #2 p5[10] = X[6] & Y[4];
assign #2 p5[11] = X[7] & Y[4];
assign #2 p5[12] = 1'b0;
assign #2 p5[13] = 1'b0;
assign #2 p5[14] = 1'b0;

// generate p6.
assign #2 p6[0] = 1'b0;
assign #2 p6[1] = 1'b0;
assign #2 p6[2] = 1'b0;
assign #2 p6[3] = 1'b0;
assign #2 p6[4] = 1'b0;
assign #2 p6[5] = X[0] & Y[5];
assign #2 p6[6] = X[1] & Y[5];
assign #2 p6[7] = X[2] & Y[5];
assign #2 p6[8] = X[3] & Y[5];
assign #2 p6[9] = X[4] & Y[5];
assign #2 p6[10] = X[5] & Y[5];
assign #2 p6[11] = X[6] & Y[5];
assign #2 p6[12] = X[7] & Y[5];
assign #2 p6[13] = 1'b0;
assign #2 p6[14] = 1'b0;

// generate p7.
assign #2 p7[0] = 1'b0;
assign #2 p7[1] = 1'b0;
assign #2 p7[2] = 1'b0;
assign #2 p7[3] = 1'b0;
assign #2 p7[4] = 1'b0;
assign #2 p7[5] = 1'b0;
assign #2 p7[6] = X[0] & Y[6];
assign #2 p7[7] = X[1] & Y[6];
assign #2 p7[8] = X[2] & Y[6];
assign #2 p7[9] = X[3] & Y[6];
assign #2 p7[10] = X[4] & Y[6];
assign #2 p7[11] = X[5] & Y[6];
assign #2 p7[12] = X[6] & Y[6];
assign #2 p7[13] = X[7] & Y[6];
assign #2 p7[14] = 1'b0;

// generate p8.
assign #2 p8[0] = 1'b0;
assign #2 p8[1] = 1'b0;
assign #2 p8[2] = 1'b0;
assign #2 p8[3] = 1'b0;
assign #2 p8[4] = 1'b0;
assign #2 p8[5] = 1'b0;
assign #2 p8[6] = 1'b0;
assign #2 p8[7] = X[0] & Y[7];
assign #2 p8[8] = X[1] & Y[7];
assign #2 p8[9] = X[2] & Y[7];
assign #2 p8[10] = X[3] & Y[7];
assign #2 p8[11] = X[4] & Y[7];
assign #2 p8[12] = X[5] & Y[7];
assign #2 p8[13] = X[6] & Y[7];
assign #2 p8[14] = X[7] & Y[7];


endmodule

// The below module implements the fullAdder circuit where a,b,cin are 3 inputs to the full adder and cout and sum are the outputs from the full adder.
module fulladder(a,b,cin,cout,sum); 
input a,b,cin; 
output cout,sum; 
assign #6 cout= (b&cin | a&b | cin&a );
assign #6 sum=a^b^cin; 
endmodule 

// This module generates the 10 bit CSA which will take three 10-bit input p1,p2,p3 and generates sum and carry.
// note inputs p1,p2,p3 are defined as 16 bits numbers but only first 10 bits are used for CSA addition. 
module CSA10bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the first 10 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);

// set rest of the MSB bits to zero
assign sum[10] = 1'b0;
assign sum[11] = 1'b0;
assign sum[12] = 1'b0;
assign sum[13] = 1'b0;
assign sum[14] = 1'b0;
assign sum[15] = 1'b0;
assign sum[16] = 1'b0;

// set rest of the MSB bits to zero
assign carry[11] = 1'b0;
assign carry[12] = 1'b0;
assign carry[13] = 1'b0;
assign carry[14] = 1'b0;
assign carry[15] = 1'b0;
assign carry[16] = 1'b0;

endmodule

// This module generates the 11 bit CSA which will take three 11-bit input p1,p2,p3 and generates sum and carry.
// note inputs p1,p2,p3 are defined as 16 bits numbers but only first 11 bits are used for CSA addition. 
module CSA11bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the first 11 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);
fulladder f11(p1[10],p2[10],p3[10],carry[11],sum[10]);

// set rest of the MSB bits to zero
assign sum[11] = 1'b0;
assign sum[12] = 1'b0;
assign sum[13] = 1'b0;
assign sum[14] = 1'b0;
assign sum[15] = 1'b0;
assign sum[16] = 1'b0;

// set rest of the MSB bits to zero
assign carry[12] = 1'b0;
assign carry[13] = 1'b0;
assign carry[14] = 1'b0;
assign carry[15] = 1'b0;
assign carry[16] = 1'b0;

endmodule

// This module generates the 12 bit CSA which will take three 12-bit input p1,p2,p3 and generates sum and carry.
// note inputs p1,p2,p3 are defined as 16 bits numbers but only first 12 bits are used for CSA addition. 
module CSA12bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the first 12 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);
fulladder f11(p1[10],p2[10],p3[10],carry[11],sum[10]);
fulladder f12(p1[11],p2[11],p3[11],carry[12],sum[11]);

// set rest of the MSB bits to zero
assign sum[12] = 1'b0;
assign sum[13] = 1'b0;
assign sum[14] = 1'b0;
assign sum[15] = 1'b0;
assign sum[16] = 1'b0;

// set rest of the MSB bits to zero
assign carry[13] = 1'b0;
assign carry[14] = 1'b0;
assign carry[15] = 1'b0;
assign carry[16] = 1'b0;

endmodule

// This module generates the 13 bit CSA which will take three 13-bit input p1,p2,p3 and generates sum and carry.
// note inputs p1,p2,p3 are defined as 16 bits numbers but only first 13 bits are used for CSA addition.
module CSA13bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the first 13 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);
fulladder f11(p1[10],p2[10],p3[10],carry[11],sum[10]);
fulladder f12(p1[11],p2[11],p3[11],carry[12],sum[11]);
fulladder f13(p1[12],p2[12],p3[12],carry[13],sum[12]);

// set rest of the MSB bits to zero
assign sum[13] = 1'b0;
assign sum[14] = 1'b0;
assign sum[15] = 1'b0;
assign sum[16] = 1'b0;

// set rest of the MSB bits to zero
assign carry[14] = 1'b0;
assign carry[15] = 1'b0;
assign carry[16] = 1'b0;

endmodule

// This module generates the 14 bit CSA which will take three 14-bit input p1,p2,p3 and generates sum and carry.
// note inputs p1,p2,p3 are defined as 16 bits numbers but only first 14 bits are used for CSA addition.
module CSA14bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the first 14 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);
fulladder f11(p1[10],p2[10],p3[10],carry[11],sum[10]);
fulladder f12(p1[11],p2[11],p3[11],carry[12],sum[11]);
fulladder f13(p1[12],p2[12],p3[12],carry[13],sum[12]);
fulladder f14(p1[13],p2[13],p3[13],carry[14],sum[13]);

// set rest of the MSB bits to zero
assign sum[14] = 1'b0;
assign sum[15] = 1'b0;
assign sum[16] = 1'b0;

// set rest of the MSB bits to zero
assign carry[15] = 1'b0;
assign carry[16] = 1'b0;

endmodule

// This module generates the 15 bit CSA which will take three 15-bit input p1,p2,p3 and generates sum and carry.
// note inputs p1,p2,p3 are defined as 16 bits numbers but only first 15 bits are used for CSA addition.
module CSA15bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the first 15 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);
fulladder f11(p1[10],p2[10],p3[10],carry[11],sum[10]);
fulladder f12(p1[11],p2[11],p3[11],carry[12],sum[11]);
fulladder f13(p1[12],p2[12],p3[12],carry[13],sum[12]);
fulladder f14(p1[13],p2[13],p3[13],carry[14],sum[13]);
fulladder f15(p1[14],p2[14],p3[14],carry[15],sum[14]);

// set rest of the MSB bits to zero
assign sum[15] = 1'b0;
assign sum[16] = 1'b0;

// set rest of the MSB bits to zero
assign carry[16] = 1'b0;

endmodule

// This module generates the 15 bit CSA which will take three 15-bit input p1,p2,p3 and generates sum and carry.
module CSA16bits(p1,p2,p3,sum,carry);

input [15:0]p1,p2,p3;
output [16:0]sum,carry;
assign carry[0] = 1'b0;

// add the 16 bits and set sum[i] and carry[i] appropriately
fulladder f1(p1[0],p2[0],p3[0],carry[1],sum[0]);
fulladder f2(p1[1],p2[1],p3[1],carry[2],sum[1]);
fulladder f3(p1[2],p2[2],p3[2],carry[3],sum[2]);
fulladder f4(p1[3],p2[3],p3[3],carry[4],sum[3]);
fulladder f5(p1[4],p2[4],p3[4],carry[5],sum[4]);
fulladder f6(p1[5],p2[5],p3[5],carry[6],sum[5]);
fulladder f7(p1[6],p2[6],p3[6],carry[7],sum[6]);
fulladder f8(p1[7],p2[7],p3[7],carry[8],sum[7]);
fulladder f9(p1[8],p2[8],p3[8],carry[9],sum[8]);
fulladder f10(p1[9],p2[9],p3[9],carry[10],sum[9]);
fulladder f11(p1[10],p2[10],p3[10],carry[11],sum[10]);
fulladder f12(p1[11],p2[11],p3[11],carry[12],sum[11]);
fulladder f13(p1[12],p2[12],p3[12],carry[13],sum[12]);
fulladder f14(p1[13],p2[13],p3[13],carry[14],sum[13]);
fulladder f15(p1[14],p2[14],p3[14],carry[15],sum[14]);
fulladder f16(p1[15],p2[15],p3[15],carry[16],sum[15]);

assign sum[16] = 1'b0;


endmodule

// This module left-shifts all the elements in the array 'a' by 3 positions and store the shifted result in array 'b' 
module shifter3bits(a,b);

input [15:0]a;
output [15:0]b;

assign b[12+3] = a[12];
assign b[11+3] = a[11];
assign b[10+3] = a[10];
assign b[9+3] = a[9];
assign b[8+3] = a[8];
assign b[7+3] = a[7];
assign b[6+3] = a[6];
assign b[5+3] = a[5];
assign b[4+3] = a[4];
assign b[3+3] = a[3];
assign b[2+3] = a[2];
assign b[1+3] = a[1];
assign b[0+3] = a[0];
assign b[0] = 1'b0;
assign b[1] = 1'b0;
assign b[2] = 1'b0;

endmodule

// This module left-shifts all the elements in the array 'a' by 6 positions and store the shifted result in array 'b' 
module shifter6bits(a,b);

input [15:0]a;
output [15:0]b;

assign b[9+6] = a[9];
assign b[8+6] = a[8];
assign b[7+6] = a[7];
assign b[6+6] = a[6];
assign b[5+6] = a[5];
assign b[4+6] = a[4];
assign b[3+6] = a[3];
assign b[2+6] = a[2];
assign b[1+6] = a[1];
assign b[0+6] = a[0];
assign b[0] = 1'b0;
assign b[1] = 1'b0;
assign b[2] = 1'b0;
assign b[3] = 1'b0;
assign b[4] = 1'b0;
assign b[5] = 1'b0;

endmodule

// This module generates the CSA tree and add the CLA module at the end i.e. it implements the actual multiplier. 
// This module takes A, B, clk which are multiplicand, multiplier, and clock as input and generate multiplication result and stores it in Result. cout store the carry out from CLA which can be 
//ignored
module CSAStages(A,B,clk,Result,cout);

input [7:0]A,B;
input clk;
wire [14:0]l01,l02,l03,l04,l05,l06,l07,l08;

output [16:0]Result;
output cout;

// generate the partial products from A and B and store in l01..l0n
PP p1(A,B,l01,l02,l03,l04,l05,l06,l07,l08);

// CSA tree starts here..

// first stage of the CSA addition
wire [15:0]l11,l12;
// Note:10 bit CSA are used instead of 16 bits in-order to optimize the area.
CSA10bits c1(l01[15:0],l02[15:0],l03[15:0],l11,l12);
wire [15:0]l13,l14;
// Note:10 bit CSA are used instead of 16 bits in-order to optimize the area.
CSA10bits c2(l04[15:3],l05[15:3],l06[15:3],l13,l14);

// first stage of CSA addition ends

// second stage of the CSA addition
wire [15:0]l25,l26;
CSA15bits c3(l07[15:0],l08[15:0],l11[15:0],l25,l26);
wire [15:0]l13x;
shifter3bits s1(l13,l13x);
wire [15:0]l14x;
shifter3bits s2(l14,l14x);
wire [15:0]l27,l28;
// Note:13 bit CSA are used instead of 16 bits in-order to optimize the area.
CSA13bits c4(l12[15:0],l13x[15:0],l14x[15:0],l27,l28);
wire [15:0]l39,l310;
// second stage of the CSA addition ends..

// add the register stage after second stage as kstage=2 
reg [15:0]r25;
reg [15:0]r26;
reg [15:0]r27;
always @(negedge clk)
 begin
r25 <= l25;
r26 <= l26;
r27 <= l27;
end

// third stage of the CSA addition
CSA15bits c5(r25[15:0],r26[15:0],r27[15:0],l39,l310);
// third stage of CSA addition ends..

wire [15:0]l411,l412;

reg [15:0]r28;
always @(negedge clk)
 begin
r28 <= l28;
end
// register stage after the second stage ends...

// fourth stage of CSA addition
CSA16bits c6(r28[15:0],l39[15:0],l310[15:0],l411,l412);
// fourth stage of CSA addition ends..

//  add the register stage after fourth stage as kstage=2 
reg [15:0]r411;
reg [15:0]r412;
always @(negedge clk)
begin
r411 <= l411;
r412 <= l412;
end
// register stage after the fouth stage ends...
// CSA tree ends..

// add the final two outputs of the CSA tree by using the CLA and store the results in Results and cout.
CLA r1(r411,r412,cout,Result);
endmodule
