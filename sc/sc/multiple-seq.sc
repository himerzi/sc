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



// define some useful labels
#label first                %b10101010000000000000000000000000
#label bmarkid              %b00010100000000000000000000000000
#label second               %b01001001000000000000000000000000
#label mult-type                  %b10000000000000000000000000000000
#label zero                 %b00000000000000000000000000000000
#label one                  %b10000000000000000000000000000000
#label two                  %b01000000000000000000000000000000
#label three                %b11000000000000000000000000000000
#label four                 %b00100000000000000000000000000000
#label sum-type               %b00110000000000000000000000000000
#label zerotofour           %b???00000000000000000000000000000
#label dontcare             %b????????????????????????????????
#label onetwoeight          %b???????1????????????????????????
#label nooutput             %b????????0???????????????????????
#label notone               %b0???????????????????????????????

// and the program begins here:
barrenland (%d0 %d0 %d0)
solution (%d0 %d0 %d0)
main-wrapper (%d0 %d0 %d0)
operation0 (%d0 %d0 %d0)
operation1 (%d0 %d0 %d0)
chain-head0 (%d0 %d0 %d0)
chain-head1 (%d0 %d0 %d0)
[0:1]dummy-system (sum-type %d0 three)

//first set of data. for computing (11*13)+7 10010110
data10 (mult-type %d0 %d11)
data20 (mult-type %d0 %d13)
//sum-type attaches to sum schema
data30 (sum-type %d0 %d7)
//second set of data. for computing (7*13)+11 1100110
data11 (mult-type %d0 %d7)
data21 (mult-type %d0 %d13)
//11 attaches to sum schema
data31 (sum-type %d0 %d11)


//has bitmark in its kernel...matches dummy system on teh right.
esc1 ([dontcare bmarkid dontcare] ESCAPE(0,0) [sum-type %d0 three])
esc ([three zero dontcare] ESCAPE(0,0) [dontcare zero dontcare])
[0:1]sum ([three zero dontcare] ADD(0,0) [sum-type zero dontcare])
[0:1]times ([three zero dontcare] MULT(0,0) [three zero dontcare])
output  ([three zero dontcare] PRINT(0,0) [dontcare zero dontcare])
//marks the number three on left schema
[0:1]bmark ([mult-type zero dontcare] BITMARK(0,0) [mult-type zero dontcare])


//Chain array. [0:N]chainname . Each chain would be named and stored as chainameN.
#chain [0:1]bmark
{
//note that sum here doesn't use $R, MULT sets $R to 1 after operating. So we need to fish out a new system from the scope
//to add to the result of MULT (stored in $L), this new system is ?A. + ($L sum ?A) + ($L output A). Only $l gets escaped
//chain-head geats escaped into main where it operates, then a solution gets escaped into solution, where it is printed
//then a solution is escaped into barrenland where it rests and whithers.
($L times0 $R)  +($L sum0 ?A) + ($L esc A)
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
main-wrapper
}
#scope main-wrapper
{
operation0
operation1
output
}

#scope operation0
{
data10
data20
data30
times0
sum0
chain-head0
}
#scope operation1
{
data11
data21
data31
times0
sum0
chain-head1
}
#scope chain-head0
{
bmark0
dummy-system0
esc1
}
#scope chain-head1
{
bmark1
dummy-system1
esc1
}
#systemic end