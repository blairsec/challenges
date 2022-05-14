from sage.all import *
from tqdm import tqdm

rlen = 607
rtap = 273

def add_coeffs(a, b):
    return [(x + y) % 2 ** 64 for (x, y) in zip(a, b)]

class Rng:
    def __init__(self, vec=None):
        self.tap = 0
        self.feed = rlen - rtap
        if vec is None:
            self.vec = [[0] * rlen for i in range(rlen)]
            for i in range(rlen):
                self.vec[i][i] = 1
        else:
            self.vec = vec

    def gen(self):
        self.tap -= 1
        if self.tap < 0:
            self.tap += rlen
        self.feed -= 1
        if self.feed < 0:
            self.feed += rlen
        if isinstance(self.vec[self.feed], int) or isinstance(self.vec[self.feed], Integer):
            x = (self.vec[self.feed] + self.vec[self.tap]) % 2 ** 64
        else:
            x = add_coeffs(self.vec[self.feed], self.vec[self.tap])
        self.vec[self.feed] = x
        return x


rng = Rng()
with open("out.txt", "r") as f:
    nums = [int(x) for x in f.read().strip().split("\n")[4:]]

for _ in tqdm(range(100000)):
    rng.gen()
secrets = [rng.gen() for _ in range(4)]
m = []
gap = 0
for num in nums:
    g = rng.gen()
    m.append(g)
    for _ in range(gap):
        rng.gen()
    gap = (gap + 1) % 13
m = matrix(Integers(2**64), rlen, rlen, m)
print("solving matrix...")
print(m.det())
v = m.solve_right(vector(nums))
print(v)
rng = Rng([int(v) for v in v])
for _ in tqdm(range(100000)):
    rng.gen()
for i in range(10):
    print(rng.gen())

