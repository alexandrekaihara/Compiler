LOAD SYSTEMNUM1
STORE result
LOAD SYSTEMNUM0
STORE n
INPUT n
LOAD n
STORE AUX
LOAD SYSTEMNUM0
SUB AUX
JMPZ endcomp0
JMPP endcomp0
LOAD ONE
STORE RESULT0
JMP endcomp1
endcomp0:
LOAD ZERO
STORE RESULT0
endcomp1:
LOAD RESULT0
JMPZ endif0
repeat0:
LOAD SYSTEMNUM1
STORE AUX
LOAD n
SUB AUX
JMPZ endcomp2
JMPP endcomp2
LOAD ONE
STORE RESULT1
JMP endcomp3
endcomp2:
LOAD ZERO
STORE RESULT1
endcomp3:
LOAD RESULT1
SUB ONE
JMPZ endrepeat0
LOAD n
STORE AUX
LOAD result
MULT AUX
STORE RESULT3
LOAD RESULT3
STORE result
LOAD SYSTEMNUMNEG1
STORE AUX
LOAD n
ADD AUX
STORE RESULT2
LOAD RESULT2
STORE n
JMP repeat0
endrepeat0:
LOAD result
STORE AUX
OUTPUT AUX
endif0:
STOP
ZERO: CONST 0
ONE: CONST 1
AUX: SPACE
SYSTEMNUM0: CONST 0
RESULT0: SPACE
SYSTEMNUM1: CONST 1
RESULT1: SPACE
SYSTEMNUMNEG1: CONST -1
RESULT2: SPACE
n: SPACE
RESULT3: SPACE
result: SPACE