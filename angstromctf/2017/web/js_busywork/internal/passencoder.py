import sys
password = sys.argv[1].strip()

print(ord(password[0]))

d = []
for i in range(1, len(password)):
    d.append(ord(password[i]) - ord(password[i - 1]))

print("[" + ",".join([str(n) for n in d]) + "]")
