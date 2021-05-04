from pwn import *

context.log_level = logging.ERROR

import os
os.chdir("..")

p = process("califrobnication")

import time
t = int(time.time())

p.recvuntil(": ")

flag = p.recvall()[:-1]
flag = "".join([chr(ord(c)^42) for c in flag])

print(t)
print(flag)
