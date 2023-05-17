from capstone import *
from sys import argv

md = Cs(CS_ARCH_X86, CS_MODE_64)

def allgood(bytestring, offs):
    for l in range(8):
        for flip in range(1 << (l+1)):
            prl = list(bytestring[:l+1])
            for a in range(l+1):
                if flip & (1 << a):
                    prl[a] ^= 1 << 6
            pr = bytes(prl)
            ins = list(md.disasm(pr, offs))
            if len(ins) != 1 or ins[0].size != l+1:
                continue
            print(ins[0], ins[0].size, pr)

f = open("uncertainty_r", "rb").read()
of = int(argv[1], 16)
allgood(f[of:of+8], of)
