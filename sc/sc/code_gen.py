import sys
def c_dmark(x):
    return """#chain data_mark{x}
{{
($L output{x} $R) + ($L rec{x} ?A) + ($L tst_output{x} A) + ($L res{x} $R) + ($L incrementor{x} $R) + ($L esc{x} $R)
(?A tagf{x} ?B)
}}
""".format(x=x)

def c_bmark(x):
    return"""#chain bmark{x}
{{
($L hollow0{x} $R) + ($L hollow1{x} $R) +  ($L mark_ans{x} $R) + ($L capt_ans{x} ?A) + ($R capt{x} A)
}}
""".format(x=x)
def s_uni(iterations):
    """special case yo"""
    s = ["main{}".format(x) for x in range(iterations)]
    return """#scope universe
{{
{}
}}
""".format("\n".join(s))

def s_main(x):
    return """#scope main{x}
{{
capt{x}
capt_ans{x}
data_center{x}
data0{x}
data1{x}
bmark{x}
}}
""".format(x=x)

def s_datac(x):
    return """
#scope data_center{x}
{{
fitness{x}
data_mark{x}
}}
""".format(x=x)

def header(x):
    x = x - 1 #recall that array defined systems are 0 indexed
    return """#systemic start
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
[0:{x}]main (%d0 %d0 %d0)
//1
//where the data gets recorded & recycled
[0:{x}]data_center (bin_label %d0 %d0)
//2
[0:{x}]data0 (num %d0 %d1)
//3
[0:{x}]data1 (num %d0 %d1)
//4
[0:{x}]fitness (%d19 %d0 %d0)

//Main scope
//5
[0:{x}]capt_ans ([%d20 zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//6
[0:{x}]capt ([three zero dontcare] CAPTURE(0,0) [bin_label zero dontcare])
//7
[0:{x}]esc ([num zero dontcare] ESCAPE_BOTH(0,0) [num zero dontcare])
//9
[0:{x}]incrementor ([num zero dontcare] INCREMENTOR(0,0) [num zero dontcare])
//marks the number three on left schema
//10
[0:{x}]bmark ([num zero dontcare] BITMARK_INIT(0,0) [num zero dontcare])
//data-center scope
//these two are just used for debugging
//the answer is in 20 -> 21. the  x value is in 3 -> 4
[0:{x}]output ([%d21 zero dontcare] PRINT(0,0) [%d4 zero dontcare])
[0:{x}]tst_output ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//14
[0:{x}]data_mark ([%d20 zero dontcare] BITMARKP1(0,0) [three zero dontcare])
//15
[0:{x}]rec ([%d21 zero dontcare] RECORDER(0,0) [%d19 zero dontcare])
//16
[0:{x}]res ([%d21 zero dontcare] RESETTER(0,0) [%d4 zero dontcare])
//17
[0:{x}]tagf ([%d19 zero dontcare] TAG_FITNESS(0,0) [dontcare dontcare dontcare])
[0:{x}]mark_ans ([three zero dontcare] MARK_ANSWER(0,0) [dontcare dontcare dontcare])
universe (%d0 %d0 %d0)
//read_main ([dontcare zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//blah (%d0 %d0 %d0)
[0:{x}]hollow0 ([three zero dontcare] NOP(0,0) [three zero dontcare])
[0:{x}]hollow1 ([three zero dontcare] NOP(0,0) [three zero dontcare])

///////////    Begin chain definitions /////////

""".format(x=x)
if len(sys.argv) is 1:
    print "do remember to pass me an arg for how many reps youd like"
reps = int(sys.argv[1])

with open('GP_autogen.sc', 'w') as f:
    f.write(header(reps))

    for rep in range(reps):
        f.write(c_dmark(rep))
        f.write("\n")
        f.write(c_bmark(rep))
        f.write("\n")


    f.write(s_uni(reps))
    f.write("\n")
    for rep in range(reps):
        f.write(s_main(rep))
        f.write("\n")
        f.write(s_datac(rep))
        f.write("\n")
    f.write("#systemic end")