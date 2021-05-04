import socket
import time
import os
from ctypes import cdll
flag = ""
libc = cdll.LoadLibrary("libc.so.6")
def cksum(cv):
    libc.srand(cv ^ 12345678)
    rn = 0
    for d in range(8):
        rn *= 10
        rn += libc.rand() % 10
    return rn * cv
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("localhost", 21450))
i = b''
while len(i) < 13:
    i += s.recv(13 - len(i))
k = b'\xde\xad\xbe\xef\xfe\xed\xca\xfe\x13\x37\xab\xcd\xef'
i2 = bytes([k[a] ^ i[a] for a in range(13)])
v = 0x13371337
def checkc(ch):
    global v
    s.sendall(cksum(v).to_bytes(8,'little') + bytes([ord(ch)]) + i2)
    #print('sent...')
    code = s.recv(1)
    #print(code)
    #input()
    if code == b'\r': return False
    vb = b''
    while len(vb) < 4:
        resp = s.recv(1)
        vb += resp
    v = int.from_bytes(vb,'big')
    return True
while True:
    print(flag)
    for c in range(32,128):
        if checkc(chr(c)):
            flag += chr(c)
            break
    else:
        print("finished")
        break
