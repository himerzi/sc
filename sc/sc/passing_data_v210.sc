// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/passing_data_v2.sc

#systemic start
//Passing data. Based on Scoped seq.
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



// define some useful labels
#label first                %b10101010000000000000000000000000
#label bmarkid              %b00010100000000000000000000000000
#label second               %b01001001000000000000000000000000
#label num                  %b10000000000000000000000000000000
#label zero                 %b00000000000000000000000000000000
#label one                  %b10000000000000000000000000000000
#label two                  %b01000000000000000000000000000000
#label three                %b11000000000000000000000000000000
#label four                 %b00100000000000000000000000000000
#label twelve               %b00110000000000000000000000000000
#label garbage              %b?0000000000000000000000000000000
#label bin_label            %b11010000000000000000000000000000
//garbage has either "three" or "num" in its left schema"
#label garbage_s1           %b?0??0000000000000000000000000000

#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????
#label nooutput             %b????????0???????????????????????
#label notone               %b0???????????????????????????????

// and the program begins here:
barrenland (%d0 NOP(0,0) %d0)
solution (%d0 NOP(0,0) %d0)
main (%d0 NOP(0,0) %d0)
chain (%d0 NOP(0,0) %d0)
bin (%d11 NOP(0,0) %d0)
dummy (%d12 NOP(0,0) %d3)
data1 (%d3 NOP(0,0) %d2)
data1 (%d3 NOP(0,0) %d4)
data2 (%d1 NOP(0,0) %d2)
data3 (%d12 NOP(0,0) %d3)
data4 (%d12 NOP(0,0) %d4)
data5 (%d12 NOP(0,0) %d5)
data6 (%d12 NOP(0,0) %d6)
data7 (%d12 NOP(0,0) %d7)
data8 (%d12 NOP(0,0) %d8)
data9 (%d12 NOP(0,0) %d9)
capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
sum ([three zero dontcare] ADD(0,0) [twelve zero dontcare])
times ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
output ([three zero dontcare] PRINT(0,0) [dontcare zero dontcare])
incrementor ([three zero dontcare] INCREMENTOR(0,0) [dontcare zero dontcare])
bmark ([num zero dontcare] BITMARK(0,0) [num zero dontcare])

// set up any chains
#chain bmark
{
($L times $R) + ($L capt ?A) + ($R capt A)
}


// set up the scopes
#scope barrenland
{
      solution
}

#scope solution
{
      main
      output
}

#scope main
{
      bin
      capt
      times
      bmark
}

#scope bin
{
      data1
      data1
      output
}

#systemic end