from pwn import *

e = ELF("./tranquil")

context.binary = e
context.terminal = ["konsole", "-e"]

p = process([e.path])


#context.log_level="debug"
#gdb.attach(p, """""")







p.interactive()
