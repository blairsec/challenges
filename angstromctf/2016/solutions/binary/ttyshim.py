#!/usr/bin/python
import os, sys
import subprocess, time
import pty
import select

# Create the process
master, slave = pty.openpty()
process = subprocess.Popen(["./bank_roppery"], stdin=subprocess.PIPE, stdout=slave)
sout = os.fdopen(master, "rb", 0)

# Constantly receive input and write output
while True:
	#start = time.time()
	#while time.time() - start < 4.0:
	#	poll_result = poll_obj.poll(0)
	#	if poll_result:
	#		line = sout.readline()
	#		sys.stdout.write(line)
	#		sys.stdout.flush()
	#for i in range(0,6):
	#	sys.stdout.write(sout.read())
	#	sys.stdout.flush()

	#print("Begin printing")
	start = time.time()
	while time.time() - start < 1.0:
		poll_result = select.select([sout], [], [], 0.0001)
		#print poll_result
		if sout in poll_result[0]:
			outdata = sout.read(1)
			sys.stdout.write(outdata)
			sys.stdout.flush()

	if process.poll() != None: sys.exit(0)

	#print("Begin reading")
	indata = raw_input()
	process.stdin.write(indata + "\n")
