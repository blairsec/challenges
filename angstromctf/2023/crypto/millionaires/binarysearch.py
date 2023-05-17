from pwn import *


context.log_level="info"

p = process(['python3', 'millionaires.py'])

low = 0
high = 2**(2**10)-1
mid = (low + high) // 2

p.sendlineafter(": ", str(mid))
tries = 1

r = p.recvline()

while b"done" not in r:
    r = eval(r)

    if r == []:
        high = mid
        mid = (low + high) // 2
    else:
        low = mid
        mid = (low + high) // 2

    p.sendlineafter(": ", str(mid))
    tries += 1

    r = p.recvline()

print(tries)