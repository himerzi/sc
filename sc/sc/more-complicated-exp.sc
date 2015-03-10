#systemic start
//this does nested operations

// define the functions
#function NOP               %b00000000000000000000000000000000
#function ADD               %b10000000000000000000000000000000
#function ADDe              %b01000000000000000000000000000000
#function SUBTRACT          %b11000000000000000000000000000000
#function SUBTRACTe         %b00100000000000000000000000000000
#function MULT              %b10100000000000000000000000000000
#function MULTe             %b01100000000000000000000000000000
#function DIV               %b11100000000000000000000000000000
#function DIVe              %b00010000000000000000000000000000
#function MOD               %b10010000000000000000000000000000
#function MODe              %b01010000000000000000000000000000
#function AND               %b11010000000000000000000000000000
#function OR                %b00110000000000000000000000000000
#function EOR               %b10110000000000000000000000000000
#function ZERO              %b01110000000000000000000000000000
#function ESCAPE            %b11110000000000000000000000000000
#function CAPTURE           %b00001000000000000000000000000000
#function PRINT             %b10001000000000000000000000000000
#function COPY              %b01001000000000000000000000000000
#function ISZERO            %b11001000000000000000000000000000
#function ISEQUAL           %b00101000000000000000000000000000
#function CAPTUREINHERIT    %b10101000000000000000000000000000
#function POW               %b00000100000000000000000000000000
#function ROOT              %b10000100000000000000000000000000
#function BITMARK           %b00010100000000000000000000000000



// define some useful labels
#label first                %b10101010000000000000000000000000
#label bmarkid              %b00010100000000000000000000000000
#label second               %b01001001000000000000000000000000
#label mult-type                  %b10000000000000000000000000000000
#label zero                 %b00000000000000000000000000000000
#label one                  %b10000000000000000000000000000000
#label two                  %b01000000000000000000000000000000
#label three                %b11000000000000000000000000000000
#label four                 %b00100000000000000000000000000000
#label sum-type               %b00110000000000000000000000000000
#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????
#label nooutput             %b????????0???????????????????????
#label notone               %b0???????????????????????????????

// and the program begins here:
barrenland (%d0 %d0 %d0)
main-wrapper (%d0 %d0 %d0)
operation0 (%d0 %d0 %d0)
operation1 (%d0 %d0 %d0)
operation2 (%d0 %d0 %d0)
chain-head0 (%d0 %d0 %d0)
chain-head1 (%d0 %d0 %d0)
[0:1]dummy-system (sum-type %d0 three)

//first set of data. for computing (3**2) op1
data10 (mult-type %d0 %3)
data20 (mult-type %d0 %d3)

//second set of data. for computing (11*13)+7 op0
data11 (mult-type %d0 %d1)
data21 (mult-type %d0 %d2)



//has bitmark in its kernel...matches dummy system on teh right.

// this esc is used to escape the bitmarkers which live in teh chain head scopes, out to the operations scope where they are
// are used to initiate operations
esc1 ([dontcare bmarkid dontcare] ESCAPE(0,0) [sum-type %d0 three])
//this escs are is to escape arithmetic in hte "operations" scopes" out into parent scope
esc ([three zero dontcare] ESCAPE(0,0) [dontcare zero dontcare])
//need one sum for 1+2 (this is op0), and one sum for (op0) + (op1)
[0:1]sum ([three zero dontcare] ADD(0,0) [sum-type zero dontcare])
//times is used in op1 3**2
times ([three zero dontcare] MULT(0,0) [three zero dontcare])
output  ([three zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//marks the number three on left schema
[0:1]bmark ([mult-type zero dontcare] BITMARK(0,0) [mult-type zero dontcare])



//3**2
#chain bmark0
{
($L times0 $R)  + ($L esc A)
}
//1+2
#chain bmark1
{
($L sum0 ?A) + ($L esc A)
}
#chain sum1
{
($L esc A)
}
#chain output
{
//so that stuff only gets printed once
($L esc $R)
}

// set up the scopes
//barrenland this is where systems go to rest (and not be printed)
#scope barrenland
{
main-wrapper
}
#scope main-wrapper
{
operation2
output
}
#scope operation2
{
operation0
operation1
sum1 //has an esc chained to it
}
#scope operation0
{
data10
data20
data30
times0
sum0
chain-head0
}
#scope operation1
{
data11
data21
data31
times0
sum0
chain-head1
}
#scope chain-head0
{
bmark0
dummy-system0
esc1
}
#scope chain-head1
{
bmark1
dummy-system1
esc1
}
#systemic end