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
main1 (%d0 NOP(0,0) %d5)
main2 (%d495 NOP(0,0) %d8)
main3 (%d140 NOP(0,0) %d11)
main4 (%d75 NOP(0,0) %d14)
main5 (%d105 NOP(0,0) %d17)
main6 (%d0 NOP(0,0) %d20)
main7 (%d0 NOP(0,0) %d23)
main8 (%d0 NOP(0,0) %d26)
main9 (%d0 NOP(0,0) %d29)
[0:9]data_center (%d11 NOP(0,0) %d0)
data0 (%d1 NOP(0,0) %d11)
data1 (%d1 NOP(0,0) %d9)
data2 (%d3 NOP(0,0) %d11)
data3 (%d20 NOP(0,0) %d25)
data4 (%d20 NOP(0,0) %d13)
data5 (%d3 NOP(0,0) %d11)
data6 (%d1 NOP(0,0) %d11)
data7 (%d1 NOP(0,0) %d8)
data8 (%d3 NOP(0,0) %d9)
data9 (%d1 NOP(0,0) %d9)
data10 (%d1 NOP(0,0) %d11)
data11 (%d1 NOP(0,0) %d9)
data12 (%d20 NOP(0,0) %d143)
[13:14]data (%d3 NOP(0,0) %d11)
data15 (%d20 NOP(0,0) %d16)
data16 (%d1 NOP(0,0) %d11)
data17 (%d1 NOP(0,0) %d8)
data18 (%d20 NOP(0,0) %d82)
data19 (%d1 NOP(0,0) %d9)
fitness0 (%d19 NOP(0,0) %d770)
fitness1 (%d19 NOP(0,0) %d108)
fitness2 (%d19 NOP(0,0) %d495)
fitness3 (%d19 NOP(0,0) %d140)
fitness4 (%d19 NOP(0,0) %d75)
fitness5 (%d19 NOP(0,0) %d105)
fitness6 (%d19 NOP(0,0) %d550)
fitness7 (%d19 NOP(0,0) %d196)
fitness8 (%d19 NOP(0,0) %d212)
fitness9 (%d19 NOP(0,0) %d68)
[0:9]capt_ans ([dontcare zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
[0:9]capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
[0:9]esc ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
[0:9]incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
[0:9]bmark ([num zero dontcare] BITMARK_INIT(0,0) [num zero dontcare])
[0:9]output ([dontcare zero dontcare] PRINT(0,0) [four zero dontcare])
[0:9]tst_output ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
[0:9]data_mark ([dontcare zero dontcare] BITMARKP1(0,0) [three zero dontcare])
[0:9]rec ([dontcare zero dontcare] RECORDER(0,0) [dontcare zero dontcare])
[0:9]res ([dontcare zero dontcare] RESETTER(0,0) [four zero dontcare])
[0:9]tagf ([dontcare zero dontcare] TAG_FITNESS(0,0) [dontcare dontcare dontcare])
[0:9]mark_ans ([three zero dontcare] (0,0) [dontcare dontcare dontcare])
universe (%d0 NOP(0,0) %d0)
[0:1]hollow ([three zero dontcare] ADD1(0,0) [three zero dontcare])
hollow2 ([three zero dontcare] ADD3(0,0) [three zero dontcare])
hollow3 ([three zero dontcare] ADD1(0,0) [three zero dontcare])
hollow4 ([three zero dontcare] ADD2(0,0) [three zero dontcare])
hollow5 ([three zero dontcare] ADD3(0,0) [three zero dontcare])
hollow6 ([three zero dontcare] MARK_ANSWER(0,0) [three zero dontcare])
hollow7 ([three zero dontcare] ADD3(0,0) [three zero dontcare])
hollow8 ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
hollow9 ([three zero dontcare] MARK_ANSWER(0,0) [three zero dontcare])
hollow10 ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
hollow11 ([three zero dontcare] ADD1(0,0) [three zero dontcare])
hollow12 ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
hollow13 ([three zero dontcare] MARK_ANSWER(0,0) [three zero dontcare])
hollow14 ([three zero dontcare] ADD2(0,0) [three zero dontcare])
hollow15 ([three zero dontcare] MARK_ANSWER(0,0) [three zero dontcare])
[16:17]hollow ([three zero dontcare] MULT_V2(0,0) [three zero dontcare])
[18:19]hollow ([three zero dontcare] ADD2(0,0) [three zero dontcare])

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

#chain data_mark1
{
($L output1 $R) + ($L rec1 ?A) + ($L tst_output1 A) + ($L res1 $R) + ($L incrementor1 $R) + ($L esc1 $R)
(?A tagf1 ?B)
}

#chain bmark1
{
($L hollow01 $R) + ($L hollow11 $R) +  ($L mark_ans1 $R) + ($L capt_ans1 ?A) + ($R capt1 A)
}

#chain data_mark2
{
($L output2 $R) + ($L rec2 ?A) + ($L tst_output2 A) + ($L res2 $R) + ($L incrementor2 $R) + ($L esc2 $R)
(?A tagf2 ?B)
}

#chain bmark2
{
($L hollow02 $R) + ($L hollow12 $R) +  ($L mark_ans2 $R) + ($L capt_ans2 ?A) + ($R capt2 A)
}

#chain data_mark3
{
($L output3 $R) + ($L rec3 ?A) + ($L tst_output3 A) + ($L res3 $R) + ($L incrementor3 $R) + ($L esc3 $R)
(?A tagf3 ?B)
}

#chain bmark3
{
($L hollow03 $R) + ($L hollow13 $R) +  ($L mark_ans3 $R) + ($L capt_ans3 ?A) + ($R capt3 A)
}

#chain data_mark4
{
($L output4 $R) + ($L rec4 ?A) + ($L tst_output4 A) + ($L res4 $R) + ($L incrementor4 $R) + ($L esc4 $R)
(?A tagf4 ?B)
}

#chain bmark4
{
($L hollow04 $R) + ($L hollow14 $R) +  ($L mark_ans4 $R) + ($L capt_ans4 ?A) + ($R capt4 A)
}

#chain data_mark5
{
($L output5 $R) + ($L rec5 ?A) + ($L tst_output5 A) + ($L res5 $R) + ($L incrementor5 $R) + ($L esc5 $R)
(?A tagf5 ?B)
}

#chain bmark5
{
($L hollow05 $R) + ($L hollow15 $R) +  ($L mark_ans5 $R) + ($L capt_ans5 ?A) + ($R capt5 A)
}

#chain data_mark6
{
($L output6 $R) + ($L rec6 ?A) + ($L tst_output6 A) + ($L res6 $R) + ($L incrementor6 $R) + ($L esc6 $R)
(?A tagf6 ?B)
}

#chain bmark6
{
($L hollow06 $R) + ($L hollow16 $R) +  ($L mark_ans6 $R) + ($L capt_ans6 ?A) + ($R capt6 A)
}

#chain data_mark7
{
($L output7 $R) + ($L rec7 ?A) + ($L tst_output7 A) + ($L res7 $R) + ($L incrementor7 $R) + ($L esc7 $R)
(?A tagf7 ?B)
}

#chain bmark7
{
($L hollow07 $R) + ($L hollow17 $R) +  ($L mark_ans7 $R) + ($L capt_ans7 ?A) + ($R capt7 A)
}

#chain data_mark8
{
($L output8 $R) + ($L rec8 ?A) + ($L tst_output8 A) + ($L res8 $R) + ($L incrementor8 $R) + ($L esc8 $R)
(?A tagf8 ?B)
}

#chain bmark8
{
($L hollow08 $R) + ($L hollow18 $R) +  ($L mark_ans8 $R) + ($L capt_ans8 ?A) + ($R capt8 A)
}

#chain data_mark9
{
($L output9 $R) + ($L rec9 ?A) + ($L tst_output9 A) + ($L res9 $R) + ($L incrementor9 $R) + ($L esc9 $R)
(?A tagf9 ?B)
}

#chain bmark9
{
($L hollow09 $R) + ($L hollow19 $R) +  ($L mark_ans9 $R) + ($L capt_ans9 ?A) + ($R capt9 A)
}


// set up the scopes
#scope main0
{
      data_center0
      data0
      data10
      capt_ans0
      capt0
      bmark0
}

#scope main1
{
      data_center1
      data1
      data11
      capt_ans1
      capt1
      bmark1
}

#scope main2
{
      data_center2
      capt_ans2
      capt2
      bmark2
}

#scope main3
{
      data_center3
      capt_ans3
      capt3
      bmark3
}

#scope main4
{
      data_center4
      capt_ans4
      capt4
      bmark4
}

#scope main5
{
      data_center5
      capt_ans5
      capt5
      bmark5
}

#scope main6
{
      data_center6
      data6
      data16
      capt_ans6
      capt6
      bmark6
}

#scope main7
{
      data_center7
      data7
      data17
      capt_ans7
      capt7
      bmark7
}

#scope main8
{
      data_center8
      capt_ans8
      capt8
      bmark8
}

#scope main9
{
      data_center9
      data9
      data19
      capt_ans9
      capt9
      bmark9
}

#scope data_center0
{
      fitness0
      data_mark0
}

#scope data_center1
{
      fitness1
      data_mark1
}

#scope data_center2
{
      data2
      data12
      fitness2
      data_mark2
}

#scope data_center3
{
      data3
      data13
      fitness3
      data_mark3
}

#scope data_center4
{
      data4
      data14
      fitness4
      data_mark4
}

#scope data_center5
{
      data5
      data15
      fitness5
      data_mark5
}

#scope data_center6
{
      fitness6
      data_mark6
}

#scope data_center7
{
      fitness7
      data_mark7
}

#scope data_center8
{
      data8
      data18
      fitness8
      data_mark8
}

#scope data_center9
{
      fitness9
      data_mark9
}

#scope universe
{
      [0:9]main
}

#systemic end