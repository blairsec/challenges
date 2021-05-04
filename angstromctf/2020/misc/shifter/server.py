from math import sqrt
import secrets
import os
import string

def find(n):
    n = ((1+sqrt(5))/2)**n - ((1-sqrt(5))/2)**n
    return int(n/sqrt(5))

def genPlain():
    return ''.join(secrets.choice(list(string.ascii_uppercase)) for _ in range(secrets.randbelow(50)))


def genN():
    return secrets.randbelow(50)


def solve(p,n):
    shift = find(n) % 26

    r = ""

    for a in p:
        r += chr((ord(a) - 65 + shift) % 26 + 65)

    return r


print("Solve 50 of these epic problems in a row to prove you are a master crypto man like Aplet123!")
print("You'll be given a number n and also a plaintext p.")
print("Caesar shift `p` with the nth Fibonacci number.")
print("n < 50, p is completely uppercase and alphabetic, len(p) < 50")
print("You have 60 seconds!")

print('-'*20)

for i in range(50):
    p = genPlain()
    n = genN()
    
    print(f"Shift {p} by n={n}")

    x = input(": ")

    ans = solve(p,n)

    if x != ans:
        print(f"Sorry, you got it wrong. The answer was {ans}. Better luck next time!")
        break

else:
    print(os.environ.get("FLAG"))
