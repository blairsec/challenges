import os

# Assemble the shellcode
os.system("nasm -f bin overflow3-scode.s -o overflow3-scode.bin")

# Read the shellcode and convert to a bytearray
data = open("overflow3-scode.bin",'rb').read()
data = bytearray(data)

# Write to a file
fout = open("overflow3-scode.txt",'w')
for byte in data:
    s = r"\x" + hex(byte).replace("0x",'').zfill(2)
    fout.write(s)
fout.close()
