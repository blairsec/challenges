from pwn import *
import random
import time
import base64


def otp(a, b):
	r = ""
	for i, j in zip(a, b):
		r += chr(ord(i) ^ ord(j))
	return r


def genSample():
	p = ''.join([string.ascii_letters[random.randint(0, len(string.ascii_letters)-1)] for _ in range(random.randint(1, 30))])
	k = ''.join([string.ascii_letters[random.randint(0, len(string.ascii_letters)-1)] for _ in range(len(p))])

	x = otp(p, k)

	return x, p, k



conn = remote("misc.2020.chall.actf.co", 20301)

seed = int(time.time())
random.seed(seed)

sample_x, sample_p, sample_k = genSample()


conn.sendlineafter("> ", "2")

need = conn.recvline().strip()
data = conn.recvuntil(": ")


d = base64.b64decode(need).decode()

offset = -20
while sample_x != d:
        random.seed(seed+offset)
        sample_x, sample_p, sample_k = genSample()
        
        offset += 1

conn.sendline(sample_p)
print(conn.recv().decode())



