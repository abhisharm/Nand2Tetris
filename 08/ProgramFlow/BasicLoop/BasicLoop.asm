// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop local 0
@LCL
D=M
@0
D=D+A
@R15
M=D
@SP
A=M-1
D=M
@R15
A=M
M=D
@SP
M=M-1
// write_label LOOP_START
(LOOP_START)
// push argument 0
@ARG
D=M
@0
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push local 0
@LCL
D=M
@0
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@SP
A=M-1
D=M
@R14
M=D
@SP
M=M-1
@R14
D=M
@R13
D=D+M
@R14
M=D
@R14
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop local 0
@LCL
D=M
@0
D=D+A
@R15
M=D
@SP
A=M-1
D=M
@R15
A=M
M=D
@SP
M=M-1
// push argument 0
@ARG
D=M
@0
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@SP
A=M-1
D=M
@R14
M=D
@SP
M=M-1
@R14
D=M
@R13
D=D-M
@R14
M=D
@R14
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop argument 0
@ARG
D=M
@0
D=D+A
@R15
M=D
@SP
A=M-1
D=M
@R15
A=M
M=D
@SP
M=M-1
// push argument 0
@ARG
D=M
@0
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// write_if LOOP_START
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@R13
D=M;
@LOOP_START
D;JNE
// push local 0
@LCL
D=M
@0
A=A+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// end of program
(BasicLoop.END)
@BasicLoop.END
0;JMP
