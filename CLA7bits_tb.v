`include "CLA7bits.v"

module testbench;
reg [6:0]a,b;
wire [6:0]sum;
wire cout;

initial  //Apply Inputs
begin
	#1
	a= 127 ;b=1;
	#50
	a= 24 ;b=32;
	#50
	a= 32 ;b=16;
	#50
	a= 31 ;b=15;
	#50
	$finish;
end

CLA r1(a,b,cout,sum);

initial
begin
	$monitor($time, " a=%b b=%b cout=%b sum=%b",a,b,cout,sum);
end

endmodule
