`include "PP.v"

module testbench;
reg [31:0]a,b;
wire [64:0]r1;
wire cout;

initial  //Apply Inputs
begin
	#1
	a= 1201 ;b=1621;
	#200
	a= 127 ;b=65535;
	#200
	a= 4294967295 ;b=4294967295;
	#200
	a= 0 ;b=4294967295;
	#200
	a= 65535 ;b=65535;
	#200
	$finish;
end

CSAStages s1(a,b,r1,cout);

initial
begin
	$monitor($time, " a=%d b=%d result=%d cout=%d",a,b,r1,cout);
end

endmodule
