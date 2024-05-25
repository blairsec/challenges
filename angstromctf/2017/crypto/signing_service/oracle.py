#!/usr/bin/python

from Crypto.PublicKey import RSA
from Crypto.Hash import MD5

f = open('key.pem','r')
key = RSA.importKey(f.read())

while True:
    data = raw_input("Enter string to sign: ")
    print hex(key.sign(data,'')[0])[2:-1]
