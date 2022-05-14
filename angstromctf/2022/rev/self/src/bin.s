; vim: ft=nasm

global mem 

section .data

mem:
    times 0x800 dd 0
prog:
    incbin "build/obj/prog.bin"
prog_end:
    times 0x7ff - (prog_end - prog) / 4 dd 0
