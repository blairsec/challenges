import private_set_intersection.python as psi
from pwn import *
import base64
from functools import lru_cache

def fake_psi(a, b):
    return [i for i in a if i in b]

@lru_cache
def zero_encoding(x, n):
    ret = []

    s = bin(x)[2:].zfill(n)

    for i in range(n):
        if s[i] == "0":
            ret.append(s[:i] + "1")

    return ret

def one_encoding(x, n):
    ret = []

    s = bin(x)[2:].zfill(n)

    for i in range(n):
        if s[i] == "1":
            ret.append(s[:i+1])

    return ret

def intersect(c, items):
    p.sendlineafter("set: ", str(len(items)))
    p.recvuntil("Setup: ")

    setup = psi.ServerSetup()

    setup.ParseFromString(base64.b64decode(p.recvline().strip()))

    p.sendlineafter("Request: ", base64.b64encode(c.CreateRequest(items).SerializeToString()).decode())
    
    resp = psi.Response()
    resp.ParseFromString(base64.b64decode(p.recvline()))

    return sorted([items[i] for i in c.GetIntersection(setup, resp)], key=lambda x: len(x))

context.log_level="info"

p = process(['python3', 'millionaires.py'])

p = remote("challs.actf.co", 32300)

NBITS = 256

amt = 0


# context.log_level="debug"

for i in range(10):
    stop = False
    history = []

    c = psi.client.CreateWithNewKey(True)

    

    res = intersect(c, zero_encoding(0, NBITS))
    p.sendlineafter(": ", " ".join(res))
    history.append(0)

    while True:
        if len(res) > 0:
            closest = res[-1]

        if len(closest) == NBITS:
            guess = int(closest, 2)
        else:
            guess = int(closest.ljust(NBITS, "0"), 2) | (1 << (NBITS - len(closest)-1))

        res = intersect(c, zero_encoding(guess, NBITS))
        
        p.sendlineafter(": ", " ".join(res))
        history.append(guess)
        
        p.sendlineafter("? ", "N")

        if len(res) > 0 and len(res[-1]) == NBITS:
            break

        if len(res) > 0:
            closest = res[-1]


        count = 1
        while res == []:
            if NBITS - len(closest)-count-1 < 0:
                stop = True
                break

            guess &= ~(1 << (NBITS - len(closest)-count))

            guess = int(closest.ljust(NBITS, "0"), 2) | (1 << (NBITS - len(closest)-count-1))
            
            count += 1

            res = intersect(c, zero_encoding(guess, NBITS))

            p.sendlineafter(": ", " ".join(res))
            history.append(guess)

            p.sendlineafter("? ", "N")

            if len(res) > 0 and len(res[-1]) == NBITS:
                stop = True
                break

        if stop:
            break

        res = intersect(c, zero_encoding(guess, NBITS))
        
        p.sendlineafter(": ", " ".join(res))
        history.append(guess)
        
        p.sendlineafter("? ", "N")

        if len(res) > 0 and len(res[-1]) == NBITS:
            break

    guess ^= 1

    res = intersect(c, zero_encoding(guess, NBITS))

    p.sendlineafter(": ", " ".join(res))
    history.append(guess)

    if res != []:
        guess += 1

    p.sendlineafter("? ", "y")
    p.sendline(str(guess))

    for i in history:
        p.sendlineafter(": ", str(i))

    amt += 1

    print(f"Guessed {amt}")
    
p.interactive()