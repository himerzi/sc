// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/pd4.sc

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
#function BITMARK           %b00010100000000000000000000000000
#function INCREMENTOR       %b10010100000000000000000000000000
#function MULT_V2           %b01010100000000000000000000000000
#function RECORDER          %d43
#function RESETTER          %d44
#function ESCAPE_BOTH       %d45
#function BITMARKP1         %d46

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
#label bin_label            %b11010000000000000000000000000000

#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????
#label nooutput             %b????????0???????????????????????
#label notone               %b0???????????????????????????????

// and the program begins here:
//0
main (%d0 NOP(0,0) %d0)
data_center (%d11 NOP(0,0) %d0)
data1 (%d3 NOP(0,0) %d2)
data1 (%d17 NOP(0,0) %d4)
fitness (%d19 NOP(0,0) %d0)
capt_ans ([dontcare zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
esc ([num zero dontcare] %d45 [num zero dontcare])
times ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
bmark ([num zero dontcare] BITMARK(0,0) [num zero dontcare])
output ([three zero dontcare] PRINT(0,0) [three zero dontcare])
data_mark ([dontcare zero dontcare] %d46 [three zero dontcare])

// set up any chains
#chain bmark
{
($L times $R) + ($L capt_ans ?A) + ($R capt A)
}

#chain data_mark
{
($L output $R)
}


// set up the scopes
#scope main
{
      data_center
      capt_ans
      capt
      times
      bmark
}

#scope data_center
{
      data1
      data1
}

#systemic end