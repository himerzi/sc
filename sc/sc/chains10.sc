// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/chains.sc

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
#label first                %b10101010000000000000000000000000
#label second               %b01001001000000000000000000000000
#label num                  %b10000000000000000000000000000000
#label zero                 %b00000000000000000000000000000000
#label one                  %b10000000000000000000000000000000
#label two                  %b01000000000000000000000000000000
#label three                %b11000000000000000000000000000000
#label four                 %b00100000000000000000000000000000
#label twelve               %b00110000000000000000000000000000
#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????
#label nooutput             %b????????0???????????????????????

// and the program begins here:
main (%d0 NOP(0,0) %d0)
[1:3]data (%d1 NOP(0,0) %d0)
a1 (%d12 NOP(0,0) %d2)
a2 (%d11 NOP(0,0) %d12)
a3 (%d1 NOP(0,0) %d4)
a4 (%d2 NOP(0,0) %d0)
sum ([num zero dontcare] ADD(0,0) [num zero dontcare])
times ([num zero dontcare] MULT(0,0) [num zero dontcare])
output ([num zero dontcare] PRINT(0,0) [num zero dontcare])
[1:2]c (%d255 NOP(0,0) %d255)

// set up any chains
#chain times
{
//note that sum here doesn't use $R, MULT sets $R to 1 after operating. So we need to fish out a new system from the scope
//to add to the result of MULT (stored in $L), this new system is ?A.
($L sum ?A) + ($L output A)
}


// set up the scopes
#scope main
{
      [1:3]data
      sum
      times
      output
}

#systemic end