// Draw a black rectangle at the center of the 256*512 screen
// Allow arrow keys to be pressed to move the location of the rectangle
// by 10 pixels.

// Initialization of 'variables'
// set center to (128, 256) (middle of screen)
@128
D=A
@cr
M=D // cr=128 (row-pointer of rectangle)
@256
D=A
@cc
M=D // cc=256 (column-pointer of rectangle)
@10
D=A
@move_size
M=D //move_size = 10
@10
D=A
@side_length
M=D // side_length = 10

// Main program loop:
// First whitens the screen (lazy, I know),
// then draws the rectangle, 
// then gets keyboard input to 
// check if the center needs to be 
// moved
(MAIN_LOOP)
// whiten screen
@8191
D=A
@whiten_counter
M=D // whiten_counter = 8191
@SCREEN
D=A
@whiten_pointer
M=D
(WHITEN_LOOP)
@whiten_counter
D=M
@END_WHITEN
D;JLT // if whiten_counter < 0, end
@SCREEN
D=A+D
A=D
M=0 // set screen+whiten_counter to 0 (whiten)
@whiten_counter
M=M-1 // whiten_counter -= 1
@WHITEN_LOOP
0;JMP // loop
(END_WHITEN)
// Draw the rectangle first
// start-point: cr*256/16 + cc/16
@16
D=A
@a
M=D // a = 16
@cr
D=M
@b
M=D // b = cr
@MULTIPLY
0;JMP
(RETURN_MULT)
@c
D=M
@start_point
M=D // start_point = cr * 16
@cc
D=M
@numerator
M=D // numerator = cc
@16
D=A
@denominator
M=D //denominator = 16
@DIVIDE
0;JMP
(RETURN_DIV)
@div_result
D=M
@start_point
M=M+D // start_point = cr * 16 + cc / 16
@side_length 
D=M
@row_counter
M=D // row_counter=side_length
(DRAW_LOOP) // draw the rectangle, row-by-row
@row_counter
D=M
@END_DRAW
D;JEQ // if row_counter = 0, jump to end of draw loop
@SCREEN
D=A
@start_point
D=M+D 
@pointer
M=D // pointer = start_point + @SCREEN
@1023 // TODO: make this general
D=A // D = 10 1's (side_length = 10)
@pointer
A=M // set address to pointer
M=D // draw line
@32
D=A
@start_point
M=M+D // start_point += 512/16 (one row down)
@row_counter
M=M-1 // row_counter -= 1
@DRAW_LOOP
0;JMP // loop
(END_DRAW)
@KBD // Getting keyboard input
D=M
@move // move=KBD
M=D
@130
D=A
@move
D=M-D
@LEFT
D;JEQ // if move=130, jump to left
@131
D=A
@move
D=M-D
@UP
D;JEQ // if move=131, jump up
@132
D=A
@move
D=M-D
@RIGHT
D;JEQ // if move=132, jump right
@133
D=A
@move
D=M-D
@DOWN
D;JEQ // if move=133, jump down 
@MAIN_LOOP
0;JMP // return to loop otherwise

(LEFT)
@move_size
D=M
@cc
D=M-D
@MAIN_LOOP
D;JLT // return if the result would make cc<0
@cc
M=D // cc = cc - move_size
@MAIN_LOOP
0;JMP // return to loop

(DOWN)
@move_size
D=M
@cr
D=M+D
@256
D=A-D
@MAIN_LOOP
D;JLT // return if the result would make cr > 256
@move_size
D=M
@cr
M=M+D // cr = cr + move_size
@MAIN_LOOP
0;JMP // return to loop

(RIGHT)
@move_size
D=M
@cc
D=M+D
@512
D=A-D
@MAIN_LOOP
D;JLT // return if the result would make cc > 512
@move_size
D=M
@cc
M=M+D // cc = cc + move_size
@MAIN_LOOP
0;JMP // return to loop

(UP)
@move_size
D=M
@cr
D=M-D
@MAIN_LOOP
D;JLT // return if the result would make cr<0
@move_size
D=M
@cr
M=M-D // cr = cr - move_size
@MAIN_LOOP
0;JMP // return to loop

(DIVIDE) // implements positive integer division of
           // @numerator and @denominator and stores the
           // result in @div_result
           // TODO: add overflow
@numerator
D=M
@running_sum
M=D // running_sum=numerator
@div_result
M=0
(DIV_LOOP) // keep on subtracting until running_sum <= 0
@running_sum
D=M
@RETURN_DIV
D;JEQ // return if running_sum == 0
@RETURN_RESULT_MINUS_1
D;JLT // set div_result-=1 & return if running_sum < 0
@denominator
D=M
@running_sum
M=M-D // running_sum -= denominator
@div_result
M=M+1 // div_result += 1
@DIV_LOOP
0;JMP // return to loop
(RETURN_RESULT_MINUS_1)
@div_result
M=M-1 // result-=1
@RETURN_DIV
0;JMP // return

(MULTIPLY) // multiply a and b and leave in mult_result 
@mult_result // sum = 0
M=0
@a
D=M
@cnt //cnt = a
M=D
(MULT_LOOP) // main loop
@cnt
D=M
@POSITIVE // if cnt > 0
D;JGT
// negative case omitted
@mult_result // cnt=0
D=M
@c
M=D // c=mult_result
@RETURN_MULT // end loop
0;JMP

(POSITIVE)
@b
D=M
@mult_result
M=M+D
@cnt
M=M-1 // decrement cnt by 1
@MULT_LOOP
0;JMP

