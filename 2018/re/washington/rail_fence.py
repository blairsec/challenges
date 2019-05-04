import random

def fence(lst, numrails):
    fence = [[None] * len(lst) for n in range(numrails)]
    rails = list(range(numrails - 1)) + list(range(numrails - 1, 0, -1))
    for n, x in enumerate(lst):
        fence[rails[n % len(rails)]][n] = x

    return [c for rail in fence for c in rail if c is not None]

rails = int(input())
ptRaw = input()

random.seed(0x1337 ^ rails)
pt = b""
for i in ptRaw:
    pt += bytes([ord(i) ^ random.randint(0,127)])

print(''.join([hex(i)[2:] for i in fence(pt,3)]))


