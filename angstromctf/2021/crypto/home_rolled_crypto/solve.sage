import os
os.environ["PWNLIB_NOTERM"] = "true"

from pwn import remote
import binascii

io = remote("", 0)

def encrypt(msg):
    io.recvuntil("Would you like to encrypt [1], or try encrypting [2]? ")
    io.sendline(b"1")
    io.recvuntil("What would you like to encrypt: ")
    io.sendline(binascii.hexlify(msg))
    return io.recvline()[:-1].decode()

