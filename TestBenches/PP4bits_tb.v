`include "PP.v"

module testbench;
reg [3:0]a,b;
wire [8:0]r1;
wire cout;

initial  //Apply Inputs
begin
	#1
	a= 4 ;b=12;
	#100
	a= 8 ;b=9;
	#100
	a= 15 ;b=15;
	#100
	$finish;
end

CSAStages s1(a,b,r1,cout);

initial
begin
	$monitor($time, " a=%d b=%d result=%d cout=%d",a,b,r1,cout);
end

endmodule
