#!/usr/bin/python3
import os,struct

flag = "actf{flag}"

def randCard():
    mod1 = struct.unpack('<L',os.urandom(4))[0]
    mod2 = struct.unpack('<L',os.urandom(4))[0]
    mod3 = struct.unpack('<L',os.urandom(4))[0]

    mult1 = struct.unpack('<L',os.urandom(4))[0] % mod1
    mult2 = struct.unpack('<L',os.urandom(4))[0] % mod2
    mult3 = struct.unpack('<L',os.urandom(4))[0] % mod3

    s1 = (mult1 * struct.unpack('<L',os.urandom(4))[0]) % mod1
    s2 = (mult2 * struct.unpack('<L',os.urandom(4))[0]) % mod2
    s3 = (mult3 * struct.unpack('<L',os.urandom(4))[0]) % mod3

    r = int((mod1/(s1+1) + mod2/(s2+1) + mod3/(s3+1))*13) % 52

    if r%4 == 0:
        suit = ' of Spades'
    elif r%4 == 1:
        suit = ' of Hearts'
    elif r%4 == 2:
        suit = ' of Clubs'
    else:
        suit = ' of Diamonds'

    if (r+3)//4 == 0:
        val = 'Ace'
    elif (r+3)//4 == 9:
        val = '10'
    elif (r+3)//4 == 10:
        val = 'Jack'
    elif (r+3)//4 == 11:
        val = 'Queen'
    elif (r+3)//4 == 12:
        val = 'King'
    else:
        val = str((r+3)//4+1)

    return (val+suit)

money = 10000
maxBet = 100
while money > 0 and money < 100000:
    print("You have",money,"dollars")
    gamble = int(input("Please enter an amount to bet (1-100): "))
    while gamble <= 0 or gamble > maxBet:
        print("Invalid bet")
        gamble = int(input("Please enter an amount to bet: "))
    money -= gamble
    guess = input("Please enter what the next card will be: ")

    newCard = randCard()
    print("The card is " + newCard)
    if newCard.lower() == guess.lower():
        print("You win",gamble*50,"dollars.")
        money += gamble * 50
    else:
        print("Your guess was wrong!")

if money >= 100000:
    print("You made so much money I am going to give you the flag to go away")
    print(flag)

if money <= 0:
    print("You ran out of money better luck next time")
    
    
