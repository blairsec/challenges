from Crypto.Util.number import long_to_bytes
from pwn import *

p = process(["python3", "rsa2.py"])

p.recvuntil("n = ")
n = int(p.recvline().strip())
e = 65537
p.recvuntil("c = ")
c = int(p.recvline().strip())


p.sendlineafter("decrypt: ", str(c * pow(2, e, n)))

p.recvuntil("m = ")

m = int(p.recvline())

print(long_to_bytes(m//2))
p.close()