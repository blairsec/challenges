from pwn import *
import time

# p = process("./simon_says")
p = remote("challs.actf.co", 31402)
context.log_level = "debug"
n = 0

while n < 20:
    data = p.recvline()
    decoded = data.decode("utf-8")
    x = decoded.split("of ", 1)
    list = x[1].split(" ")
    first = list[0]
    last = list[-1]
    response = first[:3] + last[-4:]
    time.sleep(0.01)
    p.send(response.encode("utf-8"))
    n = n + 1

flag = p.recvline()
print(flag)
