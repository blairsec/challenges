import os

# Assemble the shellcode
os.system("nasm -f bin shellcode-scode.s -o shellcode-scode.bin")

# Read the shellcode and convert to a bytearray
data = open("shellcode-scode.bin",'rb').read()
data = bytearray(data)

# Write to a file
fout = open("shellcode-scode.txt",'w')
for byte in data:
    s = r"\x" + hex(byte).replace("0x",'').zfill(2)
    fout.write(s)
fout.close()
