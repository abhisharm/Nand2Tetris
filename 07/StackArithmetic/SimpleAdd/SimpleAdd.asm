// push constant 7
@7
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 8
@8
D=A
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
// end of program
(SimpleAdd.END)
@SimpleAdd.END
0;JMP
// end of program
(SimpleAdd.END)
@SimpleAdd.END
0;JMP
