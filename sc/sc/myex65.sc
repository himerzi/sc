// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/myex.sc

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
a1 (%d0 SUBTRACTe(0,0) %d0)
[2:3]a (%d0 SUBTRACT(0,0) %d0)
a4 (%d0 ADDe(0,0) %d0)
subtracte ([dontcare zero dontcare] SUBTRACTe(0,0) [dontcare dontcare dontcare])
sum ([num zero dontcare] ADD(0,0) [num zero dontcare])
times ([num zero dontcare] MULT(0,0) [num zero dontcare])
output ([num dontcare dontcare] PRINT(0,0) [num dontcare dontcare])
[1:3]c (%d255 %d63 %d255)

// set up any chains

// set up the scopes
#scope main
{
      times
      output
      [1:2]c
}

#scope c1
{
      [1:2]a
      subtracte
}

#scope c2
{
      [3:4]a
      subtracte
}

#systemic end