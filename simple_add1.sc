// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/simple_add.sc

#systemic start

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



// define some useful labels
#label num                  %b10000000000000000000000000000000
#label zero                 %b00000000000000000000000000000000
#label one                  %b10000000000000000000000000000000
#label two                  %b01000000000000000000000000000000
#label three                %b11000000000000000000000000000000
#label four                 %b00100000000000000000000000000000
#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????

// and the program begins here:
main (%d0 NOP(0,0) %d0)
fred (%d4 NOP(0,0) %d0)
harry (%d4 NOP(0,0) %d81)
[0:7]data (%d1 NOP(0,0) %d0)
data8 (%d1 NOP(0,0) %d230)
data9 (%d1 NOP(0,0) %d0)
[0:4095]buffer (%d2 NOP(0,0) %d2)
sum ([num dontcare dontcare] ADD(0,0) [num dontcare dontcare])
power ([four zero three] POW(0,0) [four zero four])
output ([num dontcare dontcare] PRINT(0,0) [num dontcare dontcare])

// set up any chains
#chain sum
{
    ($L output $R)
}


// set up the scopes
#scope main
{
      fred
      harry
      [0:9]data
      [0:4095]buffer
      sum
      power
      output
}

#scope output
{
      [5:8]data
      buffer9
}

#systemic end