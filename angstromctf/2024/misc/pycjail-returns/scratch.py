from ops import ops
import dis

names = ("breakpoint",)
consts = (None,)

cod = """
LOAD_GLOBAL_BUILTIN 1
SEND_GEN 0 *4
CALL_PY_EXACT_ARGS 0
SEND_GEN 0 *3
LOAD_ATTR_SLOT 0
""".strip()

bc = b""
for c in cod.split("\n"):
    if c.startswith("#"):
        continue
    l = c.split()
    if len(l) == 1 or l[1].startswith("*"):
        l.insert(1, "0")
    if len(l) == 2:
        l.append("*1")
    [a, b, c] = l
    if not c.startswith("*"):
        raise ValueError("bad mult")
    bc += bytes([ops[a], int(b)] * int(c[1:]))

def f():
    return

f.__code__ = f.__code__.replace(co_names=names, co_consts=consts, co_code=bc, co_stacksize=128)

# print(len(bc))
# print(f())
print(bc.hex())
print(names[0])
