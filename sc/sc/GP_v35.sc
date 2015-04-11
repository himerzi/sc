// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/GP_v3.sc

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
#function TAG_FITNESS       %d47
#function TAG_CHAIN         %d48
#function BITMARK2          %d49
#function BITMARK_INIT      %d50
#function MARK_ANSWER       %d55


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
// and the program begins here:
//0
main0 (%d85 NOP(0,0) %d2)
main1 (%d95 NOP(0,0) %d5)
[0:1]data_center (%d11 NOP(0,0) %d0)
data0 (%d20 NOP(0,0) %d14)
data0 (%d3 NOP(0,0) %d11)
fitness0 (%d19 NOP(0,0) %d85)
fitness1 (%d19 NOP(0,0) %d95)
[0:1]capt_ans ([dontcare zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
[0:1]capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
[0:1]esc ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
[0:1]times ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
[0:1]incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
[0:1]bmark ([num zero dontcare] BITMARK_INIT(0,0) [num zero dontcare])
output ([dontcare zero dontcare] PRINT(0,0) [four zero dontcare])
tst_output ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
[0:1]data_mark ([dontcare zero dontcare] BITMARKP1(0,0) [three zero dontcare])
[0:1]rec ([dontcare zero dontcare] RECORDER(0,0) [dontcare zero dontcare])
[0:1]res ([dontcare zero dontcare] RESETTER(0,0) [four zero dontcare])
[0:1]tagf ([dontcare zero dontcare] TAG_FITNESS(0,0) [dontcare dontcare dontcare])
mark_ans ([three zero dontcare] (0,0) [dontcare dontcare dontcare])
universe (%d0 NOP(0,0) %d0)
hollow0 ([three zero dontcare] ADD2(0,0) [three zero dontcare])
hollow1 ([three zero dontcare] ADD3(0,0) [three zero dontcare])
hollow10 ([three zero dontcare] ADD2(0,0) [three zero dontcare])
hollow11 ([three zero dontcare] MARK_ANSWER(0,0) [three zero dontcare])
data1 (%d3 NOP(0,0) %d11)
data1 (%d20 NOP(0,0) %d15)

// set up any chains
#chain data_mark0
{
($L output $R) + ($L rec0 ?A) + ($L tst_output A) + ($L res0 $R) + ($L incrementor0 $R) + ($L esc0 $R)
(?A tagf0 ?B)
}

#chain bmark0
{
($L hollow00 $R) + ($L hollow01 $R) +  ($L mark_ans $R) + ($L capt_ans0 ?A) + ($R capt0 A)
}

#chain data_mark1
{
($L output $R) + ($L rec1 ?A) + ($L tst_output A) + ($L res1 $R) + ($L incrementor1 $R) + ($L esc1 $R)
(?A tagf1 ?B)
}

#chain bmark1
{
($L hollow10 $R) + ($L hollow11 $R) +  ($L mark_ans $R) +  ($L capt_ans1 ?A) + ($R capt1 A)
}


// set up the scopes
#scope main0
{
      data_center0
      capt_ans0
      capt0
      times0
      bmark0
}

#scope main1
{
      data_center1
      capt_ans1
      capt1
      times1
      bmark1
}

#scope data_center0
{
      data0
      data0
      fitness0
      data_mark0
}

#scope data_center1
{
      fitness1
      data_mark1
      data1
      data1
}

#scope universe
{
      [0:1]main
}

#systemic end