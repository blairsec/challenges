import ctypes
from z3 import *
from tqdm import tqdm

rlen = 607
rtap = 273

class Rng:
    def __init__(self, vec=None):
        self.tap = 0
        self.feed = rlen - rtap
        if vec is None:
            self.vec = [BitVec(f"v{i}", 64) for i in range(rlen)]
        else:
            self.vec = vec

    def gen(self):
        self.tap -= 1
        if self.tap < 0:
            self.tap += rlen
        self.feed -= 1
        if self.feed < 0:
            self.feed += rlen
        x = self.vec[self.feed] + self.vec[self.tap]
        if isinstance(x, int):
            x %= 2 ** 64
        else:
            x = simplify(x)
        self.vec[self.feed] = x
        return x


rng = Rng()
with open("out.txt", "r") as f:
    nums = [int(x) for x in f.read().strip().split("\n")[4:]]

s = Solver()
for _ in tqdm(range(100000)):
    rng.gen()
secrets = [rng.gen() for _ in range(4)]
for num in nums[:607]:
    g = rng.gen()
    print(num, g)
    s.add(num == g)
if s.check() != sat:
    raise Exception("cringe unsat")
m = s.model()
print([m.evaluate(x) for x in secrets])
v = sorted(m.decls(), key=lambda x: int(x.name()[1:]))
rng = Rng([m[x].as_long() for x in v])

