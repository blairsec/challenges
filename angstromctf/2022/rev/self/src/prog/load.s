
; vim: ft=nasm

%assign prog_start 0x869


_start:

%include "src/prog/assembler.mac"



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

imm 0, 0x000
imm 1, 0x001
imm 3, 0x1ff

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




; vim: ft=nasm
imm 511, 10
imm 512, 125
imm 513, 103
imm 514, 110
imm 515, 111
imm 516, 108
imm 517, 97
imm 518, 95
imm 519, 108
imm 520, 108
imm 521, 97
imm 522, 95
imm 523, 117
imm 524, 111
imm 525, 121
imm 526, 95
imm 527, 115
imm 528, 97
imm 529, 119
imm 530, 95
imm 531, 103
imm 532, 97
imm 533, 108
imm 534, 102
imm 535, 95
imm 536, 101
imm 537, 104
imm 538, 116
imm 539, 123
imm 540, 102
imm 541, 116
imm 542, 99
imm 543, 97



imm r15, 0x023
imm r14, 0x001
imm r13, 0x010

mul r15, r13
mul r15, r13
mul r15, r13
mul r14, r13
mul r14, r13
mul r14, r13

print_loop:
    jiz r15, (end_print - print_loop) >> 2
    add prog_start + ((out_loc - _start) >> 2), r15
out_loc:
    out 0x1fc
    sub prog_start + ((out_loc - _start) >> 2), r15
    sub r15, r14
    jmp prog_start + ((print_loop - _start) >> 2) - 1
end_print:
;

