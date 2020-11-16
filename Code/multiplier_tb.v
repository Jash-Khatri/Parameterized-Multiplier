`include "multiplier.v"

module testbench;
reg [7:0]a,b;
reg clk;
wire [16:0]r1;
wire cout;

initial
begin
  clk = 1'b0;
end

always
begin
#16 clk = ~clk;
end

initial  //Apply Inputs
begin
	#0
	a= 28 ;b=13;
	#32
	a= 250 ;b=250;
	#32
	a= 255 ;b=255;
	#32
	a= 0 ;b= 255;
	#32
	a= 45 ;b= 149;
	#32
	a = 201; b=223;
	
	#1000
	$finish;
end

CSAStages s1(a,b,clk,r1,cout);

initial
begin
	$monitor($time, " a=%d b=%d result=%d cout=%d",a,b,r1,cout);
end

endmodule
