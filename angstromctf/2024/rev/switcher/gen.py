import random

random.seed(1337)

def emit(name, table):
    return "v %s(p i){s(*i++){%sc '\\0':r;d:%s(i);r;}}" % (name, "".join(f"c '{k}':{v}(i);r;" for (k, v) in table.items() if k != "default"), table["default"])

counter = 0
decls = []
def name():
    global counter
    ret = f"f{counter}"
    counter += 1
    decls.append(f"void {ret}(char*);")
    return ret

flag = "jumping_my_way_to_the_flag_one_by_one"

valid = "abcdefghijkmlnopqrstuvwxyz_"

win_node = name()
flag_nodes = [{"name": name(), "table": {}} for _ in range(len(flag))]
decoy = [{"name": name(), "table": {}} for _ in range(1000)]
unreachable = [{"name": name(), "table": {}} for _ in range(1000)]

targets = [win_node] + [x["name"] for x in flag_nodes + decoy + unreachable]

for node in unreachable:
    for k in list(valid) + ["default"]:
        node["table"][k] = random.choice(targets)

targets = [x["name"] for x in decoy]
for node in decoy + flag_nodes:
    for k in list(valid) + ["default"]:
        node["table"][k] = random.choice(targets)

for i in range(len(flag)):
    flag_nodes[i]["table"][flag[i]] = (flag_nodes[i + 1]["name"] if i + 1 < len(flag_nodes) else win_node)

funcs = [emit(**x) for x in flag_nodes + decoy + unreachable]
random.shuffle(funcs)

with open("cases.h", "w") as f:
    f.write("#ifndef CASES_H\n#define CASES_H\n\n")
    f.write(f"#define START_NODE {flag_nodes[0]['name']}\n")
    f.write(f"#define WIN_NODE {win_node}\n")
    f.write("\n".join(decls))
    f.write("\n\n#endif\n")

with open("cases.c", "w") as f:
    f.write("#include \"cases.h\"\n#define c case\n#define r return\n#define s switch\n#define v void\n#define p char*\n#define d default\n")
    f.write("\n".join(funcs) + "\n")



