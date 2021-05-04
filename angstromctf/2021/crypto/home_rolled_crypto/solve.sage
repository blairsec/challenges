from pwn import *

p = remote('crypto.2021.chall.actf.co', 21602)

p.recvuntil(b'[2]? ')
p.sendline(b'1')
p.sendline(b'00'*16)
p.recvuntil(b'crypt: ')
c0 = int(p.recvline(keepends=False), 16)

p.recvuntil(b'[2]? ')
p.sendline(b'1')
p.sendline(b'ff'*16)
p.recvuntil(b'crypt: ')
c1 = int(p.recvline(keepends=False), 16)


xor = c0 ^ c1

p.sendline(b'2')
for i in range(10):
    p.recvuntil(b'Encrypt this: ')
    
    phex = p.recvline(keepends=False)
    print(phex)
    blocks = [int(phex[x:x+16*2], 16) for x in range(0, len(phex), 16*2)]
    c = ''
    for b in blocks:
        enc = c1
        enc |= (b ^ xor) & xor
        c += f"{enc:032x}"

    print(c)
    p.sendline(c.encode('ascii'))

print(p.recvuntil(b'[2]? '))

