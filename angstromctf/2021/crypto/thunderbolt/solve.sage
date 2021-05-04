import os
os.environ["PWNLIB_NOTERM"] = "true"

from pwn import remote
import binascii

io = remote("crypto.2021.chall.actf.co", 21603)

io.recvuntil("encrypt: ")
io.sendline("a" * 400000)
print(io.recvline())


