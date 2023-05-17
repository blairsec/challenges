import random

x = "<html><body>"

idx = random.randint(3000, 5000-1)

for i in range(5000):
    x += f"<a href=\"{i}.html\">page {i}</a><br />\n"
    with open(f"src/{i}.html", "w") as f:
        if i == idx:
            f.write("actf{y0u_f0und_me_b51d0cde76739fa3}")
        else:
            f.write("your flag is in another file")

x += "</body></html>"
with open("src/index.html", "w") as f:
    f.write(x)
