#!/usr/bin/env python3
import subprocess, fcntl, os, sys, selectors
os.chdir("app")
p = subprocess.Popen(["python3", "alphabeta.py"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# set files descriptors to nonblocking and create selectors
fcntl.fcntl(p.stdout, fcntl.F_SETFL, fcntl.fcntl(p.stdout, fcntl.F_GETFL) | os.O_NONBLOCK)
fcntl.fcntl(sys.stdin, fcntl.F_SETFL, fcntl.fcntl(sys.stdin, fcntl.F_GETFL) | os.O_NONBLOCK)
selector = selectors.DefaultSelector()
selector.register(sys.stdin, selectors.EVENT_READ, 'stdin')
selector.register(p.stdout, selectors.EVENT_READ, 'stdout')
whitelist = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
# until the program has finished
while p.poll() == None:
	events = selector.select()
	for key, mask in events:
		if key.data == 'stdin':
			# write input
			line = key.fileobj.readline()
			for c in line:
				if c not in whitelist:
					print("That's not AlphaBeta-cal!")
					sys.exit()
			p.stdin.write(bytes(line, encoding="utf-8"))
			p.stdin.flush()
		elif key.data == 'stdout':
			# print output
			output = key.fileobj.read()
			sys.stdout.write(output.decode("utf-8"))
			sys.stdout.flush()
output, error = p.communicate()
if error: sys.stdout.write(error.decode("utf-8"))
sys.stdout.flush()
