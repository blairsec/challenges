import private_set_intersection.python as psi
from pwn import *
import base64

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

    return [items[i] for i in c.GetIntersection(setup, resp)]

p = process(["python3", "server.py"])

clientItems = zero_encoding(23324, 1024)

c = psi.client.CreateWithNewKey(True)

# stuff here


p.interactive()