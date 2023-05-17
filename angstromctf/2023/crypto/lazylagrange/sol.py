from pwn import *

K = 128

# p = process(["python3", "server.py"])
p = remote("challs.actf.co", 32100)

p.sendlineafter(": ", "1")
p.sendlineafter("> ", str(K))


out1 = p.recvline()

a = []
y = int(out1)
n = 0
while y:
	a.append(y % K)
	y //= K
	n += 1

p.sendlineafter(": ", "2")
p.sendlineafter("> ", ' '.join([str(i) for i in a]))

out2 = p.recvline()

p.close()

p = [int(x) for x in out2.split()]
flag = [0] * n
for i in range(n):
	flag[p[i]] = a[i]

print('flag: ' + ''.join(chr(x) for x in flag))