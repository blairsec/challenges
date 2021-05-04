from Crypto.Util.number import getPrime, bytes_to_long, long_to_bytes

flag = 'actf{d0ggy!!!111!1}'

flag = bytes_to_long(flag.encode())

p = getPrime(1024)
q = getPrime(1024)
n = p*q
phi = (p-1)*(q-1)
d = getPrime(64)
e = pow(d, -1, phi)
assert (e*d)%phi == 1

ciphertext = pow(flag, e, n)

print(f'n: {n}')
print(f'e: {e}')
print(f'c: {ciphertext}')
