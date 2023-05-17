import random
import numpy as np


f = open("flag.txt").read().strip()

flag = [ord(i) for i in f]

with open("chars.txt", "w") as f_:
    f_.write(str(flag))


assert len(f) > 1000

vectors = []

for i in range(len(f)):
    vectors.append([random.randint(0, 1000) for i in range(len(f))])


assert np.linalg.slogdet(vectors)[0] != 0

check = np.transpose(np.array(vectors)).dot(np.array(flag))

template = """#include <stdio.h>
#include <stdlib.h>

long long check[{arrsize}];
long long needed[{arrsize}] = {arr};


{functions}

void main(){{
    int amt = 0;
    {readinput}
    int i = 0;
    for(; i < {arrsize}; ++i){{
        if (check[i] != needed[i]) {{
            printf("%d %d %d", i, check[i], needed[i]);
            puts("wrong");
            exit(1);
        }}
    }}

    if (i == {arrsize}) {{
        puts("right");
    }}
}}
"""

read = ""
out = ""

for i, fx in enumerate(vectors):
    r = """printf("> ");
    scanf("%d", &amt);

    for(int c = 0; c < amt; ++c){{
        func{i}();
    }}\n\n
    """

    read += r.format(i=i)


    o = """void func{i}(){{
{assignments}
}}\n\n"""

    a = ""

    for k, v in enumerate(fx):
        a += f"\tcheck[{k}] += {v};\n"

    out += o.format(i=i, assignments=a)

program = template.format(
    arr="{" + str(check.tolist())[1:-1] + "}",
    arrsize=len(check),
    readinput=read,
    functions=out,
)

with open("moon.c", "w") as f:
    f.write(program)

