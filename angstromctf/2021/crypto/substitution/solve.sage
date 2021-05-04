import os

os.environ["PWNLIB_NOTERM"] = "true"

from pwn import *
from sage.all import *

context.log_level = "error"

FLAG_LEN = 80
MOD = 691

p = remote("crypto.2021.chall.actf.co", 21601)


def enc(n):
    p.sendlineafter("> ", str(n))
    p.recvuntil(">> ")
    return int(p.recvline().decode("utf8").strip())


vals = vector(mod(enc(i), MOD) for i in range(FLAG_LEN))

coeffs = Matrix(
    [
        [mod(i ** (FLAG_LEN - j - 1), MOD) for j in range(FLAG_LEN)]
        for i in range(FLAG_LEN)
    ]
)
flag = coeffs.solve_right(vals)
print("".join(chr(x) for x in flag))