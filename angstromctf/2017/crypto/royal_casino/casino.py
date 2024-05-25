#!/usr/bin/python3

import os,struct

flag = "actf{flag}"

words = [struct.unpack('<L',os.urandom(4))[0] for i in range(512)]
seed = struct.unpack('<L',os.urandom(4))[0]
def randCardsWithScore():
    global seed,words
    for i in range(0,512,4):
        seed  = seed << 5 & 0xFFFFFFFF
        seed = seed + words[i] & 0xFFFFFFFF
        seed  = seed >> 6 & 0xFFFFFFFF
        seed = seed ^ (words[i+1] << 16) & 0xFFFFFFFF
        seed = seed ^ (words[i+1] >> 16) & 0xFFFFFFFF
        seed  = seed << 7 & 0xFFFFFFFF
        seed = seed ^ (words[i+2] & 0xAAAAAAAA | words[i+3] & 0x55555555)
        seed = seed >> 3 & 0xFFFFFFFF
        seed = seed ^ (words[i+2] & 0x55555555 | words[i+3] & 0xAAAAAAAA)
        seed = seed ** 2 & 0xFFFFFFFF
    words.pop(0)
    seed %= 52**5
    words.append(seed)
    numbers = []
    for i in range(4,-1,-1):
            numbers.append(seed//52**i)
            seed %= 52**i

    cards = []
    for r in numbers:
        if r%4 == 0:
            suit = ' of Spades'
        elif r%4 == 1:
            suit = ' of Hearts'
        elif r%4 == 2:
            suit = ' of Clubs'
        else:
            suit = ' of Diamonds'

        check = (r+4)//4
        if check == 1:
            val = 'Ace'
            score = 1
        elif check == 10:
            val = '10'
            score = 0
        elif check == 11:
            val = 'Jack'
            score = 0
        elif check == 12:
            val = 'Queen'
            score = 0
        elif check == 13:
            val = 'King'
            score = 0
        else:
            val = str(check)
            score = check

        cards.append((val+suit, score))
        
    return (cards)

money = 1000
shoe = randCardsWithScore()
while money > 0 and money < 1000000:
    print("You have", money, "dollars")
    gamble = int(input("Please enter an amount to bet: "))
    while gamble <= 0 or gamble > money:
        print("Invalid bet")
        gamble = int(input("Please enter an amount to bet: "))
    money -= gamble
    pos = input("Please enter which position you would like to bet on (Player, Banker, Tie): ")

    if len(shoe) < 4: shoe = randCardsWithScore()
    playerCards = [shoe.pop(0) for i in range(2)]
    bankerCards = [shoe.pop(0) for i in range(2)]

    playerScore = sum([playerCards[i][1] for i in range(2)]) % 10
    bankerScore = sum([bankerCards[i][1] for i in range(2)]) % 10

    if not playerScore in [8,9] and not bankerScore in [8,9]:
        if playerScore <= 5:
            if len(shoe) < 1: shoe = randCardsWithScore()
            playerCards.append(shoe.pop(0))
            playerScore = sum([playerCards[i][1] for i in range(3)]) % 10

            if playerCards[2][1] in [2,3] and bankerScore <= 4:
                if len(shoe) < 1: shoe = randCardsWithScore()
                bankerCards.append(shoe.pop(0))
                bankerScore = sum([bankerCards[i][1] for i in range(3)]) % 10
            elif playerCards[2][1] in [4,5] and bankerScore <= 5:
                if len(shoe) < 1: shoe = randCardsWithScore()
                bankerCards.append(shoe.pop(0))
                bankerScore = sum([bankerCards[i][1] for i in range(3)]) % 10
            elif playerCards[2][1] in [6,7] and bankerScore <= 6:
                if len(shoe) < 1: shoe = randCardsWithScore()
                bankerCards.append(shoe.pop(0))
                bankerScore = sum([bankerCards[i][1] for i in range(3)]) % 10
            elif playerCards[2][1] == 8 and bankerScore <= 2:
                if len(shoe) < 1: shoe = randCardsWithScore()
                bankerCards.append(shoe.pop(0))
                bankerScore = sum([bankerCards[i][1] for i in range(3)]) % 10
            elif playerCards[2][1] in [0,1,9] and bankerScore <= 3:
                if len(shoe) < 1: shoe = randCardsWithScore()
                bankerCards.append(shoe.pop(0))
                bankerScore = sum([bankerCards[i][1] for i in range(3)]) % 10
        elif bankerScore <= 5:
            if len(shoe) < 1: shoe = randCardsWithScore()
            bankerCards.append(shoe.pop(0))
            bankerScore = sum([bankerCards[i][1] for i in range(3)]) % 10

    print("The Banker's cards are:", bankerCards)
    print("The Banker's score is:", bankerScore)
    print("The Players's cards are:", playerCards)
    print("The Players's score is:", playerScore)
    if playerScore > bankerScore and pos.lower() == 'player':
        money += gamble*2
    elif playerScore < bankerScore and pos.lower() == 'banker':
        money += int(gamble*1.95)
    elif playerScore == bankerScore and pos.lower() == 'tie':
        money += gamble*9
                
if money >= 1000000:
    print("You made some much money I am going to give you the flag to go away")
    print(flag)

if money <= 0:
    print("You ran out of money better luck next time")

