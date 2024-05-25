# coding: utf-8
cipher = open("flag.encrypted", "rb").read()
exstart = open("in.png", "rb").read()[:16]
key = [cipher[i] ^ exstart[i] for i in range(16)]
plain = [cipher[i] ^ key[i % 16] for i in range(len(cipher))]
p = bytes(plain)
f = open("out.png", "wb")
f.write(p)
f.close()
