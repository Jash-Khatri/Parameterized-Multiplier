﻿

**How to run the code?**

• Run the following commands to generate the N-bit Wallace tree multiplier circuit (with

isolation register stage after every K stages) in Verilog from the C++ code. Note the file Multiplier\_gen\_pipeline.cpp is present in the Code directory

• The code will ask the user to input the value of N and K when the user runs ./a.out. For N,

you can enter any value in the power of 2. For K (>0), enter any value greater than zero, not

necessarily in the powers of 2.


g++ **Multiplier\_gen\_pipeline.cpp**
./a.out
8 2

• After running the above commands, the **multiplier.v** file will be generated in the current

directory, which is the Verilog file for the N-bit Wallace tree multiplier circuit (with

isolation register stage after every K stages). N is 8, and K is 2 in this case.

• In order to run the generated Verilog files using the test benches to see the multiplication

results, use the following commands.

iverilog multiplier.v

iverilog **multiplier\_tb.v**

./a.out

• **Note:** Carry Lookahead Adder is not pipelined while adding the pipeline stages. The

pipeline stages are only added to the CSA tree.

• **Note:** There will be **many warnings generated** when the user compiles the code

using the commands -> iverilog multiplier.v or iverilog multiplier\_tb.v. Please ignore

those warnings that will not affect the final results.

• **Note:** The Multiplier\_gen\_pipeline.cpp is the multiplier generation code, while the

multiplier.v is the self-generated Verilog code generated by

Multiplier\_gen\_pipeline.cpp. The multiplier\_tb.v is the testbench file, which is

written manually and is not generated by Multiplier\_gen\_pipeline.cpp.

• **Note:** The time period of the clock (clk input) has to be changed manually. This can

be done from multiplier\_tb.v.


**Figure(2)**

• The above figure shows the result of executing the generated verilog code by HLL

with N=8 and K=2. The **time period of the clock is set to 32 units.** Same results

**can be replicated by running the submitted testbench file(multiplier\_tb.v)**.

• The ‘a’ and ‘b’ represent multiplicand and multiplier, respectively.

• The ‘result’ shows the actual multiplication result while ‘cout’ is the carry out

of the CLA which can be ignored.

• Total of 6 inputs are feed to the pipelined multiplier circuit at each clock cycle

i.e. at t=0,32,64,96,128,160 respectively.

• The result of the first input is available at t=88. (28\*13 =364).

• After this point, results for all the successive inputs are getting available at the

successive clock cycles.

• So, 250\*250 will be available at 88 + 32(time period of clock) = 120 time unit.

• 255\*255 will be available at 88 + 2\*32 = 152 time unit.

• 0\*255 will be available at 88 + 3\*32 = 184 time unit.

• 45\*149 will be available at 88 + 4\*32 = 216 time unit.

• 201\*223 will be available at 88 + 5\*32 = 248 time unit.





**Results**

• The following table summarizes the results for the 8-bit pipelined Wallace tree

multiplier with K=1,2,3,4,5.

• Note that new input is pushed into the 8-bit pipelined Wallace tree multiplier in

each clock cycle. **A total of 11 inputs are feed to the pipelined multiplier.**

**The observations derived from it are tabulated below.**

• Note that output for the first input will be available after t time units. After that

in each clock cycle we will get the results for the rest of the inputs. Execution

time given below is the time taken to get outputs for all the inputs.

• Note that 8-bit Wallace tree multiplier has only 4 levels of the CSA tree. So

K=5(or above) will not add any register stage at all in the CSA tree (i.e. Non-

pipelined implementation of multiplier).

• The 11 inputs that we feed to the pipeline were (a,b) = (28,13) , (255,255) ,

(250,250) , (0,255) , (45,149) , (201,223) , (21,23) , (90,100) , (200,50) ,

(255,155) , (1,223). The **result tabulated below can be replicated by setting**

**values of a,b as mentioned above and adjusting clock as given in the table.**

**The delay after which each input (a,b) is provided to the multiplier has to**

**be adjusted according to the clock in the testbench file.**

N

K

Number of 1- Time period of Execution

bit registers

used for

pipelining

the clock (clk) time(in units) to

in units

finish all the 11

inputs

8

8

8

8

8

1

2

3

4

5

240

96

48

32

0

24 units

24 units

24 units

30 units

360 units

312 units

294 units

334 units

unclocked/Non- 660 units

pipelined





**Comparative Analysis for different values of K**

**Register needed for different values of K.**

**Figure(3)**

• The above figure tells us that as the value of K (from 1 to 5 in this case)

increases, the number of 1-bit registers needed to implement the pipeline

reduces.

• This is intuitive because for K=1, we add the register stage after every CSA stage,

while for K=4 we add the register stage only after every four register stages. Hence

the number of the register should decrease as K increases.

**Time period of the Clock for different values of K.**

• For K=1,2,3 CLA with the delay of 24 units will become the bottleneck for the

pipelined multiplier. Hence, the time period of the clock is set to 24 units.

• For K=4, CLA, and all 4 stages of the CSA tree are separated by the register stage. In

this case, the delay in processing the input from the CSA tree(30 units) will be more

than the delay of CLA (24 units). Hence, the clock is set to 30 units.

• For K=5 or above, we will not have any register stage as the K value exceeds the

number of CSA stages. So, in this case we do not need to set the time period of the

clock. In this cases, each multiplication operation may take as much as 60 units in

worst case.





**Execution time analysis for different values of K and**

**speedup vs area trade-off**

**Figure(4)**

• For K=5, Since there are no register stages that have been added. Hence the

execution is completely sequential (Non-pipelined). In this case, **each**

**multiplication may take takes 60 units in the worst case.** Hence 11

multiplication operations will take 660 units of time. However, **the advantage**

**here is we are not using any 1-bit register at all** since there is no pipeline

• For K=1, the execution time obtained is 360. Out of these 120 units of time are

needed to compute the first output. After that, we will get one output in each

clock cycle(24 units) for the rest of the 10 inputs. **Hence total delay = 120 +**

**10\*24 = 360.** For a large number of inputs, the time to compute the first output

will be less significant. Hence, in general, we can say **for N=8, K=1, and a**

**large number of inputs**, the time taken to compute each output is 24 units

(that is the time period of the clock). So, **speed-up over the sequential circuit**

**is 60/24 = 2.5,** and the **total number of 1-bit register needed is 240.**

• Similarly for K=2, the execution time obtained is 312. Out of these 72 units of

time are needed to compute the first output. After that, we will get one output

in each clock cycle(24 units) for the rest of the 10 inputs. **Hence total delay =**

**72 + 10\*24 = 312.** For a large number of inputs, the time to compute the first

output will be less significant. Hence, in general, we can say **for N=8, K=2,**

**and a large number of inputs**, the time taken to compute each output is 24





units (that is the time period of the clock). So, **speed-up over the sequential**

**circuit is 60/24 = 2.5,** and the **total number of 1-bit register needed is 96.**

• Similarly for K=3, the execution time obtained is 294. Out of these 54 units of

time are needed to compute the first output. After that, we will get one output

in each clock cycle(24 units) for the rest of the 10 inputs. **Hence total delay =**

**54 + 10\*24 = 294.** For a large number of inputs, the time to compute the first

output will be less significant. Hence, in general, we can say **for N=8, K=3,**

**and a large number of inputs**, the time taken to compute each output is 24

units (that is the time period of the clock). So, **speed-up over the sequential**

**circuit is 60/24 = 2.5,** and the **total number of 1-bit register needed is 48.**

• Finally for K=4, the execution time obtained is 354. Out of these 54 units of

time are needed to compute the first output. After that, we will get one output

in each clock cycle(30 units) for the rest of the 10 inputs. **Hence total delay =**

**54 + 10\*30 = 354.** For a large number of inputs, the time to compute the first

output will be less significant. Hence, in general, we can say **for N=8, K=4,**

**and a large number of inputs**, the time taken to compute each output is 30

units (that is the time period of the clock). So, **speed-up over the sequential**

**circuit is 60/30 = 2,** and the **total number of 1-bit register needed is 32.**

N

K

Speed-up over the Number of 1-bit

sequential execution registers used for

(if provided large

number of input

values)

pipelining

8

8

**8**

8

8

1

2.5

2.5

**2.5**

2

240

96

**48**

32

0

2

**3**

4

5 (or above)

1

**Conclusion**

• From these experiments, we can conclude that for **N=8; the K=3 may give us**

**the best achievable speed-up while using an optimal number of registers**

for implementing the 8-bit pipelined Wallace tree multiplier.

