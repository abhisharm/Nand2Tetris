// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.
@sum // sum = 0
M=0
@R0
D=M
@r0cnt //r0cnt = R0
M=D
(LOOP) // main loop
@r0cnt
D=M
@POSITIVE // if r0cnt > 0
D;JGT
@NEGATIVE // if r0cnt < 0
D;JLT
// r0cnt=0
@sum
D=M
@R2
M=D // R2=sum
(END)
@END // end loop
0;JMP

(POSITIVE)
@R1
D=M
@sum
M=M+D
@r0cnt
M=M-1 // decrement r0cnt by 1
@LOOP
0;JMP

(NEGATIVE)
@R1
D=M
@sum
M=M-D
@r0cnt // increment r0cnt by 1
M=M+1
@LOOP
0;JMP
