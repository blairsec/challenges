ltf = open("not_flag", "rb")
lt = ltf.read()
lt = [a[0] + (a[1] << 8) for a in zip(lt[::2], lt[1::2])]
def delookup(short):
    return lt.index(short)
def delookup2(integer):
    return delookup(integer & 0xffff) + ((delookup(integer >> 16)) << 16)
def revone(val, a, b):
    val ^= a
    addr = delookup2(val)
    addr ^= b + (b << 16)
    return addr
def c1(arr):
    return list(map(lambda x: revone(x, 0xdead, 0xbeef), arr))
def c2(arr):
    return list(map(lambda x: revone(x, 0x1337, 0xcafe), arr))
inp = [int(x, 16) for x in input().split()]
dec = c1(c2(c2(c1(inp))))
print(''.join([chr(x & 0xff) for x in dec]))
