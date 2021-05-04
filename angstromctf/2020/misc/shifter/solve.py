from pwn import *

from math import sqrt


conn = remote("misc.2020.chall.actf.co", 20300)


def find(n):
    n = ((1+sqrt(5))/2)**n - ((1-sqrt(5))/2)**n
    return int(n/sqrt(5))

def solve(p,n):
    shift = find(n) % 26

    r = ""

    for a in p:
        r += chr((ord(a) - 65 + shift) % 26 + 65)

    return r


def chall():
    conn.recvuntil("Shift ")
    s = conn.recvuntil(": ")


    s = s.split()
    pt = s[0]
    amt = int(s[2][2:])


    ct = solve(pt, amt)

    conn.sendline(ct)

for _ in range(50):
    chall()

print(conn.recv())
