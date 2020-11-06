`include "PP.v"

module testbench;
reg [7:0]a,b;
wire [16:0]r1;
wire cout;

initial  //Apply Inputs
begin
	#1
	a= 28 ;b=13;
	#100
	a= 250 ;b=250;
	#100
	a= 255 ;b=255;
	#100
	$finish;
end

CSAStages s1(a,b,r1,cout);

initial
begin
	$monitor($time, " a=%d b=%d result=%d cout=%d",a,b,r1,cout);
end

endmodule
