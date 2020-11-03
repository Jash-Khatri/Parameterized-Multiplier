`include "PP8bits.v"

module testbench;
reg [7:0]a,b;
wire [14:0]o1,o2,o3,o4,o5,o6,o7,o8;

initial  //Apply Inputs
begin
	#1
	a= 28 ;b=13;
	#50
	a= 25 ;b=32;
	#50
	a= 255 ;b=255;
	#50
	$finish;
end

PP p1(a,b,o1,o2,o3,o4,o5,o6,o7,o8);

initial
begin
	$monitor($time, " a=%b b=%b p1=%b p2=%b p3=%b p4=%b p5=%b p6=%b p7=%b p8=%b",a,b,o1,o2,o3,o4,o5,o6,o7,o8);
end

endmodule
