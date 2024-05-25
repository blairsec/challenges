import socket
from time import sleep

TCP_IP = '127.0.0.1'
TCP_PORT = 5000

def pad(string):
    '''uses PKCS7'''
    strLen = len(string)
    return string+bytes([16-len(string)%16])*(16-len(string)%16)

def get_response(message):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    s.recv(1024)
    s.recv(1024)

    sleep(0.01)
    s.send(message)
    sleep(0.01)
    data = s.recv(1024)

    s.close()

    return data

def xor(a, b):
    if len(a) != len(b): raise ValueError("Must be the same length: " + str(a) + ", " + str(b))

    l = []
    for x in range(len(a)):
        l.append(a[x] ^ b[x])

    return bytes(l)

last = b''
C2 = b'a'*16
for x in range(1, 17):
    for y in range(0, 256):
        if get_response(b'a'*(16-x) + bytes([y]) + xor(bytes([x])*(x-1), last) + C2) != b'Invalid padding':
            if get_response(b'b'*(16-x) + bytes([y]) + xor(bytes([x])*(x-1), last) + C2) != b'Invalid padding':
                print(y, x)
                last = bytes([y^x]) + last
                break
    print(len(last))

print(get_response(xor(pad(b'.flag'), last) + C2))