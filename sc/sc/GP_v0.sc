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
//#function BITMARK           %b00010100000000000000000000000000
#function INCREMENTOR       %b10010100000000000000000000000000
#function MULT_V2           %b01010100000000000000000000000000
#function BITMARKP1         %d43
#function RECORDER          %d44
#function RESETTER          %d45
#function ESCAPE_BOTH       %d46
#function TAG_FITNESS       %d47
#function TAG_CHAIN         %d48
#function BITMARK2          %d49


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
#label bitmk                %b00010100000000000000000000000000

// and the program begins here:
//0
[0:1]main (%d0 %d0 %d0)
//1
//where the data gets recorded & recycled
[0:1]data_center (bin_label %d0 %d0)
//2
data0_0 (num %d0 %d1)
//3
data0_1 (num %d0 %d1)
//4
[0:1]fitness (%d19 %d0 %d0)

//Main scope
//5
[0:1]capt_ans ([%d20 zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//6
[0:1]capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//7
[0:1]esc ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
//8
[0:1]times ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
//9
[0:1]incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
//marks the number three on left schema
//10
[0:1]bmark ([num zero dontcare] BITMARK2(0,0) [num zero dontcare])
//data-center scope
//these two are just used for debugging
//the answer is in 20 -> 21. the  x value is in 3 -> 4
output ([%d21 zero dontcare] PRINT(0,0) [%d4 zero dontcare])
tst_output ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//14
[0:1]data_mark ([%d20 zero dontcare] BITMARKP1(0,0) [three zero dontcare])
//15
[0:1]rec ([%d21 zero dontcare] RECORDER(0,0) [%d19 zero dontcare])
//16
[0:1]res ([%d21 zero dontcare] RESETTER(0,0) [%d4 zero dontcare])
//17
[0:1]tagf ([%d19 zero dontcare] TAG_FITNESS(0,0) [dontcare dontcare dontcare])
//18
//notused exists in bmark2[0:1]tagc ([dontcare bitmk dontcare] TAG_CHAIN(0,0) [dontcare dontcare dontcare])
universe (%d0 %d0 %d0)
//try sharing the data systems?
data1_0 (num %d0 %d1)
data1_1 (num %d0 %d1)

//if the return value of data_mark is 1, tagf will execute
#chain data_mark0
{
($L output $R) + ($L rec0 ?A) + ($L tst_output A) + ($L res0 $R) + ($L tst_output $R) + ($L incrementor0 $R) + ($L tst_output $R) + ($L esc0 $R)
(?A tagf0 ?B)
}

#chain bmark0
{
($L times0 $R) + ($L capt_ans0 ?A) + ($R capt0 A)
}

#chain data_mark1
{
($L rec1 ?A) + ($L res1 $R) + ($L incrementor1 $R) + ($L esc1 $R)
(?A tagf1 ?B)
}

#chain bmark1
{
($L times1 $R) + ($L capt_ans1 ?A) + ($R capt1 A)
}

#scope universe
{
main0
main1
}

#scope main0
{
capt0
capt_ans0
data_center0
data0_0
data0_1
times0
bmark0

}

#scope data_center0
{
fitness0
data_mark0
}

#scope main1
{
capt1
capt_ans1
data_center1
data1_0
data1_1
times1
bmark1

}

#scope data_center1
{
fitness1
data_mark1
}

#systemic end