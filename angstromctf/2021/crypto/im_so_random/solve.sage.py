

# This file was *autogenerated* from the file solve.sage
from sage.all_cmdline import *   # import sage library

_sage_const_21600 = Integer(21600); _sage_const_8 = Integer(8); _sage_const_2 = Integer(2); _sage_const_0 = Integer(0); _sage_const_1 = Integer(1)
import os

os.environ["PWNLIB_NOTERM"] = "true"
from pwn import remote

io = remote('crypto.2021.chall.actf.co', _sage_const_21600 )

def get_num():
    io.recvuntil("Would you like to get a random output [r], or guess the next random number [g]?")
    io.sendline(b"r")
    return int(io.recvline())


fnum = get_num()
snum = get_num()
tnum = get_num()
possibilities = []
for i in divisors(fnum):
    if len(str(i)) == _sage_const_8 :
        possibilities.append((i, fnum//i))

def get_next_num(n):
    return int(str(n**_sage_const_2 ).rjust(_sage_const_8 *_sage_const_2 , "0")[_sage_const_8 //_sage_const_2 :_sage_const_8  + _sage_const_8 //_sage_const_2 ])

np = []

for i in possibilities:
    nnum = get_next_num(i[_sage_const_0 ]) * get_next_num(i[_sage_const_1 ])
    if nnum == snum:
        np.append(i)

assert(len(np) == _sage_const_2 )
# try again if len(np) != 2, it should usually be equal to 2, sometimes there might be 2 possibilities but probability low
seeds = np[_sage_const_0 ]
assert(get_next_num(get_next_num(seeds[_sage_const_0 ]))*get_next_num(get_next_num(seeds[_sage_const_1 ])) == tnum)

t = (get_next_num(get_next_num(seeds[_sage_const_0 ])), get_next_num(get_next_num(seeds[_sage_const_1 ])))

io.sendline(b"g")
io.recvuntil("What is your guess to the next value generated? ")
io.sendline(str(get_next_num(t[_sage_const_0 ])*get_next_num(t[_sage_const_1 ])).encode())
t = (get_next_num(t[_sage_const_0 ]), get_next_num(t[_sage_const_1 ]))
io.recvuntil("What is your guess to the next value generated? ")
io.sendline(str(get_next_num(t[_sage_const_0 ])*get_next_num(t[_sage_const_1 ])).encode())
print(io.recv().decode())
