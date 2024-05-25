def tri(n):
    return int(n * (n + 1) / 2)

def get(n, js):
    return ",".join([str(ord(js[0]))] + [str(ord(js[i]) - ord(js[i - 1]) - tri(i)) for i in range(1, len(js))])
