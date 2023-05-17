from pwn import *

context.log_level="info"

p = process("./a.out")

x = eval(open("chars.txt").read())

p.sendline(" ".join([str(i) for i in x]))

p.interactive()