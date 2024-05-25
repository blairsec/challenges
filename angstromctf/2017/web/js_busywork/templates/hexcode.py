def get(n, data):
    return "".join([hex(ord(ch))[2:] for ch in data])
