`include "PP.v"

module testbench;
reg [63:0]a,b;
wire [128:0]r1;
wire cout;

initial  //Apply Inputs
begin
	#1
	a= 120154 ;b=162134;
	#200
	a= 127032 ;b=655352;
	#200
	a= 4294967295 ;b=65535;
	#200
	a= 0 ;b=18446744073709551615;
	#200
	a= 18446744073709551615 ;b=18446744073709551615;
	#200
	$finish;
end

CSAStages s1(a,b,r1,cout);

initial
begin
	$monitor($time, " a=%d b=%d result=%d cout=%d",a,b,r1,cout);
end

endmodule
