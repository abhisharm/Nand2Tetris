// write function SimpleFunction.test 2
(SimpleFunction.test)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
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
// push local 1
@LCL
D=M
@1
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
// not
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@R13
D=!M
@R13
M=D
@R13
D=M
@SP
A=M
M=D
@SP
M=M+1
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
// push argument 1
@ARG
D=M
@1
A=A+D
D=M
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
// write return
@LCL
D=M
@R13
M=D
@R13
D=M
@5
D=D-A
A=D
D=M
@R14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@SP
M=M-1
@ARG
D=M
@1
D=D+A
@SP
M=D
@R13
D=M
@1
A=D-A
D=M
@THAT
M=D
@R13
D=M
@2
A=D-A
D=M
@THIS
M=D
@R13
D=M
@3
A=D-A
D=M
@ARG
M=D
@R13
D=M
@4
A=D-A
D=M
@LCL
M=D
@R14
A=M;JMP
// end of program
(SimpleFunction.END)
@SimpleFunction.END
0;JMP