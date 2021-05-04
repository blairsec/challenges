import os

os.environ["PWNLIB_NOTERM"] = "true"
from pwn import remote

io = remote('crypto.2021.chall.actf.co', 21600)

def get_num():
    io.recvuntil("Would you like to get a random output [r], or guess the next random number [g]?")
    io.sendline(b"r")
    return int(io.recvline())


fnum = get_num()
snum = get_num()
tnum = get_num()
possibilities = []
for i in divisors(fnum):
    if len(str(i)) == 8:
        possibilities.append((i, fnum//i))

def get_next_num(n):
    return int(str(n**2).rjust(8*2, "0")[8//2:8 + 8//2])

np = []

for i in possibilities:
    nnum = get_next_num(i[0]) * get_next_num(i[1])
    if nnum == snum:
        np.append(i)

assert(len(np) == 2)
# try again if len(np) != 2, it should usually be equal to 2, sometimes there might be 2 possibilities but probability low
seeds = np[0]
assert(get_next_num(get_next_num(seeds[0]))*get_next_num(get_next_num(seeds[1])) == tnum)

t = (get_next_num(get_next_num(seeds[0])), get_next_num(get_next_num(seeds[1])))

io.sendline(b"g")
io.recvuntil("What is your guess to the next value generated? ")
io.sendline(str(get_next_num(t[0])*get_next_num(t[1])).encode())
t = (get_next_num(t[0]), get_next_num(t[1]))
io.recvuntil("What is your guess to the next value generated? ")
io.sendline(str(get_next_num(t[0])*get_next_num(t[1])).encode())
print(io.recv().decode())