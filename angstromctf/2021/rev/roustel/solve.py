mem = [int(x, 16) for x in input("hex bytes separated by spaces ").split()] + [0,0]
i = 0
while True:
    for b in range(len(mem)-3, -1, -1):
        mem[b] = (mem[b+1] ^ (mem[b] - mem[b+2])) % 256
    print(i, repr(''.join([chr(c) for c in mem[:-2]])))
    i += 1
    if i == 260: # prevent off-by-one and also let you check for like periodic behavior or sth
        input("probably done, enter to continue anyway")
