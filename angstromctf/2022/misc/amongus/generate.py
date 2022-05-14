import secrets

data = secrets.token_hex(10000)

for i in range(1000):
    nonce = secrets.token_hex(6)
    with open("out/actf{look1ng_f0r_answers_in_the_p0uring_r4in_%s}.txt" % nonce, "w") as f:
        f.write(data)

