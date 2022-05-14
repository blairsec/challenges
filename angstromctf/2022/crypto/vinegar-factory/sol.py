from pwn import *

alpha = string.ascii_lowercase

def decrypt(msg, key):
    ret = ""
    i = 0
    for c in msg:
        if c in alpha:
            ret += alpha[(alpha.index(c) - alpha.index(key[i])) % len(alpha)]
            i = (i + 1) % len(key)
        else:
            ret += c
    return ret


# p = process(argv=["python", "main.py"])
p = remote("challs.actf.co", 31333)

for n in range(100):
    p.recvuntil(b": ")
    m = p.recvline(keepends=False).decode()
    i = -1
    while True:
        try:
            i = m.index("{", i + 1)
        except ValueError:
            break
        if i < 4:
            continue
        if not all(x in alpha for x in m[i - 4:i]):
            continue
        key = decrypt(m[i - 4:i], "actf")
        try:
            end = m.index("}", i)
        except ValueError:
            break
        d = decrypt(m[i - 4:end + 5], key)
        if d[-4:] != "fleg":
            continue
        print(f"{n:02}: {d[:-4]}")
        p.sendlineafter(b"> ", d[:-4])
        break
