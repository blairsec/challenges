flag = "actf{the_flag_was_you_all_along}\n"[::-1]



with open("src/prog/patched_prog.bin", "rb") as f:
    program = f.read()


from random import randbytes


key = randbytes(len(program))

encrypted = bytes( a ^ b for a, b in zip(program, key))


with open("src/prog/enc.s", "w") as f:
    f.write("prog_enc: \n")
    for n in range(len(encrypted)):
        f.write(f"db {encrypted[n]}\n")
    f.write("prog_key: \n")
    for n in range(len(key)):
        f.write(f"db {key[n]}\n")
