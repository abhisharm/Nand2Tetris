// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
(LOOP)
@KBD
D=M
@WHITEN
D;JEQ // if keyboard was not touched, go to whiten
@DARKEN
D;JNE // if keyboard was touched, go to darken
@LOOP // should never reach this code
0;JMP

(WHITEN)
@color
M=0 // color is set to white (0)
@COLOR
0;JMP // color screen

(DARKEN)
@color
M=-1 // color set to black (-1)
@COLOR
0;JMP // color screen

(COLOR) // colors the screen whatever color is in @color
@8191 // 8k-1
D=A
@rowcounter // rowcounter=8k-1
M=D
(COLORLOOP)
@rowcounter
D=M
@LOOP
D;JLT // return to main loop if rowcounter<0
@SCREEN
D=A
@rowcounter
D=D+M // store address of current row
@rowaddress
M=D // @rowaddress = address of current row
@color
D=M
@rowaddress
M=D // color current row with @color
@rowcounter
M=M-1 // decrement row
@COLORLOOP
0;JMP // continue coloring the screen

