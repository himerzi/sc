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
#function BITMARKP1         %d43
#function RECORDER          %d44
#function RESETTER          %d45
#function ESCAPE_BOTH       %d46


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
main (%d0 %d0 %d0)
//1
//where the data gets recorded & recycled
data_center (bin_label %d0 %d0)
//2
data1_0 (num %d0 %d0)
//3
data1_1 (num %d0 %d0)
//4
fitness (%d19 %d0 %d0)

//Main scope
//5
capt_ans ([%d20 zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//6
capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//7
esc ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
//8
times ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
//9
incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
//marks the number three on left schema
//10
bmark ([num zero dontcare] BITMARK(0,0) [num zero dontcare])
//data-center scope
//these two are just used for debugging
output ([%d21 zero dontcare] PRINT(0,0) [%d4 zero dontcare])
tst_output ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//14
data_mark ([%d20 zero dontcare] BITMARKP1(0,0) [three zero dontcare])
rec ([%d21 zero dontcare] RECORDER(0,0) [%d19 zero dontcare])
res ([%d21 zero dontcare] RESETTER(0,0) [%d4 zero dontcare])

#chain bmark
{
($L times $R) + ($L capt_ans ?A) + ($R capt A)
}

#chain data_mark
{
($L output $R) + ($L rec ?A) + ($L tst_output A) + ($L res $R) + ($L tst_output $R) + ($L incrementor $R) + ($L tst_output $R) + ($L esc $R)
}

#scope main
{
capt
capt_ans
data_center
data1_0
data1_1
times
bmark
}

#scope data_center
{
fitness
data_mark
}

#systemic end