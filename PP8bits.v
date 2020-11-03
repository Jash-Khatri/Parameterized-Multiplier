module PP(X,Y,p1,p2,p3,p4,p5,p6,p7,p8);

input [7:0]X,Y; 
output [14:0]p1,p2,p3,p4,p5,p6,p7,p8;

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
