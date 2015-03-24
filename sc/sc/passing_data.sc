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
#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????
#label nooutput             %b????????0???????????????????????
#label notone               %b0???????????????????????????????

// and the program begins here:
barrenland (%d0 %d0 %d0)
solution (%d0 %d0 %d0)
main (%d0 %d0 %d0)
chain-head (%d0 %d0 %d0)
dummy-system (twelve %d0 three)

data1 (num %d0 %d11)
data2 (num %d0 %d13)
//twelve attaches to sum schema
data3 (twelve %d0 %d7)
//has bitmark in its kernel...matches dummy system on teh right.
esc1 ([dontcare bmarkid dontcare] ESCAPE(0,0) [twelve %d0 three])
esc ([three zero dontcare] ESCAPE(0,0) [dontcare zero dontcare])
sum ([three zero dontcare] ADD(0,0) [twelve zero dontcare])
times ([three zero dontcare] MULT(0,0) [three zero dontcare])
output  ([three zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//marks the number three on left schema
bmark ([num zero dontcare] BITMARK(0,0) [num zero dontcare])



#chain bmark
{
//note that sum here doesn't use $R, MULT sets $R to 1 after operating. So we need to fish out a new system from the scope
//to add to the result of MULT (stored in $L), this new system is ?A. + ($L sum ?A) + ($L output A). Only $l gets escaped
//, i think should do (11*13)+7 = 10010110
//chain-head geats escaped into main where it operates, then a solution gets escaped into solution, where it is printed
//then a solution is escaped into barrenland where it rests and whithers.
($L times $R)  +($L sum ?A) + ($L esc A)
//R should be trashed, ?A should be trashed. Need a way of marking shit that needs to be trashed.
}
#chain output
{
//so that stuff only gets printed once
($L esc $R)
}

// set up the scopes
//barrenland this is where systems go to rest (and not be printed)
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
data1 //1
data2 //13
data3 //7
times
sum
chain-head
}
#scope chain-head
{
bmark
dummy-system
esc1
}

#systemic end