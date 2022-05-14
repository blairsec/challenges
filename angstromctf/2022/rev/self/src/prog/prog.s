; vim: ft=nasm

%assign prog_start 0x800

_start:


%include "src/prog/assembler.mac"

imm 0, 0x000
imm 1, 0x001
imm 3, 0x1ff ; mem

; stack at 0x0ff -> 0x1ff, grows downwards
; stack pointer is stored at 0x02

; memory is at 0x1ff -> 0x6ff

; registers r0 -> r15
; r0 / r1 -> used for intermediate operations, quick store


imm (0x10 + 0), 0x000
%assign r0 (0x10 + 0)
imm (0x10 + 1), 0x000
%assign r1 (0x10 + 1)
imm (0x10 + 2), 0x000
%assign r2 (0x10 + 2)
imm (0x10 + 3), 0x000
%assign r3 (0x10 + 3)
imm (0x10 + 4), 0x000
%assign r4 (0x10 + 4)
imm (0x10 + 5), 0x000
%assign r5 (0x10 + 5)
imm (0x10 + 6), 0x000
%assign r6 (0x10 + 6)
imm (0x10 + 7), 0x000
%assign r7 (0x10 + 7)
imm (0x10 + 8), 0x000
%assign r8 (0x10 + 8)
imm (0x10 + 9), 0x000
%assign r9 (0x10 + 9)
imm (0x10 + 10), 0x000
%assign r10 (0x10 + 10)
imm (0x10 + 11), 0x000
%assign r11 (0x10 + 11)
imm (0x10 + 12), 0x000
%assign r12 (0x10 + 12)
imm (0x10 + 13), 0x000
%assign r13 (0x10 + 13)
imm (0x10 + 14), 0x000
%assign r14 (0x10 + 14)
imm (0x10 + 15), 0x000
%assign r15 (0x10 + 15)

%assign one 1
%assign zero 0


%macro mult 2
    mov r0, %2
    jiz %2, 4
    sub %2, one
    add r1, %1
    jiz 0, -3
    mov %1, r1
    mov r1, zero 
    mov %2, r0
    mov r0, zero
%endmacro
%define mul mult


imm r10, 0xfff

nest_1:
    jiz r10, ((end_1 - nest_1) >> 2)
    sub r10, one


imm r9, 0xfff

nest_2:
    jiz r9, ((end_2 - nest_2) >> 2)
    sub r9, one


imm r8, 0xfff

nest_3:
    jiz r8, ((end_3 - nest_3) >> 2)
    sub r8, one



imm r7, 0xfff

nest_4:
    jiz r7, ((end_4 - nest_4) >> 2)
    sub r7, one


; r15 -> loop variable
; r14 -> loop over the first immediate for xor
; r13 -> loop over the second immediate for xor
; r12 -> constant for multiplying r14 to the right value
; r11 -> subtractor for r14

imm r15, 0x74

imm r14, prog_start + ((prog_enc - _start) >> 2) + 0x74 
imm r13, prog_start + ((prog_key - _start) >> 2) + 0x74
imm r12, 0x010
imm r11, 0x001

mul r14, r12
mul r14, r12
mul r14, r12
mul r11, r12
mul r11, r12
mul r11, r12

decryption_loop:
    jiz r15, ((end_decryption - decryption_loop) >> 2)
    sub r15, one
    sub r14, r11
    sub r13, one


    add prog_start + ((xor_ins - _start) >> 2), r14
    add prog_start + ((xor_ins - _start) >> 2), r13
    xor_ins:
        xor 000, 000

    sub prog_start + ((xor_ins - _start) >> 2), r14
    sub prog_start + ((xor_ins - _start) >> 2), r13

    jmp prog_start + ((decryption_loop - _start) >> 2) - 1
end_decryption:

    jmp prog_start + ((nest_4 - _start) >> 2) - 1
end_4:

    jmp prog_start + ((nest_3 - _start) >> 2) - 1
end_3:

    jmp prog_start + ((nest_2 - _start) >> 2) - 1
end_2:

    jmp prog_start + ((nest_1 - _start) >> 2) - 1
end_1:


print "tst"

%include "src/prog/enc.s"
