`include "PP.v"

module testbench;
reg [15:0]a,b;
wire [32:0]r1;
wire cout;

initial  //Apply Inputs
begin
	#1
	a= 12 ;b=16;
	#100
	a= 3801 ;b=1332;
	#100
	a= 65535 ;b=65535;
	#100
	a= 0 ;b=65535;
	#100
	$finish;
end

CSAStages s1(a,b,r1,cout);

initial
begin
	$monitor($time, " a=%d b=%d result=%d cout=%d",a,b,r1,cout);
end

endmodule
