// Auto-generated output from decompiler.
// Original source: /Users/md/Documents/UCL 3/final year project/code/sc/sc/GP_autogen.sc

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
main0 (%d0 NOP(0,0) %d2)
data_center0 (%d11 NOP(0,0) %d0)
data0 (%d21 NOP(0,0) %d19)
data10 (%d4 NOP(0,0) %d8)
fitness0 (%d19 NOP(0,0) %d96)
capt_ans0 ([dontcare zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
capt0 ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
esc0 ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
incrementor0 ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
bmark0 ([num zero dontcare] BITMARK_INIT(0,0) [num zero dontcare])
output0 ([dontcare zero dontcare] PRINT(0,0) [four zero dontcare])
tst_output0 ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
data_mark0 ([dontcare zero dontcare] BITMARKP1(0,0) [three zero dontcare])
rec0 ([dontcare zero dontcare] RECORDER(0,0) [dontcare zero dontcare])
res0 ([dontcare zero dontcare] RESETTER(0,0) [four zero dontcare])
tagf0 ([dontcare zero dontcare] TAG_FITNESS(0,0) [dontcare dontcare dontcare])
mark_ans0 ([three zero dontcare] (0,0) [dontcare dontcare dontcare])
universe (%d0 NOP(0,0) %d0)
hollow0 ([three zero dontcare] ADD1(0,0) [three zero dontcare])
hollow10 ([three zero dontcare] MARK_ANSWER(0,0) [three zero dontcare])

// set up any chains
#chain data_mark0
{
($L output0 $R) + ($L rec0 ?A) + ($L tst_output0 A) + ($L res0 $R) + ($L incrementor0 $R) + ($L esc0 $R)
(?A tagf0 ?B)
}

#chain bmark0
{
($L hollow00 $R) + ($L hollow10 $R) +  ($L mark_ans0 $R) + ($L capt_ans0 ?A) + ($R capt0 A)
}


// set up the scopes
#scope main0
{
      data_center0
      capt_ans0
      capt0
      bmark0
}

#scope data_center0
{
      data0
      data10
      fitness0
      data_mark0
}

#scope universe
{
      main0
}

#systemic end