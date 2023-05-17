#!/usr/local/bin/python

x = input("How many bananas do I have?\n")

if x == "103 bananas":
    print(open("flag.txt").read())
else:
    print("Nope")