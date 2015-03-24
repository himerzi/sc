// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/more-complicated-exp.sc

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
barrenland (%d0 NOP(0,0) %d0)
main (%d0 NOP(0,0) %d0)
[0:2]operation (%d0 NOP(0,0) %d0)
chain (%d0 NOP(0,0) %d0)
chain (%d0 NOP(0,0) %d0)
dummy (%d12 NOP(0,0) %d3)
dummy (%d12 NOP(0,0) %d3)
data10 (%d3 NOP(0,0) %d0)
data20 (%d3 NOP(0,0) %d0)
data30 (%d12 NOP(0,0) %d0)
data11 (%d3 NOP(0,0) %d0)
data21 (%d3 NOP(0,0) %d16)
esc1 ([dontcare bmarkid dontcare] ESCAPE(0,0) [sum-type zero three])
esc ([three zero dontcare] ESCAPE(0,0) [dontcare zero dontcare])
sum0 ([three zero dontcare] ADD(0,0) [sum-type zero dontcare])
sum1 ([three zero dontcare] ADD(0,0) [three zero dontcare])
times ([three zero dontcare] MULT(0,0) [three zero dontcare])
output ([three zero dontcare] PRINT(0,0) [dontcare zero dontcare])
[0:1]bmark ([mult-type zero dontcare] BITMARK(0,0) [mult-type zero dontcare])

// set up any chains
#chain bmark0
{
($L times $R)  +($L sum0 ?A) + ($L esc A)
}

#chain bmark1
{
($L times $R) + ($L esc $R)
}

#chain sum1
{
($L esc $R)
}

#chain output
{
//so that stuff only gets printed once
($L esc $R)
}


// set up the scopes
#scope barrenland
{
      main
}

#scope main-wrapper
{
      operation2
      data21
      output
}

#scope operation0
{
      chain
      data20
      data30
      sum0
      times
      bmark0
}

#scope operation1
{
      chain
      data11
      times
      bmark1
}

#scope operation2
{
      [0:1]operation
      data10
      sum1
}

#scope chain-head0
{
      dummy
      esc1
}

#scope chain-head1
{
      dummy
      esc1
}

#systemic end