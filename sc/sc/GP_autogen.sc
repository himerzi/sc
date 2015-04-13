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
[0:99]main (%d0 %d0 %d0)
//1
//where the data gets recorded & recycled
[0:99]data_center (bin_label %d0 %d0)
//2
[0:99]data0 (num %d0 %d1)
//3
[0:99]data1 (num %d0 %d1)
//4
[0:99]fitness (%d19 %d0 %d0)

//Main scope
//5
[0:99]capt_ans ([%d20 zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//6
[0:99]capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//7
[0:99]esc ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
//9
[0:99]incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
//marks the number three on left schema
//10
[0:99]bmark ([num zero dontcare] BITMARK_INIT(0,0) [num zero dontcare])
//data-center scope
//these two are just used for debugging
//the answer is in 20 -> 21. the  x value is in 3 -> 4
[0:99]output ([%d21 zero dontcare] PRINT(0,0) [%d4 zero dontcare])
[0:99]tst_output ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//14
[0:99]data_mark ([%d20 zero dontcare] BITMARKP1(0,0) [three zero dontcare])
//15
[0:99]rec ([%d21 zero dontcare] RECORDER(0,0) [%d19 zero dontcare])
//16
[0:99]res ([%d21 zero dontcare] RESETTER(0,0) [%d4 zero dontcare])
//17
[0:99]tagf ([%d19 zero dontcare] TAG_FITNESS(0,0) [dontcare dontcare dontcare])
[0:99]mark_ans ([three zero dontcare] MARK_ANSWER(0,0) [dontcare dontcare dontcare])
universe (%d0 %d0 %d0)
//read_main ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//blah (%d0 %d0 %d0)
[0:99]hollow0 ([three zero dontcare] NOP(0,0) [three zero dontcare])
[0:99]hollow1 ([three zero dontcare] NOP(0,0) [three zero dontcare])

///////////    Begin chain definitions /////////

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

#chain data_mark10
{
($L output10 $R) + ($L rec10 ?A) + ($L tst_output10 A) + ($L res10 $R) + ($L incrementor10 $R) + ($L esc10 $R)
(?A tagf10 ?B)
}

#chain bmark10
{
($L hollow010 $R) + ($L hollow110 $R) +  ($L mark_ans10 $R) + ($L capt_ans10 ?A) + ($R capt10 A)
}

#chain data_mark11
{
($L output11 $R) + ($L rec11 ?A) + ($L tst_output11 A) + ($L res11 $R) + ($L incrementor11 $R) + ($L esc11 $R)
(?A tagf11 ?B)
}

#chain bmark11
{
($L hollow011 $R) + ($L hollow111 $R) +  ($L mark_ans11 $R) + ($L capt_ans11 ?A) + ($R capt11 A)
}

#chain data_mark12
{
($L output12 $R) + ($L rec12 ?A) + ($L tst_output12 A) + ($L res12 $R) + ($L incrementor12 $R) + ($L esc12 $R)
(?A tagf12 ?B)
}

#chain bmark12
{
($L hollow012 $R) + ($L hollow112 $R) +  ($L mark_ans12 $R) + ($L capt_ans12 ?A) + ($R capt12 A)
}

#chain data_mark13
{
($L output13 $R) + ($L rec13 ?A) + ($L tst_output13 A) + ($L res13 $R) + ($L incrementor13 $R) + ($L esc13 $R)
(?A tagf13 ?B)
}

#chain bmark13
{
($L hollow013 $R) + ($L hollow113 $R) +  ($L mark_ans13 $R) + ($L capt_ans13 ?A) + ($R capt13 A)
}

#chain data_mark14
{
($L output14 $R) + ($L rec14 ?A) + ($L tst_output14 A) + ($L res14 $R) + ($L incrementor14 $R) + ($L esc14 $R)
(?A tagf14 ?B)
}

#chain bmark14
{
($L hollow014 $R) + ($L hollow114 $R) +  ($L mark_ans14 $R) + ($L capt_ans14 ?A) + ($R capt14 A)
}

#chain data_mark15
{
($L output15 $R) + ($L rec15 ?A) + ($L tst_output15 A) + ($L res15 $R) + ($L incrementor15 $R) + ($L esc15 $R)
(?A tagf15 ?B)
}

#chain bmark15
{
($L hollow015 $R) + ($L hollow115 $R) +  ($L mark_ans15 $R) + ($L capt_ans15 ?A) + ($R capt15 A)
}

#chain data_mark16
{
($L output16 $R) + ($L rec16 ?A) + ($L tst_output16 A) + ($L res16 $R) + ($L incrementor16 $R) + ($L esc16 $R)
(?A tagf16 ?B)
}

#chain bmark16
{
($L hollow016 $R) + ($L hollow116 $R) +  ($L mark_ans16 $R) + ($L capt_ans16 ?A) + ($R capt16 A)
}

#chain data_mark17
{
($L output17 $R) + ($L rec17 ?A) + ($L tst_output17 A) + ($L res17 $R) + ($L incrementor17 $R) + ($L esc17 $R)
(?A tagf17 ?B)
}

#chain bmark17
{
($L hollow017 $R) + ($L hollow117 $R) +  ($L mark_ans17 $R) + ($L capt_ans17 ?A) + ($R capt17 A)
}

#chain data_mark18
{
($L output18 $R) + ($L rec18 ?A) + ($L tst_output18 A) + ($L res18 $R) + ($L incrementor18 $R) + ($L esc18 $R)
(?A tagf18 ?B)
}

#chain bmark18
{
($L hollow018 $R) + ($L hollow118 $R) +  ($L mark_ans18 $R) + ($L capt_ans18 ?A) + ($R capt18 A)
}

#chain data_mark19
{
($L output19 $R) + ($L rec19 ?A) + ($L tst_output19 A) + ($L res19 $R) + ($L incrementor19 $R) + ($L esc19 $R)
(?A tagf19 ?B)
}

#chain bmark19
{
($L hollow019 $R) + ($L hollow119 $R) +  ($L mark_ans19 $R) + ($L capt_ans19 ?A) + ($R capt19 A)
}

#chain data_mark20
{
($L output20 $R) + ($L rec20 ?A) + ($L tst_output20 A) + ($L res20 $R) + ($L incrementor20 $R) + ($L esc20 $R)
(?A tagf20 ?B)
}

#chain bmark20
{
($L hollow020 $R) + ($L hollow120 $R) +  ($L mark_ans20 $R) + ($L capt_ans20 ?A) + ($R capt20 A)
}

#chain data_mark21
{
($L output21 $R) + ($L rec21 ?A) + ($L tst_output21 A) + ($L res21 $R) + ($L incrementor21 $R) + ($L esc21 $R)
(?A tagf21 ?B)
}

#chain bmark21
{
($L hollow021 $R) + ($L hollow121 $R) +  ($L mark_ans21 $R) + ($L capt_ans21 ?A) + ($R capt21 A)
}

#chain data_mark22
{
($L output22 $R) + ($L rec22 ?A) + ($L tst_output22 A) + ($L res22 $R) + ($L incrementor22 $R) + ($L esc22 $R)
(?A tagf22 ?B)
}

#chain bmark22
{
($L hollow022 $R) + ($L hollow122 $R) +  ($L mark_ans22 $R) + ($L capt_ans22 ?A) + ($R capt22 A)
}

#chain data_mark23
{
($L output23 $R) + ($L rec23 ?A) + ($L tst_output23 A) + ($L res23 $R) + ($L incrementor23 $R) + ($L esc23 $R)
(?A tagf23 ?B)
}

#chain bmark23
{
($L hollow023 $R) + ($L hollow123 $R) +  ($L mark_ans23 $R) + ($L capt_ans23 ?A) + ($R capt23 A)
}

#chain data_mark24
{
($L output24 $R) + ($L rec24 ?A) + ($L tst_output24 A) + ($L res24 $R) + ($L incrementor24 $R) + ($L esc24 $R)
(?A tagf24 ?B)
}

#chain bmark24
{
($L hollow024 $R) + ($L hollow124 $R) +  ($L mark_ans24 $R) + ($L capt_ans24 ?A) + ($R capt24 A)
}

#chain data_mark25
{
($L output25 $R) + ($L rec25 ?A) + ($L tst_output25 A) + ($L res25 $R) + ($L incrementor25 $R) + ($L esc25 $R)
(?A tagf25 ?B)
}

#chain bmark25
{
($L hollow025 $R) + ($L hollow125 $R) +  ($L mark_ans25 $R) + ($L capt_ans25 ?A) + ($R capt25 A)
}

#chain data_mark26
{
($L output26 $R) + ($L rec26 ?A) + ($L tst_output26 A) + ($L res26 $R) + ($L incrementor26 $R) + ($L esc26 $R)
(?A tagf26 ?B)
}

#chain bmark26
{
($L hollow026 $R) + ($L hollow126 $R) +  ($L mark_ans26 $R) + ($L capt_ans26 ?A) + ($R capt26 A)
}

#chain data_mark27
{
($L output27 $R) + ($L rec27 ?A) + ($L tst_output27 A) + ($L res27 $R) + ($L incrementor27 $R) + ($L esc27 $R)
(?A tagf27 ?B)
}

#chain bmark27
{
($L hollow027 $R) + ($L hollow127 $R) +  ($L mark_ans27 $R) + ($L capt_ans27 ?A) + ($R capt27 A)
}

#chain data_mark28
{
($L output28 $R) + ($L rec28 ?A) + ($L tst_output28 A) + ($L res28 $R) + ($L incrementor28 $R) + ($L esc28 $R)
(?A tagf28 ?B)
}

#chain bmark28
{
($L hollow028 $R) + ($L hollow128 $R) +  ($L mark_ans28 $R) + ($L capt_ans28 ?A) + ($R capt28 A)
}

#chain data_mark29
{
($L output29 $R) + ($L rec29 ?A) + ($L tst_output29 A) + ($L res29 $R) + ($L incrementor29 $R) + ($L esc29 $R)
(?A tagf29 ?B)
}

#chain bmark29
{
($L hollow029 $R) + ($L hollow129 $R) +  ($L mark_ans29 $R) + ($L capt_ans29 ?A) + ($R capt29 A)
}

#chain data_mark30
{
($L output30 $R) + ($L rec30 ?A) + ($L tst_output30 A) + ($L res30 $R) + ($L incrementor30 $R) + ($L esc30 $R)
(?A tagf30 ?B)
}

#chain bmark30
{
($L hollow030 $R) + ($L hollow130 $R) +  ($L mark_ans30 $R) + ($L capt_ans30 ?A) + ($R capt30 A)
}

#chain data_mark31
{
($L output31 $R) + ($L rec31 ?A) + ($L tst_output31 A) + ($L res31 $R) + ($L incrementor31 $R) + ($L esc31 $R)
(?A tagf31 ?B)
}

#chain bmark31
{
($L hollow031 $R) + ($L hollow131 $R) +  ($L mark_ans31 $R) + ($L capt_ans31 ?A) + ($R capt31 A)
}

#chain data_mark32
{
($L output32 $R) + ($L rec32 ?A) + ($L tst_output32 A) + ($L res32 $R) + ($L incrementor32 $R) + ($L esc32 $R)
(?A tagf32 ?B)
}

#chain bmark32
{
($L hollow032 $R) + ($L hollow132 $R) +  ($L mark_ans32 $R) + ($L capt_ans32 ?A) + ($R capt32 A)
}

#chain data_mark33
{
($L output33 $R) + ($L rec33 ?A) + ($L tst_output33 A) + ($L res33 $R) + ($L incrementor33 $R) + ($L esc33 $R)
(?A tagf33 ?B)
}

#chain bmark33
{
($L hollow033 $R) + ($L hollow133 $R) +  ($L mark_ans33 $R) + ($L capt_ans33 ?A) + ($R capt33 A)
}

#chain data_mark34
{
($L output34 $R) + ($L rec34 ?A) + ($L tst_output34 A) + ($L res34 $R) + ($L incrementor34 $R) + ($L esc34 $R)
(?A tagf34 ?B)
}

#chain bmark34
{
($L hollow034 $R) + ($L hollow134 $R) +  ($L mark_ans34 $R) + ($L capt_ans34 ?A) + ($R capt34 A)
}

#chain data_mark35
{
($L output35 $R) + ($L rec35 ?A) + ($L tst_output35 A) + ($L res35 $R) + ($L incrementor35 $R) + ($L esc35 $R)
(?A tagf35 ?B)
}

#chain bmark35
{
($L hollow035 $R) + ($L hollow135 $R) +  ($L mark_ans35 $R) + ($L capt_ans35 ?A) + ($R capt35 A)
}

#chain data_mark36
{
($L output36 $R) + ($L rec36 ?A) + ($L tst_output36 A) + ($L res36 $R) + ($L incrementor36 $R) + ($L esc36 $R)
(?A tagf36 ?B)
}

#chain bmark36
{
($L hollow036 $R) + ($L hollow136 $R) +  ($L mark_ans36 $R) + ($L capt_ans36 ?A) + ($R capt36 A)
}

#chain data_mark37
{
($L output37 $R) + ($L rec37 ?A) + ($L tst_output37 A) + ($L res37 $R) + ($L incrementor37 $R) + ($L esc37 $R)
(?A tagf37 ?B)
}

#chain bmark37
{
($L hollow037 $R) + ($L hollow137 $R) +  ($L mark_ans37 $R) + ($L capt_ans37 ?A) + ($R capt37 A)
}

#chain data_mark38
{
($L output38 $R) + ($L rec38 ?A) + ($L tst_output38 A) + ($L res38 $R) + ($L incrementor38 $R) + ($L esc38 $R)
(?A tagf38 ?B)
}

#chain bmark38
{
($L hollow038 $R) + ($L hollow138 $R) +  ($L mark_ans38 $R) + ($L capt_ans38 ?A) + ($R capt38 A)
}

#chain data_mark39
{
($L output39 $R) + ($L rec39 ?A) + ($L tst_output39 A) + ($L res39 $R) + ($L incrementor39 $R) + ($L esc39 $R)
(?A tagf39 ?B)
}

#chain bmark39
{
($L hollow039 $R) + ($L hollow139 $R) +  ($L mark_ans39 $R) + ($L capt_ans39 ?A) + ($R capt39 A)
}

#chain data_mark40
{
($L output40 $R) + ($L rec40 ?A) + ($L tst_output40 A) + ($L res40 $R) + ($L incrementor40 $R) + ($L esc40 $R)
(?A tagf40 ?B)
}

#chain bmark40
{
($L hollow040 $R) + ($L hollow140 $R) +  ($L mark_ans40 $R) + ($L capt_ans40 ?A) + ($R capt40 A)
}

#chain data_mark41
{
($L output41 $R) + ($L rec41 ?A) + ($L tst_output41 A) + ($L res41 $R) + ($L incrementor41 $R) + ($L esc41 $R)
(?A tagf41 ?B)
}

#chain bmark41
{
($L hollow041 $R) + ($L hollow141 $R) +  ($L mark_ans41 $R) + ($L capt_ans41 ?A) + ($R capt41 A)
}

#chain data_mark42
{
($L output42 $R) + ($L rec42 ?A) + ($L tst_output42 A) + ($L res42 $R) + ($L incrementor42 $R) + ($L esc42 $R)
(?A tagf42 ?B)
}

#chain bmark42
{
($L hollow042 $R) + ($L hollow142 $R) +  ($L mark_ans42 $R) + ($L capt_ans42 ?A) + ($R capt42 A)
}

#chain data_mark43
{
($L output43 $R) + ($L rec43 ?A) + ($L tst_output43 A) + ($L res43 $R) + ($L incrementor43 $R) + ($L esc43 $R)
(?A tagf43 ?B)
}

#chain bmark43
{
($L hollow043 $R) + ($L hollow143 $R) +  ($L mark_ans43 $R) + ($L capt_ans43 ?A) + ($R capt43 A)
}

#chain data_mark44
{
($L output44 $R) + ($L rec44 ?A) + ($L tst_output44 A) + ($L res44 $R) + ($L incrementor44 $R) + ($L esc44 $R)
(?A tagf44 ?B)
}

#chain bmark44
{
($L hollow044 $R) + ($L hollow144 $R) +  ($L mark_ans44 $R) + ($L capt_ans44 ?A) + ($R capt44 A)
}

#chain data_mark45
{
($L output45 $R) + ($L rec45 ?A) + ($L tst_output45 A) + ($L res45 $R) + ($L incrementor45 $R) + ($L esc45 $R)
(?A tagf45 ?B)
}

#chain bmark45
{
($L hollow045 $R) + ($L hollow145 $R) +  ($L mark_ans45 $R) + ($L capt_ans45 ?A) + ($R capt45 A)
}

#chain data_mark46
{
($L output46 $R) + ($L rec46 ?A) + ($L tst_output46 A) + ($L res46 $R) + ($L incrementor46 $R) + ($L esc46 $R)
(?A tagf46 ?B)
}

#chain bmark46
{
($L hollow046 $R) + ($L hollow146 $R) +  ($L mark_ans46 $R) + ($L capt_ans46 ?A) + ($R capt46 A)
}

#chain data_mark47
{
($L output47 $R) + ($L rec47 ?A) + ($L tst_output47 A) + ($L res47 $R) + ($L incrementor47 $R) + ($L esc47 $R)
(?A tagf47 ?B)
}

#chain bmark47
{
($L hollow047 $R) + ($L hollow147 $R) +  ($L mark_ans47 $R) + ($L capt_ans47 ?A) + ($R capt47 A)
}

#chain data_mark48
{
($L output48 $R) + ($L rec48 ?A) + ($L tst_output48 A) + ($L res48 $R) + ($L incrementor48 $R) + ($L esc48 $R)
(?A tagf48 ?B)
}

#chain bmark48
{
($L hollow048 $R) + ($L hollow148 $R) +  ($L mark_ans48 $R) + ($L capt_ans48 ?A) + ($R capt48 A)
}

#chain data_mark49
{
($L output49 $R) + ($L rec49 ?A) + ($L tst_output49 A) + ($L res49 $R) + ($L incrementor49 $R) + ($L esc49 $R)
(?A tagf49 ?B)
}

#chain bmark49
{
($L hollow049 $R) + ($L hollow149 $R) +  ($L mark_ans49 $R) + ($L capt_ans49 ?A) + ($R capt49 A)
}

#chain data_mark50
{
($L output50 $R) + ($L rec50 ?A) + ($L tst_output50 A) + ($L res50 $R) + ($L incrementor50 $R) + ($L esc50 $R)
(?A tagf50 ?B)
}

#chain bmark50
{
($L hollow050 $R) + ($L hollow150 $R) +  ($L mark_ans50 $R) + ($L capt_ans50 ?A) + ($R capt50 A)
}

#chain data_mark51
{
($L output51 $R) + ($L rec51 ?A) + ($L tst_output51 A) + ($L res51 $R) + ($L incrementor51 $R) + ($L esc51 $R)
(?A tagf51 ?B)
}

#chain bmark51
{
($L hollow051 $R) + ($L hollow151 $R) +  ($L mark_ans51 $R) + ($L capt_ans51 ?A) + ($R capt51 A)
}

#chain data_mark52
{
($L output52 $R) + ($L rec52 ?A) + ($L tst_output52 A) + ($L res52 $R) + ($L incrementor52 $R) + ($L esc52 $R)
(?A tagf52 ?B)
}

#chain bmark52
{
($L hollow052 $R) + ($L hollow152 $R) +  ($L mark_ans52 $R) + ($L capt_ans52 ?A) + ($R capt52 A)
}

#chain data_mark53
{
($L output53 $R) + ($L rec53 ?A) + ($L tst_output53 A) + ($L res53 $R) + ($L incrementor53 $R) + ($L esc53 $R)
(?A tagf53 ?B)
}

#chain bmark53
{
($L hollow053 $R) + ($L hollow153 $R) +  ($L mark_ans53 $R) + ($L capt_ans53 ?A) + ($R capt53 A)
}

#chain data_mark54
{
($L output54 $R) + ($L rec54 ?A) + ($L tst_output54 A) + ($L res54 $R) + ($L incrementor54 $R) + ($L esc54 $R)
(?A tagf54 ?B)
}

#chain bmark54
{
($L hollow054 $R) + ($L hollow154 $R) +  ($L mark_ans54 $R) + ($L capt_ans54 ?A) + ($R capt54 A)
}

#chain data_mark55
{
($L output55 $R) + ($L rec55 ?A) + ($L tst_output55 A) + ($L res55 $R) + ($L incrementor55 $R) + ($L esc55 $R)
(?A tagf55 ?B)
}

#chain bmark55
{
($L hollow055 $R) + ($L hollow155 $R) +  ($L mark_ans55 $R) + ($L capt_ans55 ?A) + ($R capt55 A)
}

#chain data_mark56
{
($L output56 $R) + ($L rec56 ?A) + ($L tst_output56 A) + ($L res56 $R) + ($L incrementor56 $R) + ($L esc56 $R)
(?A tagf56 ?B)
}

#chain bmark56
{
($L hollow056 $R) + ($L hollow156 $R) +  ($L mark_ans56 $R) + ($L capt_ans56 ?A) + ($R capt56 A)
}

#chain data_mark57
{
($L output57 $R) + ($L rec57 ?A) + ($L tst_output57 A) + ($L res57 $R) + ($L incrementor57 $R) + ($L esc57 $R)
(?A tagf57 ?B)
}

#chain bmark57
{
($L hollow057 $R) + ($L hollow157 $R) +  ($L mark_ans57 $R) + ($L capt_ans57 ?A) + ($R capt57 A)
}

#chain data_mark58
{
($L output58 $R) + ($L rec58 ?A) + ($L tst_output58 A) + ($L res58 $R) + ($L incrementor58 $R) + ($L esc58 $R)
(?A tagf58 ?B)
}

#chain bmark58
{
($L hollow058 $R) + ($L hollow158 $R) +  ($L mark_ans58 $R) + ($L capt_ans58 ?A) + ($R capt58 A)
}

#chain data_mark59
{
($L output59 $R) + ($L rec59 ?A) + ($L tst_output59 A) + ($L res59 $R) + ($L incrementor59 $R) + ($L esc59 $R)
(?A tagf59 ?B)
}

#chain bmark59
{
($L hollow059 $R) + ($L hollow159 $R) +  ($L mark_ans59 $R) + ($L capt_ans59 ?A) + ($R capt59 A)
}

#chain data_mark60
{
($L output60 $R) + ($L rec60 ?A) + ($L tst_output60 A) + ($L res60 $R) + ($L incrementor60 $R) + ($L esc60 $R)
(?A tagf60 ?B)
}

#chain bmark60
{
($L hollow060 $R) + ($L hollow160 $R) +  ($L mark_ans60 $R) + ($L capt_ans60 ?A) + ($R capt60 A)
}

#chain data_mark61
{
($L output61 $R) + ($L rec61 ?A) + ($L tst_output61 A) + ($L res61 $R) + ($L incrementor61 $R) + ($L esc61 $R)
(?A tagf61 ?B)
}

#chain bmark61
{
($L hollow061 $R) + ($L hollow161 $R) +  ($L mark_ans61 $R) + ($L capt_ans61 ?A) + ($R capt61 A)
}

#chain data_mark62
{
($L output62 $R) + ($L rec62 ?A) + ($L tst_output62 A) + ($L res62 $R) + ($L incrementor62 $R) + ($L esc62 $R)
(?A tagf62 ?B)
}

#chain bmark62
{
($L hollow062 $R) + ($L hollow162 $R) +  ($L mark_ans62 $R) + ($L capt_ans62 ?A) + ($R capt62 A)
}

#chain data_mark63
{
($L output63 $R) + ($L rec63 ?A) + ($L tst_output63 A) + ($L res63 $R) + ($L incrementor63 $R) + ($L esc63 $R)
(?A tagf63 ?B)
}

#chain bmark63
{
($L hollow063 $R) + ($L hollow163 $R) +  ($L mark_ans63 $R) + ($L capt_ans63 ?A) + ($R capt63 A)
}

#chain data_mark64
{
($L output64 $R) + ($L rec64 ?A) + ($L tst_output64 A) + ($L res64 $R) + ($L incrementor64 $R) + ($L esc64 $R)
(?A tagf64 ?B)
}

#chain bmark64
{
($L hollow064 $R) + ($L hollow164 $R) +  ($L mark_ans64 $R) + ($L capt_ans64 ?A) + ($R capt64 A)
}

#chain data_mark65
{
($L output65 $R) + ($L rec65 ?A) + ($L tst_output65 A) + ($L res65 $R) + ($L incrementor65 $R) + ($L esc65 $R)
(?A tagf65 ?B)
}

#chain bmark65
{
($L hollow065 $R) + ($L hollow165 $R) +  ($L mark_ans65 $R) + ($L capt_ans65 ?A) + ($R capt65 A)
}

#chain data_mark66
{
($L output66 $R) + ($L rec66 ?A) + ($L tst_output66 A) + ($L res66 $R) + ($L incrementor66 $R) + ($L esc66 $R)
(?A tagf66 ?B)
}

#chain bmark66
{
($L hollow066 $R) + ($L hollow166 $R) +  ($L mark_ans66 $R) + ($L capt_ans66 ?A) + ($R capt66 A)
}

#chain data_mark67
{
($L output67 $R) + ($L rec67 ?A) + ($L tst_output67 A) + ($L res67 $R) + ($L incrementor67 $R) + ($L esc67 $R)
(?A tagf67 ?B)
}

#chain bmark67
{
($L hollow067 $R) + ($L hollow167 $R) +  ($L mark_ans67 $R) + ($L capt_ans67 ?A) + ($R capt67 A)
}

#chain data_mark68
{
($L output68 $R) + ($L rec68 ?A) + ($L tst_output68 A) + ($L res68 $R) + ($L incrementor68 $R) + ($L esc68 $R)
(?A tagf68 ?B)
}

#chain bmark68
{
($L hollow068 $R) + ($L hollow168 $R) +  ($L mark_ans68 $R) + ($L capt_ans68 ?A) + ($R capt68 A)
}

#chain data_mark69
{
($L output69 $R) + ($L rec69 ?A) + ($L tst_output69 A) + ($L res69 $R) + ($L incrementor69 $R) + ($L esc69 $R)
(?A tagf69 ?B)
}

#chain bmark69
{
($L hollow069 $R) + ($L hollow169 $R) +  ($L mark_ans69 $R) + ($L capt_ans69 ?A) + ($R capt69 A)
}

#chain data_mark70
{
($L output70 $R) + ($L rec70 ?A) + ($L tst_output70 A) + ($L res70 $R) + ($L incrementor70 $R) + ($L esc70 $R)
(?A tagf70 ?B)
}

#chain bmark70
{
($L hollow070 $R) + ($L hollow170 $R) +  ($L mark_ans70 $R) + ($L capt_ans70 ?A) + ($R capt70 A)
}

#chain data_mark71
{
($L output71 $R) + ($L rec71 ?A) + ($L tst_output71 A) + ($L res71 $R) + ($L incrementor71 $R) + ($L esc71 $R)
(?A tagf71 ?B)
}

#chain bmark71
{
($L hollow071 $R) + ($L hollow171 $R) +  ($L mark_ans71 $R) + ($L capt_ans71 ?A) + ($R capt71 A)
}

#chain data_mark72
{
($L output72 $R) + ($L rec72 ?A) + ($L tst_output72 A) + ($L res72 $R) + ($L incrementor72 $R) + ($L esc72 $R)
(?A tagf72 ?B)
}

#chain bmark72
{
($L hollow072 $R) + ($L hollow172 $R) +  ($L mark_ans72 $R) + ($L capt_ans72 ?A) + ($R capt72 A)
}

#chain data_mark73
{
($L output73 $R) + ($L rec73 ?A) + ($L tst_output73 A) + ($L res73 $R) + ($L incrementor73 $R) + ($L esc73 $R)
(?A tagf73 ?B)
}

#chain bmark73
{
($L hollow073 $R) + ($L hollow173 $R) +  ($L mark_ans73 $R) + ($L capt_ans73 ?A) + ($R capt73 A)
}

#chain data_mark74
{
($L output74 $R) + ($L rec74 ?A) + ($L tst_output74 A) + ($L res74 $R) + ($L incrementor74 $R) + ($L esc74 $R)
(?A tagf74 ?B)
}

#chain bmark74
{
($L hollow074 $R) + ($L hollow174 $R) +  ($L mark_ans74 $R) + ($L capt_ans74 ?A) + ($R capt74 A)
}

#chain data_mark75
{
($L output75 $R) + ($L rec75 ?A) + ($L tst_output75 A) + ($L res75 $R) + ($L incrementor75 $R) + ($L esc75 $R)
(?A tagf75 ?B)
}

#chain bmark75
{
($L hollow075 $R) + ($L hollow175 $R) +  ($L mark_ans75 $R) + ($L capt_ans75 ?A) + ($R capt75 A)
}

#chain data_mark76
{
($L output76 $R) + ($L rec76 ?A) + ($L tst_output76 A) + ($L res76 $R) + ($L incrementor76 $R) + ($L esc76 $R)
(?A tagf76 ?B)
}

#chain bmark76
{
($L hollow076 $R) + ($L hollow176 $R) +  ($L mark_ans76 $R) + ($L capt_ans76 ?A) + ($R capt76 A)
}

#chain data_mark77
{
($L output77 $R) + ($L rec77 ?A) + ($L tst_output77 A) + ($L res77 $R) + ($L incrementor77 $R) + ($L esc77 $R)
(?A tagf77 ?B)
}

#chain bmark77
{
($L hollow077 $R) + ($L hollow177 $R) +  ($L mark_ans77 $R) + ($L capt_ans77 ?A) + ($R capt77 A)
}

#chain data_mark78
{
($L output78 $R) + ($L rec78 ?A) + ($L tst_output78 A) + ($L res78 $R) + ($L incrementor78 $R) + ($L esc78 $R)
(?A tagf78 ?B)
}

#chain bmark78
{
($L hollow078 $R) + ($L hollow178 $R) +  ($L mark_ans78 $R) + ($L capt_ans78 ?A) + ($R capt78 A)
}

#chain data_mark79
{
($L output79 $R) + ($L rec79 ?A) + ($L tst_output79 A) + ($L res79 $R) + ($L incrementor79 $R) + ($L esc79 $R)
(?A tagf79 ?B)
}

#chain bmark79
{
($L hollow079 $R) + ($L hollow179 $R) +  ($L mark_ans79 $R) + ($L capt_ans79 ?A) + ($R capt79 A)
}

#chain data_mark80
{
($L output80 $R) + ($L rec80 ?A) + ($L tst_output80 A) + ($L res80 $R) + ($L incrementor80 $R) + ($L esc80 $R)
(?A tagf80 ?B)
}

#chain bmark80
{
($L hollow080 $R) + ($L hollow180 $R) +  ($L mark_ans80 $R) + ($L capt_ans80 ?A) + ($R capt80 A)
}

#chain data_mark81
{
($L output81 $R) + ($L rec81 ?A) + ($L tst_output81 A) + ($L res81 $R) + ($L incrementor81 $R) + ($L esc81 $R)
(?A tagf81 ?B)
}

#chain bmark81
{
($L hollow081 $R) + ($L hollow181 $R) +  ($L mark_ans81 $R) + ($L capt_ans81 ?A) + ($R capt81 A)
}

#chain data_mark82
{
($L output82 $R) + ($L rec82 ?A) + ($L tst_output82 A) + ($L res82 $R) + ($L incrementor82 $R) + ($L esc82 $R)
(?A tagf82 ?B)
}

#chain bmark82
{
($L hollow082 $R) + ($L hollow182 $R) +  ($L mark_ans82 $R) + ($L capt_ans82 ?A) + ($R capt82 A)
}

#chain data_mark83
{
($L output83 $R) + ($L rec83 ?A) + ($L tst_output83 A) + ($L res83 $R) + ($L incrementor83 $R) + ($L esc83 $R)
(?A tagf83 ?B)
}

#chain bmark83
{
($L hollow083 $R) + ($L hollow183 $R) +  ($L mark_ans83 $R) + ($L capt_ans83 ?A) + ($R capt83 A)
}

#chain data_mark84
{
($L output84 $R) + ($L rec84 ?A) + ($L tst_output84 A) + ($L res84 $R) + ($L incrementor84 $R) + ($L esc84 $R)
(?A tagf84 ?B)
}

#chain bmark84
{
($L hollow084 $R) + ($L hollow184 $R) +  ($L mark_ans84 $R) + ($L capt_ans84 ?A) + ($R capt84 A)
}

#chain data_mark85
{
($L output85 $R) + ($L rec85 ?A) + ($L tst_output85 A) + ($L res85 $R) + ($L incrementor85 $R) + ($L esc85 $R)
(?A tagf85 ?B)
}

#chain bmark85
{
($L hollow085 $R) + ($L hollow185 $R) +  ($L mark_ans85 $R) + ($L capt_ans85 ?A) + ($R capt85 A)
}

#chain data_mark86
{
($L output86 $R) + ($L rec86 ?A) + ($L tst_output86 A) + ($L res86 $R) + ($L incrementor86 $R) + ($L esc86 $R)
(?A tagf86 ?B)
}

#chain bmark86
{
($L hollow086 $R) + ($L hollow186 $R) +  ($L mark_ans86 $R) + ($L capt_ans86 ?A) + ($R capt86 A)
}

#chain data_mark87
{
($L output87 $R) + ($L rec87 ?A) + ($L tst_output87 A) + ($L res87 $R) + ($L incrementor87 $R) + ($L esc87 $R)
(?A tagf87 ?B)
}

#chain bmark87
{
($L hollow087 $R) + ($L hollow187 $R) +  ($L mark_ans87 $R) + ($L capt_ans87 ?A) + ($R capt87 A)
}

#chain data_mark88
{
($L output88 $R) + ($L rec88 ?A) + ($L tst_output88 A) + ($L res88 $R) + ($L incrementor88 $R) + ($L esc88 $R)
(?A tagf88 ?B)
}

#chain bmark88
{
($L hollow088 $R) + ($L hollow188 $R) +  ($L mark_ans88 $R) + ($L capt_ans88 ?A) + ($R capt88 A)
}

#chain data_mark89
{
($L output89 $R) + ($L rec89 ?A) + ($L tst_output89 A) + ($L res89 $R) + ($L incrementor89 $R) + ($L esc89 $R)
(?A tagf89 ?B)
}

#chain bmark89
{
($L hollow089 $R) + ($L hollow189 $R) +  ($L mark_ans89 $R) + ($L capt_ans89 ?A) + ($R capt89 A)
}

#chain data_mark90
{
($L output90 $R) + ($L rec90 ?A) + ($L tst_output90 A) + ($L res90 $R) + ($L incrementor90 $R) + ($L esc90 $R)
(?A tagf90 ?B)
}

#chain bmark90
{
($L hollow090 $R) + ($L hollow190 $R) +  ($L mark_ans90 $R) + ($L capt_ans90 ?A) + ($R capt90 A)
}

#chain data_mark91
{
($L output91 $R) + ($L rec91 ?A) + ($L tst_output91 A) + ($L res91 $R) + ($L incrementor91 $R) + ($L esc91 $R)
(?A tagf91 ?B)
}

#chain bmark91
{
($L hollow091 $R) + ($L hollow191 $R) +  ($L mark_ans91 $R) + ($L capt_ans91 ?A) + ($R capt91 A)
}

#chain data_mark92
{
($L output92 $R) + ($L rec92 ?A) + ($L tst_output92 A) + ($L res92 $R) + ($L incrementor92 $R) + ($L esc92 $R)
(?A tagf92 ?B)
}

#chain bmark92
{
($L hollow092 $R) + ($L hollow192 $R) +  ($L mark_ans92 $R) + ($L capt_ans92 ?A) + ($R capt92 A)
}

#chain data_mark93
{
($L output93 $R) + ($L rec93 ?A) + ($L tst_output93 A) + ($L res93 $R) + ($L incrementor93 $R) + ($L esc93 $R)
(?A tagf93 ?B)
}

#chain bmark93
{
($L hollow093 $R) + ($L hollow193 $R) +  ($L mark_ans93 $R) + ($L capt_ans93 ?A) + ($R capt93 A)
}

#chain data_mark94
{
($L output94 $R) + ($L rec94 ?A) + ($L tst_output94 A) + ($L res94 $R) + ($L incrementor94 $R) + ($L esc94 $R)
(?A tagf94 ?B)
}

#chain bmark94
{
($L hollow094 $R) + ($L hollow194 $R) +  ($L mark_ans94 $R) + ($L capt_ans94 ?A) + ($R capt94 A)
}

#chain data_mark95
{
($L output95 $R) + ($L rec95 ?A) + ($L tst_output95 A) + ($L res95 $R) + ($L incrementor95 $R) + ($L esc95 $R)
(?A tagf95 ?B)
}

#chain bmark95
{
($L hollow095 $R) + ($L hollow195 $R) +  ($L mark_ans95 $R) + ($L capt_ans95 ?A) + ($R capt95 A)
}

#chain data_mark96
{
($L output96 $R) + ($L rec96 ?A) + ($L tst_output96 A) + ($L res96 $R) + ($L incrementor96 $R) + ($L esc96 $R)
(?A tagf96 ?B)
}

#chain bmark96
{
($L hollow096 $R) + ($L hollow196 $R) +  ($L mark_ans96 $R) + ($L capt_ans96 ?A) + ($R capt96 A)
}

#chain data_mark97
{
($L output97 $R) + ($L rec97 ?A) + ($L tst_output97 A) + ($L res97 $R) + ($L incrementor97 $R) + ($L esc97 $R)
(?A tagf97 ?B)
}

#chain bmark97
{
($L hollow097 $R) + ($L hollow197 $R) +  ($L mark_ans97 $R) + ($L capt_ans97 ?A) + ($R capt97 A)
}

#chain data_mark98
{
($L output98 $R) + ($L rec98 ?A) + ($L tst_output98 A) + ($L res98 $R) + ($L incrementor98 $R) + ($L esc98 $R)
(?A tagf98 ?B)
}

#chain bmark98
{
($L hollow098 $R) + ($L hollow198 $R) +  ($L mark_ans98 $R) + ($L capt_ans98 ?A) + ($R capt98 A)
}

#chain data_mark99
{
($L output99 $R) + ($L rec99 ?A) + ($L tst_output99 A) + ($L res99 $R) + ($L incrementor99 $R) + ($L esc99 $R)
(?A tagf99 ?B)
}

#chain bmark99
{
($L hollow099 $R) + ($L hollow199 $R) +  ($L mark_ans99 $R) + ($L capt_ans99 ?A) + ($R capt99 A)
}

#scope universe
{
main0
main1
main2
main3
main4
main5
main6
main7
main8
main9
main10
main11
main12
main13
main14
main15
main16
main17
main18
main19
main20
main21
main22
main23
main24
main25
main26
main27
main28
main29
main30
main31
main32
main33
main34
main35
main36
main37
main38
main39
main40
main41
main42
main43
main44
main45
main46
main47
main48
main49
main50
main51
main52
main53
main54
main55
main56
main57
main58
main59
main60
main61
main62
main63
main64
main65
main66
main67
main68
main69
main70
main71
main72
main73
main74
main75
main76
main77
main78
main79
main80
main81
main82
main83
main84
main85
main86
main87
main88
main89
main90
main91
main92
main93
main94
main95
main96
main97
main98
main99
}

#scope main0
{
capt0
capt_ans0
data_center0
data00
data10
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
data01
data11
bmark1
}


#scope data_center1
{
fitness1
data_mark1
}

#scope main2
{
capt2
capt_ans2
data_center2
data02
data12
bmark2
}


#scope data_center2
{
fitness2
data_mark2
}

#scope main3
{
capt3
capt_ans3
data_center3
data03
data13
bmark3
}


#scope data_center3
{
fitness3
data_mark3
}

#scope main4
{
capt4
capt_ans4
data_center4
data04
data14
bmark4
}


#scope data_center4
{
fitness4
data_mark4
}

#scope main5
{
capt5
capt_ans5
data_center5
data05
data15
bmark5
}


#scope data_center5
{
fitness5
data_mark5
}

#scope main6
{
capt6
capt_ans6
data_center6
data06
data16
bmark6
}


#scope data_center6
{
fitness6
data_mark6
}

#scope main7
{
capt7
capt_ans7
data_center7
data07
data17
bmark7
}


#scope data_center7
{
fitness7
data_mark7
}

#scope main8
{
capt8
capt_ans8
data_center8
data08
data18
bmark8
}


#scope data_center8
{
fitness8
data_mark8
}

#scope main9
{
capt9
capt_ans9
data_center9
data09
data19
bmark9
}


#scope data_center9
{
fitness9
data_mark9
}

#scope main10
{
capt10
capt_ans10
data_center10
data010
data110
bmark10
}


#scope data_center10
{
fitness10
data_mark10
}

#scope main11
{
capt11
capt_ans11
data_center11
data011
data111
bmark11
}


#scope data_center11
{
fitness11
data_mark11
}

#scope main12
{
capt12
capt_ans12
data_center12
data012
data112
bmark12
}


#scope data_center12
{
fitness12
data_mark12
}

#scope main13
{
capt13
capt_ans13
data_center13
data013
data113
bmark13
}


#scope data_center13
{
fitness13
data_mark13
}

#scope main14
{
capt14
capt_ans14
data_center14
data014
data114
bmark14
}


#scope data_center14
{
fitness14
data_mark14
}

#scope main15
{
capt15
capt_ans15
data_center15
data015
data115
bmark15
}


#scope data_center15
{
fitness15
data_mark15
}

#scope main16
{
capt16
capt_ans16
data_center16
data016
data116
bmark16
}


#scope data_center16
{
fitness16
data_mark16
}

#scope main17
{
capt17
capt_ans17
data_center17
data017
data117
bmark17
}


#scope data_center17
{
fitness17
data_mark17
}

#scope main18
{
capt18
capt_ans18
data_center18
data018
data118
bmark18
}


#scope data_center18
{
fitness18
data_mark18
}

#scope main19
{
capt19
capt_ans19
data_center19
data019
data119
bmark19
}


#scope data_center19
{
fitness19
data_mark19
}

#scope main20
{
capt20
capt_ans20
data_center20
data020
data120
bmark20
}


#scope data_center20
{
fitness20
data_mark20
}

#scope main21
{
capt21
capt_ans21
data_center21
data021
data121
bmark21
}


#scope data_center21
{
fitness21
data_mark21
}

#scope main22
{
capt22
capt_ans22
data_center22
data022
data122
bmark22
}


#scope data_center22
{
fitness22
data_mark22
}

#scope main23
{
capt23
capt_ans23
data_center23
data023
data123
bmark23
}


#scope data_center23
{
fitness23
data_mark23
}

#scope main24
{
capt24
capt_ans24
data_center24
data024
data124
bmark24
}


#scope data_center24
{
fitness24
data_mark24
}

#scope main25
{
capt25
capt_ans25
data_center25
data025
data125
bmark25
}


#scope data_center25
{
fitness25
data_mark25
}

#scope main26
{
capt26
capt_ans26
data_center26
data026
data126
bmark26
}


#scope data_center26
{
fitness26
data_mark26
}

#scope main27
{
capt27
capt_ans27
data_center27
data027
data127
bmark27
}


#scope data_center27
{
fitness27
data_mark27
}

#scope main28
{
capt28
capt_ans28
data_center28
data028
data128
bmark28
}


#scope data_center28
{
fitness28
data_mark28
}

#scope main29
{
capt29
capt_ans29
data_center29
data029
data129
bmark29
}


#scope data_center29
{
fitness29
data_mark29
}

#scope main30
{
capt30
capt_ans30
data_center30
data030
data130
bmark30
}


#scope data_center30
{
fitness30
data_mark30
}

#scope main31
{
capt31
capt_ans31
data_center31
data031
data131
bmark31
}


#scope data_center31
{
fitness31
data_mark31
}

#scope main32
{
capt32
capt_ans32
data_center32
data032
data132
bmark32
}


#scope data_center32
{
fitness32
data_mark32
}

#scope main33
{
capt33
capt_ans33
data_center33
data033
data133
bmark33
}


#scope data_center33
{
fitness33
data_mark33
}

#scope main34
{
capt34
capt_ans34
data_center34
data034
data134
bmark34
}


#scope data_center34
{
fitness34
data_mark34
}

#scope main35
{
capt35
capt_ans35
data_center35
data035
data135
bmark35
}


#scope data_center35
{
fitness35
data_mark35
}

#scope main36
{
capt36
capt_ans36
data_center36
data036
data136
bmark36
}


#scope data_center36
{
fitness36
data_mark36
}

#scope main37
{
capt37
capt_ans37
data_center37
data037
data137
bmark37
}


#scope data_center37
{
fitness37
data_mark37
}

#scope main38
{
capt38
capt_ans38
data_center38
data038
data138
bmark38
}


#scope data_center38
{
fitness38
data_mark38
}

#scope main39
{
capt39
capt_ans39
data_center39
data039
data139
bmark39
}


#scope data_center39
{
fitness39
data_mark39
}

#scope main40
{
capt40
capt_ans40
data_center40
data040
data140
bmark40
}


#scope data_center40
{
fitness40
data_mark40
}

#scope main41
{
capt41
capt_ans41
data_center41
data041
data141
bmark41
}


#scope data_center41
{
fitness41
data_mark41
}

#scope main42
{
capt42
capt_ans42
data_center42
data042
data142
bmark42
}


#scope data_center42
{
fitness42
data_mark42
}

#scope main43
{
capt43
capt_ans43
data_center43
data043
data143
bmark43
}


#scope data_center43
{
fitness43
data_mark43
}

#scope main44
{
capt44
capt_ans44
data_center44
data044
data144
bmark44
}


#scope data_center44
{
fitness44
data_mark44
}

#scope main45
{
capt45
capt_ans45
data_center45
data045
data145
bmark45
}


#scope data_center45
{
fitness45
data_mark45
}

#scope main46
{
capt46
capt_ans46
data_center46
data046
data146
bmark46
}


#scope data_center46
{
fitness46
data_mark46
}

#scope main47
{
capt47
capt_ans47
data_center47
data047
data147
bmark47
}


#scope data_center47
{
fitness47
data_mark47
}

#scope main48
{
capt48
capt_ans48
data_center48
data048
data148
bmark48
}


#scope data_center48
{
fitness48
data_mark48
}

#scope main49
{
capt49
capt_ans49
data_center49
data049
data149
bmark49
}


#scope data_center49
{
fitness49
data_mark49
}

#scope main50
{
capt50
capt_ans50
data_center50
data050
data150
bmark50
}


#scope data_center50
{
fitness50
data_mark50
}

#scope main51
{
capt51
capt_ans51
data_center51
data051
data151
bmark51
}


#scope data_center51
{
fitness51
data_mark51
}

#scope main52
{
capt52
capt_ans52
data_center52
data052
data152
bmark52
}


#scope data_center52
{
fitness52
data_mark52
}

#scope main53
{
capt53
capt_ans53
data_center53
data053
data153
bmark53
}


#scope data_center53
{
fitness53
data_mark53
}

#scope main54
{
capt54
capt_ans54
data_center54
data054
data154
bmark54
}


#scope data_center54
{
fitness54
data_mark54
}

#scope main55
{
capt55
capt_ans55
data_center55
data055
data155
bmark55
}


#scope data_center55
{
fitness55
data_mark55
}

#scope main56
{
capt56
capt_ans56
data_center56
data056
data156
bmark56
}


#scope data_center56
{
fitness56
data_mark56
}

#scope main57
{
capt57
capt_ans57
data_center57
data057
data157
bmark57
}


#scope data_center57
{
fitness57
data_mark57
}

#scope main58
{
capt58
capt_ans58
data_center58
data058
data158
bmark58
}


#scope data_center58
{
fitness58
data_mark58
}

#scope main59
{
capt59
capt_ans59
data_center59
data059
data159
bmark59
}


#scope data_center59
{
fitness59
data_mark59
}

#scope main60
{
capt60
capt_ans60
data_center60
data060
data160
bmark60
}


#scope data_center60
{
fitness60
data_mark60
}

#scope main61
{
capt61
capt_ans61
data_center61
data061
data161
bmark61
}


#scope data_center61
{
fitness61
data_mark61
}

#scope main62
{
capt62
capt_ans62
data_center62
data062
data162
bmark62
}


#scope data_center62
{
fitness62
data_mark62
}

#scope main63
{
capt63
capt_ans63
data_center63
data063
data163
bmark63
}


#scope data_center63
{
fitness63
data_mark63
}

#scope main64
{
capt64
capt_ans64
data_center64
data064
data164
bmark64
}


#scope data_center64
{
fitness64
data_mark64
}

#scope main65
{
capt65
capt_ans65
data_center65
data065
data165
bmark65
}


#scope data_center65
{
fitness65
data_mark65
}

#scope main66
{
capt66
capt_ans66
data_center66
data066
data166
bmark66
}


#scope data_center66
{
fitness66
data_mark66
}

#scope main67
{
capt67
capt_ans67
data_center67
data067
data167
bmark67
}


#scope data_center67
{
fitness67
data_mark67
}

#scope main68
{
capt68
capt_ans68
data_center68
data068
data168
bmark68
}


#scope data_center68
{
fitness68
data_mark68
}

#scope main69
{
capt69
capt_ans69
data_center69
data069
data169
bmark69
}


#scope data_center69
{
fitness69
data_mark69
}

#scope main70
{
capt70
capt_ans70
data_center70
data070
data170
bmark70
}


#scope data_center70
{
fitness70
data_mark70
}

#scope main71
{
capt71
capt_ans71
data_center71
data071
data171
bmark71
}


#scope data_center71
{
fitness71
data_mark71
}

#scope main72
{
capt72
capt_ans72
data_center72
data072
data172
bmark72
}


#scope data_center72
{
fitness72
data_mark72
}

#scope main73
{
capt73
capt_ans73
data_center73
data073
data173
bmark73
}


#scope data_center73
{
fitness73
data_mark73
}

#scope main74
{
capt74
capt_ans74
data_center74
data074
data174
bmark74
}


#scope data_center74
{
fitness74
data_mark74
}

#scope main75
{
capt75
capt_ans75
data_center75
data075
data175
bmark75
}


#scope data_center75
{
fitness75
data_mark75
}

#scope main76
{
capt76
capt_ans76
data_center76
data076
data176
bmark76
}


#scope data_center76
{
fitness76
data_mark76
}

#scope main77
{
capt77
capt_ans77
data_center77
data077
data177
bmark77
}


#scope data_center77
{
fitness77
data_mark77
}

#scope main78
{
capt78
capt_ans78
data_center78
data078
data178
bmark78
}


#scope data_center78
{
fitness78
data_mark78
}

#scope main79
{
capt79
capt_ans79
data_center79
data079
data179
bmark79
}


#scope data_center79
{
fitness79
data_mark79
}

#scope main80
{
capt80
capt_ans80
data_center80
data080
data180
bmark80
}


#scope data_center80
{
fitness80
data_mark80
}

#scope main81
{
capt81
capt_ans81
data_center81
data081
data181
bmark81
}


#scope data_center81
{
fitness81
data_mark81
}

#scope main82
{
capt82
capt_ans82
data_center82
data082
data182
bmark82
}


#scope data_center82
{
fitness82
data_mark82
}

#scope main83
{
capt83
capt_ans83
data_center83
data083
data183
bmark83
}


#scope data_center83
{
fitness83
data_mark83
}

#scope main84
{
capt84
capt_ans84
data_center84
data084
data184
bmark84
}


#scope data_center84
{
fitness84
data_mark84
}

#scope main85
{
capt85
capt_ans85
data_center85
data085
data185
bmark85
}


#scope data_center85
{
fitness85
data_mark85
}

#scope main86
{
capt86
capt_ans86
data_center86
data086
data186
bmark86
}


#scope data_center86
{
fitness86
data_mark86
}

#scope main87
{
capt87
capt_ans87
data_center87
data087
data187
bmark87
}


#scope data_center87
{
fitness87
data_mark87
}

#scope main88
{
capt88
capt_ans88
data_center88
data088
data188
bmark88
}


#scope data_center88
{
fitness88
data_mark88
}

#scope main89
{
capt89
capt_ans89
data_center89
data089
data189
bmark89
}


#scope data_center89
{
fitness89
data_mark89
}

#scope main90
{
capt90
capt_ans90
data_center90
data090
data190
bmark90
}


#scope data_center90
{
fitness90
data_mark90
}

#scope main91
{
capt91
capt_ans91
data_center91
data091
data191
bmark91
}


#scope data_center91
{
fitness91
data_mark91
}

#scope main92
{
capt92
capt_ans92
data_center92
data092
data192
bmark92
}


#scope data_center92
{
fitness92
data_mark92
}

#scope main93
{
capt93
capt_ans93
data_center93
data093
data193
bmark93
}


#scope data_center93
{
fitness93
data_mark93
}

#scope main94
{
capt94
capt_ans94
data_center94
data094
data194
bmark94
}


#scope data_center94
{
fitness94
data_mark94
}

#scope main95
{
capt95
capt_ans95
data_center95
data095
data195
bmark95
}


#scope data_center95
{
fitness95
data_mark95
}

#scope main96
{
capt96
capt_ans96
data_center96
data096
data196
bmark96
}


#scope data_center96
{
fitness96
data_mark96
}

#scope main97
{
capt97
capt_ans97
data_center97
data097
data197
bmark97
}


#scope data_center97
{
fitness97
data_mark97
}

#scope main98
{
capt98
capt_ans98
data_center98
data098
data198
bmark98
}


#scope data_center98
{
fitness98
data_mark98
}

#scope main99
{
capt99
capt_ans99
data_center99
data099
data199
bmark99
}


#scope data_center99
{
fitness99
data_mark99
}

#systemic end